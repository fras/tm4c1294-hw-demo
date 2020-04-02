# Hardware Demonstration Project for the TI Tiva TM4C1294 Connected LaunchPad

Auth: M. Fras, Electronics Division, MPI for Physics, Munich  
Mod.: M. Fras, Electronics Division, MPI for Physics, Munich  
Date: 07 Feb 2020  
Rev.: 02 Apr 2020  



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
    ```shell
    sudo apt-get install build-essential gcc-arm-none-eabi binutils-arm-none-eabi openocd
    ```
    Firmware flashing tool.
    ```shell
    sudo apt-get install lm4flash
    ```
    Minicom terminal program.
    ```shell
    sudo apt-get install minicom
    ```
    ARM GDB and nemiver graphical debugger.
    ```shell
    sudo apt-get install gdb-multiarch nemiver
    ```
    Python 3 and required modules.
    ```shell
    sudo apt-get install python3 python3-serial python3-tk
    ```

2. Compile and download the firmware project.  
    Change to the ```Firmware``` directory. Then clean the firmware project
    directory.
    ```shell
    make clean
    ```
    This will wipe all compiled files and backups of source files from the
    project.
    ```shell
    make mrproper
    ```
    Build the firmware project.
    ```shell
    make
    ```
    Download the firmware. Make sure that the TM4C1294 Connected LaunchPad™
    Evaluation Kit is connected to an USB port of the PC.
    ```shell
    make install
    ```
    Compile and download a debug version of the firmware, then start the
    nemiver graphical debugger. Please note that there is a breakpoint set at
    the start of the program! This prevents it from running until the program
    is continued from the debugger.
    ```shell
    make debug
    ```
    After you have finished debugging, build and download the normal firmware
    version again.
    ```shell
    make clean install
    ```

3. Communicate with the MCU using the minicom terminal program.  
    Create a file ```.minirc.hw_demo``` in your home directory with this
    content:
    ```
    pu port             /dev/ttyUSB0
    pu rtscts           No
    ```
    Adapt the ```pu port``` to the serial input to which your TM4C1294
    Connected LaunchPad™ Evaluation Kitis connected.

    Launch minicom either by calling ```make minicom``` inside the firmware
    folder or by starting minicom from the shell ```minicom hw_demo```. To quit
    minicom, press ```Ctrl-A```, then ```Q```. To edit the minicom settings,
    press ```Ctrl-A```, then ```Z```.

    Example minicom session:
    ```
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
    > info
    TIVA TM4C1294 hw_demo firmware version 0.0.7, release date: 30 Mar 2020
    It was compiled using gcc 6.3.1 20170620 at 13:56:27 on Apr  2 2020.
    > led 0xf
    OK: LEDs set to 0x0f.
    > i2c 2 0x40 0 0x2
    OK.
    > i2c 2 0x40 1 2
    OK. Data: 0x74 0x80
    > uart 6 0 0x11 0x22 0x33
    OK.
    > uart 6 1 3
    OK. Data: 0x11 0x22 0x33
    > adc 2
    OK: Joystick: X = 1919 Y = 1892 ; Accelerometer: X = 1977 Y = 2074 Z = 2867
    OK: Joystick: X = 1929 Y = 1896 ; Accelerometer: X = 1983 Y = 2072 Z = 2871
    > temp
    OK: Temperature = 27.25000 *C ; Manufacturer ID = 0x5449 ; Device ID = 0x0067
    >
    ```

4. Control the MCU using the Python scripts.  
    Change to the ```Software/pyMcu``` directory. Then type ```./pyMcu.py
    --test``` to run a pre-defined automated test procedure from the command
    line or ```./pyMcu.py --gui``` to open a GUI. You can specify a custom
    serial device to which the MCU is attached using the ```--device``` option,
    e.g. ```./pyMcu.py --device /dev/ttyUSB0 --gui```.



## Literature

### ARM® Cortex®-M4F-Based MCU TM4C1294 Connected LaunchPad™ Evaluation Kit

![EK-TM4C1294XL TM4C1294 Connected LaunchPad Board Image](http://www.ti.com/diagrams/ek-tm4c1294xl\_tm4c1294\_connected\_launchpad\_top\_view.jpg)
* [ARM® Cortex®-M4F-Based MCU TM4C1294 Connected LaunchPad™ Evaluation Kit](http://www.ti.com/tool/EK-TM4C1294XL)
* [Meet the Tiva™ C Series TM4C1294 Connected LaunchPad Evaluation Kit](http://www.ti.com/lit/ml/spmz858/spmz858.pdf)
* [Tiva™ C Series TM4C1294 Connected LaunchPad Evaluation Kit User's Guide](http://www.ti.com/lit/ug/spmu365c/spmu365c.pdf)



### BoosterPacks

#### TRS3122E: RS-232 transceiver BoosterPack™ Plug-in Module

![TRS3122E: RS-232 transceiver BoosterPack™ Plug-in Module](http://www.ti.com/diagrams/med_boostxl-rs232_boostxl-rs232_-_ti_11_15_2016--22.JPG)
* [TRS3122E: RS-232 transceiver BoosterPack™ Plug-in Module](http://www.ti.com/tool/BOOSTXL-RS232)
* [BOOSTXL-RS232 BoosterPack™ Hardware User's Guide](http://www.ti.com/lit/ug/sllu250/sllu250.pdf)

#### Educational BoosterPack MKII

![BOOSTXL-EDUMKII - Educational BoosterPack MKII](http://www.ti.com/diagrams/med_boostxl-edumkii_boostxl-edumkii_frontnewresize.jpg)
* [Educational BoosterPack MKII](http://www.ti.com/tool/BOOSTXL-EDUMKII)
* [ BOOSTXL-EDUMKII Quick Start Guide](http://www.ti.com/lit/ml/slau600/slau600.pdf)
* [BOOSTXL-EDUMKII Educational BoosterPack Plug-in Module Mark II User's Guide (Rev. A)](http://www.ti.com/lit/ug/slau599a/slau599a.pdf)



### TM4C1294NCPDT IoT enabled High performance 32-bit ARM® Cortex®-M4F based MCU 

![IoT enabled High performance 32-bit ARM® Cortex®-M4F based MCU](http://www.ti.com/graphics/folders/partimages/TM4C1294NCPDT.jpg)
* [TM4C1294NCPDT product page](http://www.ti.com/product/TM4C1294NCPDT)
* [Tiva C Series TM4C1294NCPDT Microcontroller Data Sheet datasheet (Rev. B)](http://www.ti.com/lit/ds/symlink/tm4c1294ncpdt.pdf)
* [TivaWare™ Peripheral Driver Library for C Series User's Guide (Rev. D)](http://www.ti.com/lit/ug/spmu298d/spmu298d.pdf)



### Getting Started, HOWTOs, Tutorials

<iframe width="560" height="315" src="https://www.youtube.com/embed/JpGNNCYjtFw" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

* [HowTo: Develop on the TI Tiva LaunchPad using Linux](http://chrisrm.com/howto-develop-on-the-ti-tiva-launchpad-using-linux/)
* [Tiva TM4C123G LaunchPad Blink the RGB](https://processors.wiki.ti.com/index.php/Tiva_TM4C123G_LaunchPad_Blink_the_RGB)
* [Tiva Tutorials - Tiva Peripherals with TivaWare](https://sites.google.com/site/luiselectronicprojects/tutorials/tiva-tutorials)
* [Getting Started with the TI Stellaris LaunchPad on Linux](https://www.jann.cc/2012/12/11/getting_started_with_the_ti_stellaris_launchpad_on_linux.html)

