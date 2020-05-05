#!/usr/bin/env python3
#
# File: pyMcuBatch.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 20 Apr 2020
# Rev.: 05 May 2020
#
# Python script to load and execute a batch of MCU commands from a file on the
# TM4C1294NCPDT MCU on the TM4C1294 Connected LaunchPad Evaluation Kit over a
# serial port (UART).
#



# Append hardware classes folder to Python path.
import os
import sys
sys.path.append(os.path.relpath(os.path.join(os.path.dirname(__file__), 'hw')))



# System modules.
import time



# Hardware classes.
import McuSerial



# Message prefixes and separators.
prefixWarning           = "WARNING: {0:s}: ".format(__file__)
prefixError             = "ERROR: {0:s}: ".format(__file__)
prefixDebug             = "DEBUG: {0:s}: ".format(__file__)

# Command batch file parameters.
batchFileMarkComment    = "#"
batchFileCmdDelay       = "delay"
batchFileCmdExit        = "exit"

# MCU-specific variables and parameters.
mcuCmdPrompt            = "> "
mcuCmdDefHelp           = "help"
mcuCmdDefInfo           = "info"
mcuCmdDefLcd            = "lcd"
mcuCmdDefLcdHelp        = "help"
mcuCmdDefLcdText        = "text"



# Load and execute an MCU command batch.
def exec_batch(serialDevice, batchFileName, stopOnError, verbosity):
    # Check if batchFileName is a file.
    if not os.path.isfile(batchFileName):
        print(prefixError + "The MCU command batch file parameter `{0:s}' is not a file!".format(batchFileName))
        return 1
    # Check if the MCU command batch file is readable.
    if not os.access(batchFileName, os.R_OK):
        print(prefixError + "Cannot open the MCU command batch file `{0:s}'!".format(batchFileName))
        return 1

    # Inititalize statistics counters.
    batchFileLineCount = 0
    internalCmdCount = 0
    mcuCmdCount = 0
    okCount = 0
    noEvalCount = 0
    warningCount = 0
    errorCount = 0

    # Open the MCU serial interface.
    mcuSer = McuSerial.McuSerial(serialDevice)
    mcuSer.debugLevel = 0
    mcuSer.clear()
    # Increase the maximum number of lines and the timeout value.
    mcuSer.ser.mcuReadLineMax = 200
    mcuSer.ser.timeout = 0.05

    # Read and process the batch command file.
    with open(batchFileName) as fileBatch:
        for fileBatchLine in fileBatch:
            batchFileLineCount += 1
            if verbosity >= 4:
                print(prefixDebug + "Processing line {0:d} of command batch file `{1:s}':".\
                    format(batchFileLineCount, batchFileName))
                print(prefixDebug + fileBatchLine.strip('\n\r'))
            # Strip all leading and trailing white spaces, tabs, line feeds and carriage returns.
            lineStripped = fileBatchLine.strip(' \t\n\r')
            # Remove comments.
            if lineStripped.find(batchFileMarkComment) >= 0:
                lineCommentRemoved = lineStripped[0:lineStripped.find(batchFileMarkComment)].strip(' \t')
                comment = lineStripped[lineStripped.find(batchFileMarkComment) + len(batchFileMarkComment):]
            else:
                lineCommentRemoved = lineStripped
                comment = ""
            # Get list of elements.
            lineElements = list(filter(None, lineCommentRemoved.replace("\t", " ").split(" ")))
            # Ignore empty lines.
            if not lineElements:
                continue
            mcuCmd = lineElements[0]
            if len(lineElements) >= 2:
                mcuCmdArg0 = lineElements[1]
            else:
                mcuCmdArg0 = ""
            # Check if the current command is a delay command.
            if mcuCmd == batchFileCmdDelay:
                if len(lineElements) < 2:
                    errorCount += 1
                    print(prefixError + "Line {0:d}: Delay value required after command `{1:s}'!".format(batchFileLineCount, mcuCmd))
                    if stopOnError:
                        break
                    continue
                else:
                    internalCmdCount += 1
                    delayVal = int(lineElements[1], 0)
                    time.sleep(delayVal / 1e6)
                    continue
            # Check if the current command 'exit'.
            if mcuCmd == batchFileCmdExit:
                internalCmdCount += 1
                if verbosity >= 2:
                    print("\nExecution of file `{0:s}' stopped after command `{1:s}' in line {2:d}.".\
                        format(batchFileName, batchFileCmdExit, batchFileLineCount))
                break
            mcuCmdCount += 1
            # Assign MCU command.
            mcuCmdStr = lineCommentRemoved
            # Exception: Ignore the comment mark for the LCD text command, to
            #            be able to print the comment mark on the LCD.
            if mcuCmd == mcuCmdDefLcd and mcuCmdArg0 == mcuCmdDefLcdText:
                mcuCmdStr = lineStripped
                comment = ""
            # Don't evaluate the MCU response for the following commands.
            if ((mcuCmd == mcuCmdDefHelp) or
                (mcuCmd == mcuCmdDefInfo) or
                (mcuCmd == mcuCmdDefLcd and mcuCmdArg0 == mcuCmdDefLcdHelp)):
                noEvalCount += 1
                evaluateResponse = False
            else:
                evaluateResponse = True
            if verbosity >= 3:
                print(prefixDebug + "Command batch file `{0:s}' line {1:d}:\nMCU command `{2:s}', comment: `{3:s}'".\
                    format(batchFileName, batchFileLineCount, mcuCmdStr, comment))
            # Debug message,
            if verbosity >= 2:
                print(mcuCmdPrompt + mcuCmdStr)
            # Execute MCU command.
            ret = mcuSer.send(mcuCmdStr)
            if ret:
                errorCount += 1
                print(prefixError + "Line {0:d}: Error sending command to MCU! Error code: {1:d}".format(batchFileLineCount, ret), end='')
                print(" Command batch file `{0:s}' line {1:d}: MCU command `{2:s}'".\
                    format(batchFileName, batchFileLineCount, mcuCmdStr))
                if stopOnError:
                    print("\n*** ", end='')
                    print(prefixError + "Execution of file `{0:s}' stopped after error in line {1:d}.".format(batchFileName, batchFileLineCount))
                    break
                continue
            # Debug message,
            if verbosity >= 2:
                print(mcuSer.get_full())
            # Evaluate response.
            if evaluateResponse:
                ret = mcuSer.eval()
                if ret == mcuSer.mcuResponseCodeOk:
                    okCount += 1
                elif ret == mcuSer.mcuResponseCodeWarning:
                    warningCount += 1
                    if verbosity >= 1:
                        print(prefixWarning + "Line {0:d}: Warning during execution of MCU command.".format(batchFileLineCount), end='')
                        print(" Command batch file `{0:s}' line {1:d}: MCU command `{2:s}'".\
                            format(batchFileName, batchFileLineCount, mcuCmdStr))
                else:
                    errorCount += 1
                    print(prefixError + "Line {0:d}: Error executing MCU command! Error code: {1:d}".format(batchFileLineCount, ret), end='')
                    print(" Command batch file `{0:s}' line {1:d}: MCU command `{2:s}'".\
                        format(batchFileName, batchFileLineCount, mcuCmdStr))
                    if stopOnError:
                        print("\n*** ", end='')
                        print(prefixError + "Execution of file `{0:s}' stopped after error in line {1:d}.".format(batchFileName, batchFileLineCount))
                        break
    if verbosity >= 2:
        print()
    if verbosity >= 1:
        print("Lines processed:             {0:d}".format(batchFileLineCount))
        print("Interntal commands:          {0:d}".format(internalCmdCount))
        print("MCU commands executed:       {0:d}".format(mcuCmdCount))
        print("MCU commands OK:             {0:d}".format(okCount))
        print("MCU commands w/o evaluation: {0:d}".format(noEvalCount))
        print("MCU command warnings:        {0:d}".format(warningCount))
        print("MCU command errors:          {0:d}".format(errorCount))
    if errorCount > 0:
        return -1
    return 0



# Load and execute an MCU command batch file.
if __name__ == "__main__":
    # Command line arguments.
    import argparse
    parser = argparse.ArgumentParser(description='Execute an MCU command batch file.')
    parser.add_argument('-d', '--device', action='store', type=str,
                        dest='serialDevice', default='/dev/ttyUSB0',
                        help='Serial device to access the MCU.')
    parser.add_argument('-f', '--file', action='store', type=str,
                        dest='batchFileName', default='mcu.cmd', required=True,
                        help='Batch file containing MCU commands.')
    parser.add_argument('-s', '--stop-on-error', action='store_true',
                        dest='stopOnError', default=False,
                        help='Serial device to access the MCU.')
    parser.add_argument('-v', '--verbosity', action='store', type=int,
                        dest='verbosity', default="1", choices=range(0, 5),
                        help='Set the verbosity level. The default is 1.')
    args = parser.parse_args()

    # Load and execute the MCU command batch file.
    exit(exec_batch(args.serialDevice, args.batchFileName, args.stopOnError, args.verbosity))

