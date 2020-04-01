#!/bin/sh
#
# File: debug_nemiver.sh
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 20 Nov 2019
# Rev.: 21 Nov 2019
#
# Script to launch the Nemiver debugger to debug code running on the TI Tiva
# TM4C1294 connected LaunchPad evaluation kit.
#
# This script is based on the example on this webpage:
# https://www.jann.cc/2012/12/11/getting_started_with_the_ti_stellaris_launchpad_on_linux.html
#



# Change to script directory.
cd `dirname $0`

# Check command line arguments.
if [ $# -lt 1 ]; then
    echo "Usage: `basename $0` <project>"
    exit 1
fi

# Check if required AXF file exists.
AXF_FILE="../gcc/$1.axf"
if [ ! -r ${AXF_FILE} ]; then
    echo "ERROR: ARM executable format file '${AXF_FILE}' not found."
    exit 1
fi

# Location of OpenOCD Board .cfg files.
OPENOCD_BOARD_DIR=/usr/share/openocd/scripts/board

# Start xterm with openocd in the background.
xterm -e openocd -f $OPENOCD_BOARD_DIR/ek-tm4c1294xl.cfg &

# Save the PID of the background process.
XTERM_PID=$!

# Wait a bit to be sure the hardware is ready.
sleep 2

# Execute some initialisation commands via gdb.
#arm-none-eabi-gdb --batch --command=init.gdb ${AXF_FILE}
gdb-multiarch --batch --command=init.gdb ${AXF_FILE}

# Start the gdb gui.
#nemiver --remote=localhost:3333 --gdb-binary="$(which arm-none-eabi-gdb)" ${AXF_FILE}
nemiver --remote=localhost:3333 --gdb-binary="$(which gdb-multiarch)" ${AXF_FILE}

# Close xterm when the user has exited nemiver.
kill $XTERM_PID
