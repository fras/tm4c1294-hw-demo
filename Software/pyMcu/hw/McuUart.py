# File: McuUart.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 31 Mar 2020
# Rev.: 21 Apr 2020
#
# Python class for using the UART ports of the TM4C1294NCPDT MCU.
#



import McuSerial



class McuUart:

    # Message prefixes and separators.
    prefixDetails       = " - "
    separatorDetails    = " - "
    prefixError         = "ERROR: {0:s}: ".format(__file__)
    prefixDebug         = "DEBUG: {0:s}: ".format(__file__)

    # Debug configuration.
    debugLevel = 0                 # Debug verbosity.

    # Hardware parameters.
    hwBaudMin           = 150
    hwBaudMax           = 15000000
    hwParity            = ['none', 'even', 'odd', 'one', 'zero']
    hwMarkData          = "Data:"



    # Initialize the UART port of the MCU.
    def __init__(self, mcuSer, port):
        self.port = port
        self.mcuSer = mcuSer
        self.errorCount = 0
        self.accessRead = 0
        self.accessWrite = 0
        self.bytesRead = 0
        self.bytesWritten = 0



    # Send an UART command to the MCU.
    def send_cmd(self, cmd):
        # Debug: Show command.
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Sending command to the UART port {0:d}: ".format(self.port) + cmd)
        # Send command.
        self.mcuSer.send(cmd)
        # Debug: Show response.
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Response from MCU:")
            print(self.mcuSer.get_full())
        # Evaluate response.
        ret = self.mcuSer.eval()
        if ret:
            self.errorCount += 1
            print(self.prefixError + "Error sending command to the UART port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "Command sent to MCU: " + cmd)
                print(self.prefixError + "Response from MCU:")
                print(self.mcuSer.get_full())
            return ret
        return 0



    # Print details.
    def print_details(self):
        print(self.prefixDetails, end='')
        print("UART port: {0:d}".format(self.port), end='')
        if self.debugLevel >= 1:
            print(self.separatorDetails + "Error count: {0:d}".format(self.errorCount), end='')
        if self.debugLevel >= 1:
            print(self.separatorDetails + "Read access count: {0:d}".format(self.accessRead), end='')
            print(self.separatorDetails + "Write access countn: {0:d}".format(self.accessWrite), end='')
        if self.debugLevel >= 1:
            print(self.separatorDetails + "Bytes read: {0:d}".format(self.bytesRead), end='')
            print(self.separatorDetails + "Bytes written: {0:d}".format(self.bytesWritten), end='')
        print()
        return 0



    # Setup the UART port.
    def setup(self, baud, parity, loopback):
        if baud < self.hwBaudMin or baud > self.hwBaudMax:
            self.errorCount += 1
            print(self.prefixError + "UART baud rate {0:d} outside of valid range {1:d}..{2:d}!".format(baud, self.hwBaudMin, self.hwBaudMax))
            return -1
        if parity < 0 or parity > len(self.hwParity) - 1:
            self.errorCount += 1
            print(self.prefixError + "UART parity {0:d} outside of valid range {1:d}..{2:d}!".format(parity, 0, len(self.hwParity)-1))
            return -1
        cmd = "uart-s {0:d} {1:d} {2:d} {3:d}".format(self.port, baud, parity, loopback & 0x1)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Setting up the UART port {0:d}.".format(self.port), end='')
            print(self.separatorDetails + "Baud rate: {0:d}".format(baud), end='')
            print(self.separatorDetails + "Parity: {0:s}".format(self.hwParity[parity]), end='')
            print(self.separatorDetails + "Loopback: {0:d}".format(loopback & 0x1), end='')
            print()
        # Send command.
        ret = self.send_cmd(cmd)
        return ret



    # Clear the UART RX buffer.
    def clear(self):
        # Clear the full UART RX buffer of 16 = 0x10 bytes.
        cmd = "uart {0:d} 1 {1:d}".format(self.port, 0x10)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Clearing the RX buffer of UART port {0:d}.".format(self.port))
        # Send command.
        # CAUTION: Do not check for errors here, as this command will return a
        #          warning, when it tries to read more bytes than available.
        mcuSerDebugLevelBak = self.mcuSer.debugLevel
        self.mcuSer.debugLevel = 0
        self.mcuSer.send(cmd)
        self.mcuSer.debugLevel = mcuSerDebugLevelBak
        return 0



    # Write data to the UART port.
    def write(self, data):
        if len(data) < 1:
            # Do not increase the error counter here!
            print(self.prefixError + "Error writing to the UART port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "At least one data byte must be provided!")
            return -1
        cmd = "uart {0:d} 0".format(self.port)
        for datum in data:
            cmd += " 0x{0:02x}".format(datum & 0xff)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Writing data to the UART port {0:d}.".format(self.port), end='')
            print(self.separatorDetails + "Data:", end='')
            for datum in data:
                print(" 0x{0:02x}".format(datum & 0xff), end='')
            print()
        # Send command.
        ret = self.send_cmd(cmd)
        self.accessWrite += 1
        self.bytesWritten += len(data)
        return ret



    # Write a string to the UART port.
    def write_str(self, s):
        data = [ord(x) for x in list(s)]
        return self.write(data)



    # Read data from the UART port.
    def read(self, cnt):
        if cnt < 1:
            # Do not increase the error counter here!
            print(self.prefixError + "Error reading from the UART port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "At least one data byte must be read!")
            return -1, []
        cmd = "uart {0:d} 1 {1:d}".format(self.port, cnt)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Reading data from the UART port {0:d}.".format(self.port))
        # Send command.
        ret = self.send_cmd(cmd)
        if ret:
            return ret, []
        # Get response.
        dataStr = self.mcuSer.get()
        # Find position of data.
        dataPos = dataStr.find(self.hwMarkData)
        # No data available.
        if dataPos < 0:
            self.errorCount += 1
            print(self.prefixError + "Error parsing data read from the UART port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "Command sent to MCU: " + cmd)
                print(self.prefixError + "Response from MCU:")
                print(self.mcuSer.get_full())
            return -1, []
        # Get sub-string containing the data. Add the length of hwMarkData to
        # point beyond the data mark.
        dataStr = dataStr[dataPos+len(self.hwMarkData):].strip()
        # Convert data string to list of data bytes.
        data = [int(i, 0) for i in filter(None, dataStr.split(" "))]
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Data read:", end='')
            for datum in data:
                print(" 0x{0:02x}".format(datum), end='')
            print()
        self.accessRead += 1
        self.bytesRead += len(data)
        return 0, data



    # Read all data from the UART port.
    def read_all(self):
        # Read all data available in the UART RX buffer.
        cmd = "uart {0:d} 1".format(self.port)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Reading all data from the UART port {0:d}.".format(self.port))
        # Send command.
        self.mcuSer.send(cmd)
        # Evaluate response.
        # CAUTION: When no data are available, the command will return a
        #          warning. This must be ignored.
        ret = self.mcuSer.eval()
        if ret > self.mcuSer.mcuResponseCodeWarning:
            print(self.prefixError + "Error reading all data from the UART port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "Command sent to MCU: " + cmd)
                print(self.prefixError + "Response from MCU:")
                print(self.mcuSer.get_full())
            return ret, []
        # Get response.
        dataStr = self.mcuSer.get()
        # Find position of data.
        dataPos = dataStr.find(self.hwMarkData)
        # No data available.
        if dataPos < 0:
            return self.mcuSer.mcuResponseCodeWarning, []   # Return code indicating a warning.
        # Get sub-string containing the data. Add the length of hwMarkData to
        # point beyond the data mark.
        dataStr = dataStr[dataPos+len(self.hwMarkData):].strip()
        # Convert data string to list of data bytes.
        data = [int(i, 0) for i in filter(None, dataStr.split(" "))]
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Data read:", end='')
            for datum in data:
                print(" 0x{0:02x}".format(datum), end='')
            print()
        self.accessRead += 1
        self.bytesRead += len(data)
        return 0, data



    # Read a string from the UART port.
    def read_str(self, cnt):
        # If the data count argument is 0 or negative, read all available data.
        if cnt <= 0:
            ret, data = self.read_all()
        else:
            ret, data = self.read(cnt)
        s = ''.join([chr(x) for x in data])
        return ret, s

