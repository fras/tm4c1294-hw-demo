# Hardware Demonstration Project for the TI Tiva TM4C1294 Connected LaunchPad

Auth: M. Fras, Electronics Division, MPI for Physics, Munich  
Mod.: M. Fras, Electronics Division, MPI for Physics, Munich  
Date: 07 Feb 2020  
Rev.: 24 Apr 2020  



## Prerequisites

### Software

* ARM GCC.
* GNU make.
* Firmware flashing tool lm4flash.
* Minicom terminal program.
* ARM GDB and nemiver graphical debugger.
* Python 3 and required modules.



### Hardware

* TI Tiva TM4C1294 Connected LaunchPad Evaluation Kit board.
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
  - Basic read/write from/to the I2C master ports 0 and 2 (BoosterPack 1 and
    2).
  - Functions to read information from the TMP006 temperature sensor and the
    OPT3001 ambient light sensor on the Educational BoosterPack MK II are
    implemented in the firmware.
* Synchronous Serial Interface (SSI) / SPI master:  
  Read/write from/to the SSI  ports 2 and 3 (BoosterPack 1 and 2).
* UART master:  
  Read/write from/to the UART port 6.
* Analog inputs:  
  Reading of the ADC values of the analog joystick and the accelerometer on the
  Educational BoosterPack MK II are implemented in the firmware.
* LCD on the Educational BoosterPack MKII:
  - Displays the firmware status and logo after boot.
  - Setup of the screen orientation is supported.
  - Basic functions for drawing pixels, lines, rectangles, circles and text are
    implemented.



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
    *******************************************************************************
    TIVA TM4C1294 `hw_demo' firmware version 0.3.3, release date: 24 Apr 2020
    *******************************************************************************
    
    Type `help' to get an overview of available commands.
    > help
    Available commands:
      help                                Show this help text.
      adc     [COUNT]                     Read ADC values.
      button  [INDEX]                     Get the status of the buttons.
      delay   MICROSECONDS                Delay execution.
      i2c     PORT SLV-ADR ACC NUM|DATA   I2C access (ACC bits: R/W, Sr, nP, Q).
      i2c-det PORT [MODE]                 I2C detect devices (MODE: 0 = auto,
                                              1 = quick command, 2 = read).
      illum   [COUNT]                     Read ambient light sensor info.
      info                                Show information about this firmware.
      lcd     CMD PARAMS                  LCD commands.
      led     [VALUE]                     Get/Set the value of the user LEDs.
      rgb     VALUE                       Set the RGB LED (RGB value = 0xRRGGBB).
      ssi     PORT R/W NUM|DATA           SSI/SPI access (R/W: 0 = write, 1 = read).
      ssi-set PORT FREQ [MODE] [WIDTH]    Set up the SSI port.
      temp    [COUNT]                     Read temperature sensor info.
      uart    PORT R/W NUM|DATA           UART access (R/W: 0 = write, 1 = read).
      uart-s  PORT BAUD [PARITY] [LOOP]   Set up the UART port.
    > info
    TIVA TM4C1294 `hw_demo' firmware version 0.3.3, release date: 24 Apr 2020
    It was compiled using gcc 6.3.1 20170620 at 21:41:14 on Apr 24 2020.
    > button
    OK. Button 3..0 status: 0x0
    > button
    OK. Button 3..0 status: 0xf
    > button 0
    OK. Button 0 status: 0x0, pressed count: 1, released count: 1
    > button 0
    OK. Button 0 status: 0x1, pressed count: 2, released count: 1
    > led 0xf
    OK: LEDs set to 0xf.
    > led
    OK: Current LED value: 0xf
    > i2c-det 2
    OK. I2C device(s) found at slave address: 0x40 0x44
    > i2c 2 0x40 0 0x2
    OK.
    > i2c 2 0x40 1 2
    OK. Data: 0x74 0x80
    > uart-s 6 11520 0 1
    OK.
    > uart 6 0 0x11 0x22 0x33
    OK.
    > uart 6 1
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



## Testing Hardware Features

Easily test some hardware features with the following instructions.



### ADC (Analog-to-Digital Converter)

Repeatedly read all ADC values 10,000 times:
```
> adc 10000
OK: Joystick: X = 1983 Y = 1878 ; Accelerometer: X = 2063 Y = 2067 Z = 2883
OK: Joystick: X = 1985 Y = 1867 ; Accelerometer: X = 2049 Y = 2072 Z = 2887
...
```

Watch the values change while moving the joystick and tilting/turning the whole
board.

Hint: You can stop the test by pressing the ```RESET``` button on the board.



### I2C (Inter-Integrated Circuit) Bus

Automatically detect all devices on I2C port 2:
```
> i2c-det 2
OK. I2C device(s) found at slave address: 0x40 0x44
>
```

Preform a raw access the I2C temperature sensor on the Educational BoosterPack
MK II. It has the I2C slave address ```0x40```. Set the pointer to the
temperature register and read the raw temperature values:

```
> i2c 2 0x40 0 0x01
OK.
> i2c 2 0x40 1 2
OK. Data: 0x0e 0x14
>
```



### SSI (Synchronous Serial Interface) / SPI (Serial Peripheral Interface) Bus

Note: The slave select pin ```PD2``` of the SSI port 2 on BoosterPack 1 is used
as analog input on BoosterPack 2 (joystick X on the Educational BoosterPack MK
II)! Thus it is disabled in the firmware by default.

Note: The SSI port 3 on BoosterPack 2 is used by the LCD on the Educational
BoosterPack MK II!

Since firmware version 0.2.0, you can set the clock frequency, the SSI mode and
the data width with the command ```ssi-set```.  
Example for an SSI clock of 1 MHz, SPI mode 0 and a data width of 8 on SSI port
2:
```
ssi-set 2 1000000 0 8
```

To test the SSI port 2 on BoosterPack 1 in loopback mode, remove the jumper
across pins 5 and 6 of the BOOSTXL-RS232 board and place it across the pins 14
and 15. These are the pins ```PD0``` and ```PD1``` on the TM4C1294 Connected
LaunchPad Evaluation Kit.

Then send a few bytes and read them back. E.g.
```
> ssi 2 0 0x11 0x22 0x33 0x44 0x55 0xaa
OK.
> ssi 2 1
OK. Data: 0x11 0x22 0x33 0x44 0x55 0xaa
>
```

Please note that the hardware FIFO for buffering SSI data is only 8 entries
long. This limits the maximum number of data for this test to 8.



### Universal Asynchronous Receiver Transmitter (UART)

To test the UART 6 on BoosterPack 2, you can set the loopback mode in the
configuration of the UART in the file ```hw_demo.c```.
```c
sUartBoosterPack2.bLoopback = true;
```

Since firmware version 0.2.0, you can set the baud rate, the parity and the
loopback mode with the command ```uart-s```.  
Example for a baud rate of 11520, no parity and internal loopback mode on UART
port 6:
```
uart-s 6 11520 0 1
```

Alternatively, put a jumper across the pins ```PP0``` and ```PP1``` on the
TM4C1294 Connected LaunchPad Evaluation Kit.

Then send a few bytes and read them back. E.g.
```
> uart 6 0 0x11 0x22 0x33 0x44 0x55 0xaa
OK.
> uart 6 1
OK. Data: 0x11 0x22 0x33 0x44 0x55 0xaa
>
```



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

![TM4C1294NCPDT IoT enabled High performance 32-bit ARM® Cortex®-M4F based MCU](http://www.ti.com/graphics/folders/partimages/TM4C1294NCPDT.jpg)
* [TM4C1294NCPDT product page](http://www.ti.com/product/TM4C1294NCPDT)
* [Tiva C Series TM4C1294NCPDT Microcontroller Data Sheet datasheet (Rev. B)](http://www.ti.com/lit/ds/symlink/tm4c1294ncpdt.pdf)
* [TivaWare™ Peripheral Driver Library for C Series User's Guide (Rev. D)](http://www.ti.com/lit/ug/spmu298d/spmu298d.pdf)
* [TivaWare™ Graphics Library User's Guide](http://www.ti.com/lit/ug/spmu300d/spmu300d.pdf)



### Getting Started, HOWTOs, Tutorials

[![Tiva C Series Connected Launchpad Getting Started EK-TM4C1294XL](https://img.youtube.com/vi/JpGNNCYjtFw/1.jpg)](https://www.youtube.com/watch?v=JpGNNCYjtFw "Tiva C Series Connected Launchpad Getting Started EK-TM4C1294XL")

* [HowTo: Develop on the TI Tiva LaunchPad using Linux](http://chrisrm.com/howto-develop-on-the-ti-tiva-launchpad-using-linux/)
* [Tiva TM4C123G LaunchPad Blink the RGB](https://processors.wiki.ti.com/index.php/Tiva_TM4C123G_LaunchPad_Blink_the_RGB)
* [Tiva Tutorials - Tiva Peripherals with TivaWare](https://sites.google.com/site/luiselectronicprojects/tutorials/tiva-tutorials)
* [Getting Started with the TI Stellaris LaunchPad on Linux](https://www.jann.cc/2012/12/11/getting_started_with_the_ti_stellaris_launchpad_on_linux.html)

