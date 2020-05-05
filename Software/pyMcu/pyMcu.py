#!/usr/bin/env python3
#
# File: pyMcu.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 31 Mar 2020
# Rev.: 05 May 2020
#
# Python front-end for accessing the TM4C1294NCPDT MCU on the TM4C1294
# Connected LaunchPad Evaluation Kit over a serial port (UART).
#



# Append hardware classes folder to Python path.
import os
import sys
sys.path.append(os.path.relpath(os.path.join(os.path.dirname(__file__), 'hw')))



# Module for the MCU command batch file execution.
import pyMcuBatch

# MCU GUI.
import pyMcuGui

# MCU test script.
import pyMcuTest



# Parse command line arguments.
import argparse
parser = argparse.ArgumentParser(description='Front-end software for the MCU.')
parser.add_argument('-b', '--batch', action='store', type=str,
                    dest='batchFileName', default='',
                    help='Execute an MCU command batch file.')
parser.add_argument('-d', '--device', action='store', type=str,
                    dest='serialDevice', default='/dev/ttyUSB0',
                    help='Serial device to access the MCU.')
parser.add_argument('-g', '--gui', action='store_true',
                    dest='launchGui', default=False,
                    help='Launch the hardware test GUI.')
parser.add_argument('-t', '--test', action='store_true',
                    dest='runTest', default=False,
                    help='Run a hardware feature test.')
parser.add_argument('-v', '--verbosity', action='store', type=int,
                    dest='verbosity', default="1", choices=range(0, 5),
                    help='Set the verbosity level. The default is 1.')
args = parser.parse_args()



# Load and execute an MCU command batch file.
if args.batchFileName:
    stopOnError = True
    pyMcuBatch.exec_batch(args.serialDevice, args.batchFileName, stopOnError, args.verbosity)
# Launch the GUI.
elif args.launchGui:
    pyMcuGui.launch_gui(args.serialDevice, args.verbosity)
# Run the hardware test.
elif args.runTest:
    pyMcuTest.run_test(args.serialDevice, args.verbosity)
else:
    print("Please use one of these options:")
    print("   --batch BATCHFILENAME")
    print("   --gui")
    print("   --test")

