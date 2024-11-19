# Specifications and Progress

## Evaluation Criteria

* **1 point** - Each board is associated with a unique CAN identifier.
* **3 points** - The data from all sensors is retrieved and displayed simultaneously on the host PC's GUI (to be developed in C++), at least in text form.
* **1 point** - Exchanges between the GUI and the boards are performed periodically.
* **2 points** - Use of widgets to display sensor data (pressure, luminosity, distance, wind speed).
* **2 points** - The orientation of the IMU board is graphically represented in the GUI using OpenGL.
* **2 points** - Implementation of the Madgwick algorithm (or equivalent).
* **3 points** - Pressure, temperature, and humidity data are retrieved and processed.
* **2 points** - The motor is controllable from the GUI.
* **1 point** - The motor starts when the wind speed exceeds a certain threshold.
* **3 points** - Switching between luminosity and distance measurement directly from the GUI.

---

## STM32 Tasks (Completed)

- [X] Acquisition of luminosity and distance data (via CAN bus).
- [X] Acquisition of pressure data.
- [X] Acquisition of wind speed data.
- [X] Acquisition of humidity data.
- [X] Acquisition of accelerometer data.

---

## GUI Tasks

- [X] Display of luminosity and distance values on the interface.
- [X] Switching between luminosity and distance in the interface.
- [X] Display of pressure data (**using widgets**).
- [X] Display of wind speed data (**using widgets**).
- [X] Display of humidity data (**using widgets**).
- [X] Display of the IMU board's orientation (**via OpenGL in the GUI**).

