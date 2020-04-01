Hardware Demonstration Project for the TI Tiva TM4C1294 Connected LaunchPad
===========================================================================
Auth: M. Fras, Electronics Division, MPI for Physics, Munich
Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
Date: 07 Feb 2020
Rev.: 30 Mar 2020



Requirements
============
* ARM gcc.
* GNU make.
* TI Tiva TM4C1294 Connected LaunchPad evaluation board.
* BOOSTXL-RS232 board (connected to the BoosterPack 1 socket).
* Educational BoosterPack MK II (connected to the BoosterPack 2 socket).



Firmware Features
=================
* Simple UART user interface on UART 7:
  Settings:
  - baud rate = 115200
  - data format = 8N1
  Simply connect the BOOSTXL-RS232 board to the BoosterPack 1 socket of the TI
  Tiva TM4C1294 Connected LaunchPad. Connect its 9-pin D-sub connetor to a PC
  and open a terminal software using the above settings.
* GPIO LEDs:
  Control the 4 LEDs on the Tiva TM4C1294 Connected LaunchPad board using GPIO.
* GPIO buttons:
  Receive messages on the console when one of the 2 buttons on the Tiva
  TM4C1294 Connected LaunchPad or on the Educational BoosterPack MK is pressed
  or released.
* PWM - RGB LED:
  Control the RGB LED on the Educational BoosterPack MK II using PWM.
* I2C master:
  Read/write from/to the I2C master port 2.
  Functions to read information from the TMP006 temperature sensor and the
  OPT3001 ambient light sensor on the Educational BoosterPack MK II are
  implemented in the firmware.
* SPI master:
  TODO
* UART master:
  Read/write from/to the UART port 6.
* Analog inputs:
  Reading of the ADC values of the analog joystick and the accelerometer on the
  Educational BoosterPack MK II are implemented in the firmware.

