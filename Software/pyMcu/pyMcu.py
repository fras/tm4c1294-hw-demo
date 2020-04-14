#!/usr/bin/env python3
#
# File: pyMcu.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 31 Mar 2020
# Rev.: 14 Apr 2020
#
# Python front-end for accessing the TM4C1294NCPDT MCU on the TM4C1294
# Connected LaunchPad Evaluation Kit over a serial port (UART).
#



# Append hardware classes folder to python path.
import sys
import os
sys.path.append(os.path.dirname(__file__) + '/hw')

# MCU test script.
import pyMcuTest

# MCU GUI
import pyMcuGui



# Parse command line arguments.
import argparse
parser = argparse.ArgumentParser(description='Access to Process some integers.')
parser.add_argument('-d', '--device', action='store', type=str,
                    dest='serial_device', default='/dev/ttyUSB0',
                    help='Serial device to access the MCU.')
parser.add_argument('-g', '--gui', action='store_true',
                    dest='launch_gui', default=False,
                    help='Launch the hardware test GUI.')
parser.add_argument('-t', '--test', action='store_true',
                    dest='run_test', default=False,
                    help='Run a hardware feature test.')
args = parser.parse_args()



# Run the hardware test.
if args.run_test:
    pyMcuTest.run_test(args.serial_device)
elif args.launch_gui:
    pyMcuGui.launch_gui(args.serial_device)
else:
    print("Please use one of these options:")
    print("   --test")
    print("   --gui")

