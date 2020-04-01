# Hardware Demonstration Project for the TI Tiva TM4C1294 Connected LaunchPad

Auth: M. Fras, Electronics Division, MPI for Physics, Munich  
Mod.: M. Fras, Electronics Division, MPI for Physics, Munich  
Date: 07 Feb 2020  
Rev.: 01 Apr 2020  



## Prerequisites

* ARM GCC.
* GNU make.
* Firmware flashing tool lm4flash.
* Minicom terminal program.
* ARM GDB and nemiver graphical debugger.
* Python 3 and required modules.
* TI Tiva TM4C1294 Connected LaunchPad evaluation board.
* BOOSTXL-RS232 board (connected to the BoosterPack 1 socket).
* Educational BoosterPack MK II (connected to the BoosterPack 2 socket).



## Firmware Features

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



## Getting Started

1. Install required packages. Example for Ubuntu 18.04.  
    ARM cross compiler toolchain.
    ```console
    sudo apt-get install build-essential gcc-arm-none-eabi binutils-arm-none-eabi openocd
    ```
    Firmware flashing tool.
    ```console
    sudo apt-get install lm4flash
    ```
    Minicom terminal program.
    ```console
    sudo apt-get install minicom
    ```
    ARM GDB and nemiver graphical debugger.
    ```console
    sudo apt-get install gdb-multiarch nemiver
    ```
    Python 3 and required modules.
    ```console
    sudo apt-get install python3 python3-serial python3-tk
    ```

2. Compile the firmware project.  
    Change to the ```Firmware``` directory. Then clean the firmware project
    directory. This will remove all compiled files and backups of source files.
    ```console
    make mrproper
    ```
    Build the firmware project.
    ```console
    make
    ```
    Download the firmware. Make sure that the TM4C1294 Connected LaunchPad™
    Evaluation Kit is connected to an USB port of the PC.
    ```console
    make install
    ```

3. Communicate with the MCU using the minicom terminal program.  
    Create a file ```.minirc.hw_demo``` in your home directory with this
    content:
    ```
    pu port             /dev/ttyUSB0
    pu rtscts           No
    ```
    Launch minicom either by calling ```make minicom``` inside the firmware
    folder or by starting minicom from the shell ```minicom hw_demo```. To quit
    minicom, press ```Ctrl-A```, then ```Q```. To edit the minicom settings,
    press ```Ctrl-A```, then ```Z```.

    Example minicom session:
    ```console
    **********************************************************************
    TIVA TM4C1294 hw_demo firmware version 0.0.7, release date: 30 Mar 2020
    **********************************************************************
    
    Type `help' to get an overview of available commands.
    > help
    Available commands:
      help                                Show this help text.
      adc     [COUNT]                     Read ADC values.
      i2c     PORT SLV-ADR R/W NUM|DATA   I2C access (0 = write, 1 = read).
      illum   [COUNT]                     Read ambient light sensor info.
      info                                Show information about this firmware.
      led     VALUE                       Set the LEDs.
      rgb     VALUE                       Set the RGB LED.
      temp    [COUNT]                     Read temperature sensor info.
      uart    PORT R/W NUM|DATA           UART access (0 = write, 1 = read).
    > led 0xf
    OK: LEDs set to 0x0f.
    > temp 2
    OK: Temperature = 27.25000 *C ; Manufacturer ID = 0x5449 ; Device ID = 0x0067
    OK: Temperature = 27.25000 *C ; Manufacturer ID = 0x5449 ; Device ID = 0x0067
    >
    ```

4. Control the MCU using the Python scripts.  
    Change to the ```Software/pyMcu``` directory. Then type ```./pyMcu.py
    --test``` to run a pre-defined automated test procedure from the command
    line or ```./pyMcu.py --gui``` to open a GUI. You can specify a custom
    serial device to which the MCU is attached using the ```--device``` option,
    e.g. ```./pyMcu.py --device /dev/ttyUSB0 --gui```.

