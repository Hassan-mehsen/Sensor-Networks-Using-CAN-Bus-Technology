#include "main.h"
#include "MadgwickAHRS.h"
//###################################################################
#define VL6180X	0
#define MPU9250	1
#define DYN_ANEMO_PRESS 0
//###################################################################

//====================================================================
//			CAN ACCEPTANCE FILTER
//====================================================================
#define USE_FILTER	1
// Can accept until 4 Standard IDs
#define ID_1	0x01  // Range
#define ID_2	0x02  // accéléromètre
#define ID_3	0x03  // Moteur
#define ID_4	0x04
//====================================================================
extern void systemClock_Config(void);

void (*rxCompleteCallback) (void);
void can_callback(void);
CAN_Message      rxMsg;
CAN_Message      txMsg;
long int        counter = 0;

uint8_t* aTxBuffer[2];

extern float magCalibration[3];

void VL6180x_Init(void);
void VL6180x_Step(void);

void rangeLux_Callback(void);
void anemo_Callback(void);
void humiditePression_Callback(void);

int status;
int new_switch_state;
int switch_state = -1;

uint8_t anemo = 0;
uint8_t on_cmd = 0;
uint8_t range_lux = 0;

uint8_t lsb = 0;
uint8_t msb = 0;
uint8_t lsb2 = 0;
uint8_t msb2 = 0;
uint8_t i = 0;
uint8_t sendCan = 0;

int16_t acc[3];
int16_t gyr[3];
float acc_x, acc_y, acc_z, gyr_x, gyr_y, gyr_z;

float angles[3] = {0.0f, 0.0f, 0.0f};
float m_rot[5];

//====================================================================
// >>>>>>>>>>>>>>>>>>>>>>>>>> MAIN <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//====================================================================
int main(void) {
	HAL_Init();
	systemClock_Config();
    SysTick_Config(HAL_RCC_GetHCLKFreq() / 1000); //SysTick end of count event each 1ms
	uart2_Init();
	uart3_Init();
	i2c1_Init();

#if DYN_ANEMO_PRESS
    anemo_Timer1Init();
#endif

	HAL_Delay(1000); // Wait

#if VL6180X
    VL6180x_Init();

    // pression
    i = lps22hb_whoAmI();
    lps22hb_setup();

    // humidite
	hts221_activate();
	i = hts221_whoAmI();
	hts221_storeCalibration();
#endif

#if MPU9250
    mpu9250_InitMPU9250();
    mpu9250_CalibrateMPU9250();
#if USE_MAGNETOMETER
    mpu9250_InitAK8963(magCalibration);
#endif
    uint8_t response=0;
	response =  mpu9250_WhoAmI();
	//term_printf("%d",response);
#endif


    can_Init();
    can_SetFreq(CAN_BAUDRATE); // CAN BAUDRATE : 500 MHz -- cf Inc/config.h
#if USE_FILTER
    can_Filter_list((ID_1<<21)|(ID_2<<5) , (ID_3<<21)|(ID_4<<5) , CANStandard, 0); // Accept until 4 Standard IDs
#else
    can_Filter_disable(); // Accept everybody
#endif
    can_IrqInit();
    can_IrqSet(&can_callback);

    txMsg.id=0x55;
    txMsg.data[0]=1;
    txMsg.data[1]=2;
    txMsg.len=2;
    txMsg.format=CANStandard;
    txMsg.type=CANData;

    can_Write(txMsg);

    // Décommenter pour utiliser ce Timer ; permet de déclencher une interruption toutes les N ms
    tickTimer_Init(200);
#if MPU9250
    tickTimer_Init(10); // period in ms
#endif

#if DYN_ANEMO_PRESS
   // TEST MOTEUR
    dxl_LED(1, LED_ON);
    HAL_Delay(500);
    dxl_LED(1, LED_OFF);
    HAL_Delay(500);

    dxl_torque(1, TORQUE_OFF);
    dxl_setOperatingMode(1, VELOCITY_MODE);
    dxl_torque(1, TORQUE_ON);
    dxl_setGoalVelocity(1, 140);
    HAL_Delay(1000);
    dxl_setGoalVelocity(1, 0);
#endif

    while (1) {

#if DYN_ANEMO_PRESS
    	txMsg.len=1;
    	if(anemo > 20 || on_cmd == 1) {
    		dxl_setGoalVelocity(1, 140);
    		HAL_Delay(1000);
    	} else {
    		dxl_setGoalVelocity(1, 0);
    	}
#endif

#if VL6180X
    	txMsg.len=8;
		VL6180x_Step();
		rangeLux_Callback();
		humiditePression_Callback();
#endif

#if MPU9250
		txMsg.id = 0x95;
		txMsg.len=3;
		txMsg.data[0] = ((int) angles[0]); // phi
		txMsg.data[1] = ((int) angles[1]); // theta
		txMsg.data[2] = ((int) angles[2]); // psi
#endif

    	if(sendCan == 1) {
        	can_Write(txMsg);
        	sendCan = 0;
        }
    }
	return 0;
}


//====================================================================
//			CAN CALLBACK RECEPT
//====================================================================

void can_callback(void) {
	CAN_Message msg_rcv;
	can_Read(&msg_rcv);
	txMsg.format=CANStandard;
	txMsg.type=CANData;

#if VL6180X
	if(msg_rcv.id == ID_1) {
		if(msg_rcv.data[0] == 0x10) {
			txMsg.id=0x65;
			range_lux = 1;
		} else if(msg_rcv.data[0] == 0x20) {
			txMsg.id=0x75;
			range_lux = 0;
		}
	}
#endif

#if MPU9250
	if(msg_rcv.id == ID_2);
#endif

#if DYN_ANEMO_PRESS
    if(msg_rcv.id == ID_3) {
    	if(msg_rcv.data[0] == 0x10) {
    		on_cmd = 1;
    	} else if(msg_rcv.data[0] == 0x20) {
    		on_cmd = 0;
    	}
    }
#endif
}

/*
 * ANEMO Callback:
 * > 4 front montant par second = 1 tourne par second (Hz)
 * > 1 Hz = 1 km/h
 * > anemo_GetCount() = front montant par 200 ms
 * > (anemo_GetCount()*5) = front montant par second
 */
void anemo_Callback(void) {
	txMsg.id=0x85;
	anemo = (anemo_GetCount()*5)/4;
	txMsg.data[0] = anemo;
}

/*
 * RANGE LUX Callback:
 * > Als.lux a 32 bits
 * > Range a 16 bits
 */
void rangeLux_Callback(void) {
	if(range_lux) {
		msb = (Als.lux & 0xFF000000) >> 24;
		msb2 = (Als.lux & 0x00FF0000) >> 16;
		lsb2 = (Als.lux & 0x0000FF00) >> 8;
		lsb = (Als.lux & 0x000000FF);

		txMsg.data[0] = lsb;
		txMsg.data[1] = lsb2;
		txMsg.data[2] = msb2;
		txMsg.data[3] = msb;
	} else {
		msb = (range & 0xFF00) >> 8;
		lsb = (range & 0x00FF);

		txMsg.data[0] = lsb;
		txMsg.data[1] = msb;
		txMsg.data[2] = 0x00;
		txMsg.data[3] = 0x00;
	}
}

void humiditePression_Callback(void) {
	static float pres_value, temp1_value, temp2_value, humi_value;

	i = lps22hb_getPressure(&pres_value);
	i = lps22hb_getTemperature(&temp1_value);
	i = hts221_getTemperature(&temp2_value);
	i = hts221_getHumidity(&humi_value);

	txMsg.data[4] = (uint8_t) pres_value;
	txMsg.data[5] = (uint8_t) temp1_value;
	txMsg.data[6] = (uint8_t) temp2_value;
	txMsg.data[7] = (uint8_t) humi_value;
}

void accelerometre_Callback(void) {
	mpu9250_ReadAccelData(acc);
	mpu9250_ReadGyroData(gyr);

	acc_x = acc[0]*(4.0f/32768.0f);
	acc_y = acc[1]*(4.0f/32768.0f);
	acc_z = acc[2]*(4.0f/32768.0f);

	gyr_x = gyr[0]*((1000.0f/32768.0f)*(M_PI/180.0f));
	gyr_y = gyr[1]*((1000.0f/32768.0f)*(M_PI/180.0f));
	gyr_z = gyr[2]*((1000.0f/32768.0f)*(M_PI/180.0f));

	MadgwickAHRSupdateIMU(gyr_x, gyr_y, gyr_z, acc_x, acc_y, acc_z);

	m_rot[0]= 2*(q1*q2+q0*q3);
	m_rot[1]= ((q0*q0)+(q1*q1))-((q2*q2)-(q3*q3));
	m_rot[2]= 2*(q0*q1+q2*q3);
	m_rot[3]= 2*((q1*q3)-(q0*q2));
	m_rot[4]= ((q0*q0)-(q1*q1))-((q2*q2)+(q3*q3));

	angles[0] = atan2f(m_rot[2],m_rot[4])*180.0f/M_PI;
	angles[1] = -asinf(m_rot[3])*180.0f/M_PI;
	angles[2] = atan2f(m_rot[0],m_rot[1])*180.0f/M_PI;
}

//====================================================================
//			TIMER CALLBACK PERIOD
//====================================================================

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
#if DYN_ANEMO_PRESS
	anemo_Callback();
	anemo_ResetCount();
#endif

#if MPU9250
	accelerometre_Callback();
#endif
	sendCan = 1;
}
//====================================================================

#if VL6180X
void VL6180x_Init(void) {
	uint8_t id;
	State.mode = 1;

    XNUCLEO6180XA1_Init();
    HAL_Delay(500); // Wait
    // RESET
    XNUCLEO6180XA1_Reset(0);
    HAL_Delay(10);
    XNUCLEO6180XA1_Reset(1);
    HAL_Delay(1);

    HAL_Delay(10);
    VL6180x_WaitDeviceBooted(theVL6180xDev);
    id=VL6180x_Identification(theVL6180xDev);
    term_printf("id=%d, should be 180 (0xB4) \n\r", id);
    VL6180x_InitData(theVL6180xDev);

    State.InitScale=VL6180x_UpscaleGetScaling(theVL6180xDev);
    State.FilterEn=VL6180x_FilterGetState(theVL6180xDev);

     // Enable Dmax calculation only if value is displayed (to save computation power)
    VL6180x_DMaxSetState(theVL6180xDev, DMaxDispTime>0);

    switch_state=-1 ; // force what read from switch to set new working mode
    State.mode = AlrmStart;
}
//====================================================================
void VL6180x_Step(void)
{
    DISP_ExecLoopBody();

    // new_switch_state = XNUCLEO6180XA1_GetSwitch();
    new_switch_state = range_lux;
    if (new_switch_state != switch_state) {
        switch_state=new_switch_state;
        status = VL6180x_Prepare(theVL6180xDev);
        // Increase convergence time to the max (this is because proximity config of API is used)
        VL6180x_RangeSetMaxConvergenceTime(theVL6180xDev, 63);
        if (status) {
            AbortErr("ErIn");
        }
        else{
            if (switch_state == SWITCH_VAL_RANGING) {
                VL6180x_SetupGPIO1(theVL6180xDev, GPIOx_SELECT_GPIO_INTERRUPT_OUTPUT, INTR_POL_HIGH);
                VL6180x_ClearAllInterrupt(theVL6180xDev);
                State.ScaleSwapCnt=0;
                DoScalingSwap( State.InitScale);
            } else {
                 State.mode = RunAlsPoll;
                 InitAlsMode();
            }
        }
    }

    switch (State.mode) {
    case RunRangePoll:
        RangeState();
        break;

    case RunAlsPoll:
        AlsState();
        break;

    case InitErr:
        TimeStarted = g_TickCnt;
        State.mode = WaitForReset;
        break;

    case AlrmStart:
       GoToAlaramState();
       break;

    case AlrmRun:
        AlarmState();
        break;

    case FromSwitch:
        // force reading swicth as re-init selected mode
        switch_state=!XNUCLEO6180XA1_GetSwitch();
        break;

    case ScaleSwap:

        if (g_TickCnt - TimeStarted >= ScaleDispTime) {
            State.mode = RunRangePoll;
            TimeStarted=g_TickCnt; /* reset as used for --- to er display */
        }
        else
        {
        	DISP_ExecLoopBody();
        }
        break;

    default: {
    	 DISP_ExecLoopBody();
          if (g_TickCnt - TimeStarted >= 5000) {
              NVIC_SystemReset();
          }
    }
    }
}
#endif
//====================================================================

