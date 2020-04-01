# File: init.gdb
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 20 Nov 2019
# Rev.: 21 Nov 2019
#
# GBD commands used by the Nemiver debugger to debug code running on the TI
# Tiva TM4C1294 connected LaunchPad evaluation kit using GDB.
#
# This script is based on the example on this webpage:
# https://www.jann.cc/2012/12/11/getting_started_with_the_ti_stellaris_launchpad_on_linux.html
#



# Specify remote target.
target extended-remote :3333

# Reset to known state.
monitor reset halt
load
monitor reset init

# Set a breakpoint at main().
break main

# Run to the breakpoint.
continue

