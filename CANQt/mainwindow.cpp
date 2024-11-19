#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore/qmath.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *principalscreen = new QWidget;
    setCentralWidget(principalscreen);
    setWindowTitle("CAN User Interface");
    this->setStyleSheet("background-color: white;");

    // Layout
    principal_layout = new QGridLayout();

    temp_layout = new QGridLayout();
    db_layout = new QGridLayout();
    spw_layout = new QGridLayout();
    hum_layout = new QGridLayout();
    press_layout = new QGridLayout();
    mot_layout = new QGridLayout();
    angle0_layout = new QGridLayout();
    angle1_layout = new QGridLayout();
    angle2_layout = new QGridLayout();

    // GroupBox
    hum_box = new QGroupBox("Humidity");
    hum_box->setLayout(hum_layout);

    temp_box = new QGroupBox("Temperature");
    temp_box->setLayout(temp_layout);

    db_box = new QGroupBox("Distance/Brightness");
    db_box->setLayout(db_layout);

    spw_box = new QGroupBox("Wind Speed");
    spw_box->setLayout(spw_layout);

    press_box = new QGroupBox("Pressure");
    press_box->setLayout(press_layout);

    mot_box = new QGroupBox("Motor");
    mot_box->setLayout(mot_layout);

    angle0_box = new QGroupBox("Phi");
    angle0_box->setLayout(angle0_layout);

    angle1_box = new QGroupBox("Theta");
    angle1_box->setLayout(angle1_layout);

    angle2_box = new QGroupBox("Psi");
    angle2_box->setLayout(angle2_layout);

    // Qline
    press_line = new QLineEdit();
    db_line = new QLineEdit();
    temp_line = new QLineEdit();
    spw_line = new QLineEdit();
    angle0_line = new QLineEdit();
    angle1_line = new QLineEdit();
    angle2_line = new QLineEdit();

    // ProgressBar
    hum_progress = new QProgressBar(this);
    hum_progress->setRange(0, 100);
    hum_progress->setValue(0);
    hum_progress->setStyleSheet("QProgressBar {"
                                 "border: 2px solid #0099CC;" // Bordure bleu clair
                                 "border-radius: 5px;"
                                 "background-color: #E6F7FF;" // Fond léger pour le progrès
                                 "text-align: center;"
                                 "color: #003366;" // Couleur du texte bleu foncé
                                 "font-weight: bold;"
                                 "}"
                                 "QProgressBar::chunk {"
                                 "background-color: #66CCFF;" // Couleur du progrès
                                 "border-radius: 5px;"
                                 "}");

    // Boutons
    b_RadioButton = new QRadioButton("Brightness");
    b_RadioButton->setChecked(true);
    d_RadioButton = new QRadioButton("Distance");

    reset = new QPushButton("RESET");
    start = new QPushButton("START");
    onOff = new QPushButton("Off");
    onOff->setStyleSheet("QPushButton { background-color: #444444; }");
    but_send = new QPushButton("Send Message");

    // Unités Label
    MPascal = new QLabel("HPa");
    lc = new QLabel("lux");
    deg = new QLabel("°C");
    speed = new QLabel("km/h");

    // Créer une vue
    graph = new QGraphicsView();

    // Ajouter les widgets au layout
    principal_layout->addWidget(spw_box, 0, 0);  // Wind Speed à gauche
    principal_layout->addWidget(press_box, 1, 0); // Pressure à gauche
    principal_layout->addWidget(hum_box, 2, 0);   // Humidity à gauche
    principal_layout->addWidget(temp_box, 3, 0);  // Temperature à gauche
    principal_layout->addWidget(mot_box, 4, 0);   // Motor à gauche
    principal_layout->addWidget(db_box, 5, 0);    // Distance/Brightness à gauche


    principal_layout->addWidget(angle0_box, 0, 1); // Phi à droite
    principal_layout->addWidget(angle1_box, 1, 1); // Theta à droite
    principal_layout->addWidget(angle2_box, 2, 1); // Psi à droite


    Object_GL = new ObjectOpenGL();
    Object_GL->setObjectName("ObjectOpenGL");
    Object_GL->setGeometry(QRect(0, 0, this->width(), this->width()));
    principal_layout->addWidget(Object_GL, 3, 1, 3, 1);


    angle0_layout->addWidget(angle0_line, 0, 0);
    angle0_layout->addWidget(deg, 0, 1);

    angle1_layout->addWidget(angle1_line, 0, 0);
    angle1_layout->addWidget(deg, 0, 1);

    angle2_layout->addWidget(angle2_line, 0, 0);
    angle2_layout->addWidget(deg, 0, 1);

    press_layout->addWidget(press_line, 0, 0);
    press_layout->addWidget(MPascal, 0, 1);

    hum_layout->addWidget(hum_progress, 0, 0);

    spw_layout->addWidget(spw_line, 0, 0);
    spw_layout->addWidget(speed, 0, 1);

    db_layout->addWidget(b_RadioButton, 0, 0, 1, 2);
    db_layout->addWidget(d_RadioButton, 1, 0, 1, 2);
    db_layout->addWidget(db_line, 2, 0);
    db_layout->addWidget(lc, 2, 1);

    temp_layout->addWidget(temp_line, 0, 0);
    temp_layout->addWidget(deg, 0, 1);

    mot_layout->addWidget(onOff, 0, 0);

    principalscreen->setLayout(principal_layout);

    openCANPort();

    // Connexions
    connect(b_RadioButton, &QRadioButton::clicked, this, &MainWindow::db_button);
    connect(d_RadioButton, &QRadioButton::clicked, this, &MainWindow::db_button);
    connect(onOff, &QPushButton::clicked, this, &MainWindow::mot_button);

    // Créer un timer
    timer_tick = new QTimer();
    connect(timer_tick, &QTimer::timeout, this, &MainWindow::onTimer_Tick);
    //connect(but_send, &QPushButton::clicked, this, &MainWindow::onButSendClicked);

    timer_tick->start(1);


    // timer_tickSend = new QTimer();
    // connect( timer_tickSend, SIGNAL(timeout()), this, SLOT(onTimer_TickSend()));
    //connect(but_send, &QPushButton::clicked, this, &MainWindow::onButSendClicked);

    //timer_tickSend -> start(50);

}

/*
 ***********************************************************************************************/

MainWindow::~MainWindow()
{
    delete ui;
    delete hum_progress;
}

/*
 ***********************************************************************************************/


void MainWindow::mot_button()
{
    if (onOff->text() == "Off")
    {
        onOff->setText("On");
        onOff->setStyleSheet("QPushButton {background-color: #0099CC; }"); // Couleur pour "On"
    }
    else if (onOff->text() == "On")
    {
        onOff->setText("Off");
        onOff->setStyleSheet("QPushButton {background-color: #444444; }"); // Couleur pour "Off"
    }
}

/*
 ***********************************************************************************************/

void MainWindow::openCANPort()
{

    if (socket_can.open("can0") == scpp::STATUS_OK)
    {
         printf("can socket opened");
    }
    else
    {
        printf("Cannot open can socket!");
    }

}

/*
 ***********************************************************************************************/
void MainWindow::db_button()
{

    if(d_RadioButton->isChecked())
    {
        lc->setText("cm");
    }
    else if(b_RadioButton->isChecked())
    {
        lc->setText("lux");
    }

}

/*
 ***********************************************************************************************/

void MainWindow::onButSendClicked()
{
    sendCANMessage();
}

/*
 ***********************************************************************************************/


void MainWindow::sendCANMessage()
{

        scpp::CanFrame frame_to_write;

        if (onOff->text() == "On") {
            frame_to_write.id = 0x02;
            frame_to_write.len = 1;
            frame_to_write.data[0] = 0x03;
        } else if (onOff->text() == "Off") {
            frame_to_write.id = 0x02;
            frame_to_write.len = 1;
            frame_to_write.data[0] = 0x02;
        }

        socket_can.write(frame_to_write);

        if (d_RadioButton->isChecked()) {
            frame_to_write.id = 0x01;
            frame_to_write.len = 1;
            frame_to_write.data[0] = 0x02;
        } else if (b_RadioButton->isChecked()) {
            frame_to_write.id = 0x01;
            frame_to_write.len = 1;
            frame_to_write.data[0] = 0x03;
        }

        socket_can.write(frame_to_write);

        scpp::CanFrame frame_to_write1;
        frame_to_write1.id = 0x03;
        frame_to_write1.len = 1;
        frame_to_write1.data[0] = 0x1;

        socket_can.write(frame_to_write1);

}

/*
 ***********************************************************************************************/

void MainWindow::receiveCANMessage()
{
 scpp::CanFrame can_rx;

    if(socket_can.read(can_rx) == scpp::STATUS_OK)
    {
        if(can_rx.id==0x65){                                                            //   brightness data
            uint8_t lux = (can_rx.data[7]);
            if( b_RadioButton->isChecked()) db_line->setText(QString::number(lux));
        }

        if(can_rx.id==0x65){                                                            //   distance data
            uint8_t dis = can_rx.data[6];
            if ( d_RadioButton->isChecked() )  db_line->setText(QString::number(dis));
        }

        if(can_rx.id==0x65 ){                                             // pressure data
            uint16_t Pa = ( (can_rx.data[0] << 8) | can_rx.data[1] );
            press_line->setText(QString::number(Pa));
        }
        if(can_rx.id==0x65){                                             // temperature data
            uint16_t temp = ( (can_rx.data[4] << 8) | can_rx.data[5] );
            temp_line->setText(QString::number(temp));
        }
        if(can_rx.id==0x65){                                             // humidity data
            uint16_t Per = ( (can_rx.data[2] << 8) | can_rx.data[3] );
            updateHumidityProgress(int(Per));

        }

        if(can_rx.id==0x85){                                                            // wind speed data
            uint16_t Spw = ( can_rx.data[0] << 8  | can_rx.data[1] );
            spw_line->setText(QString::number(Spw));
        }

        if(can_rx.id==0x95){


            // Convertir les données en radians puis en degrés
            int16_t x = (can_rx.data[0] << 8 | can_rx.data[1]) ;
            int16_t y = (can_rx.data[2] << 8 | can_rx.data[3]) ;
            int16_t z = (can_rx.data[4] << 8 | can_rx.data[5]) ;

            int angle0 = static_cast<int>(x) / 100;
            int angle1 = static_cast<int>(y) / 100;
            int angle2 = static_cast<int>(z) / 100;

            // Afficher les angles dans les lignes correspondantes
            angle0_line->setText(QString::number(angle0));
            angle1_line->setText(QString::number(angle1));
            angle2_line->setText(QString::number(angle2));

            // Utiliser les angles dans l'objet OpenGL

            Object_GL->setAngles(angle0, angle1, angle2);
            Object_GL->updateGL();

        }

    }
}
/*
 ***********************************************************************************************/

void MainWindow::onTimer_Tick()
{
    receiveCANMessage();
    sendCANMessage();
    //Object_GL->updateGL();   // instable
}

/*
 ***********************************************************************************************/

void MainWindow::onTimer_TickSend()
{
    sendCANMessage();
}

/*
 * ************************************************************************************************/

void MainWindow::updateHumidityProgress(int humidity) {
    if (humidity < 0) humidity = 0;
    if (humidity > 100) humidity = 100;

    hum_progress->setValue(humidity);
}
