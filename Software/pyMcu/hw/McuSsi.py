# File: McuSsi.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 21 Apr 2020
# Rev.: 04 May 2020
#
# Python class for using the Synchronous Serial Interface (SSI) / Serial
# Peripheral Interface (SPI) ports of the TM4C1294NCPDT MCU.
#



import McuSerial



class McuSsi:

    # Message prefixes and separators.
    prefixDetails       = " - "
    separatorDetails    = " - "
    prefixError         = "ERROR: {0:s}: ".format(__file__)
    prefixDebug         = "DEBUG: {0:s}: ".format(__file__)

    # Debug configuration.
    debugLevel = 0                 # Debug verbosity.

    # Hardware parameters.
    hwFreqMin           = 2000
    hwFreqMax           = 50000000
    hwDataWidthMin      = 4
    hwDataWidthMax      = 16
    hwMarkData          = "Data:"
    hwMode              = ['SPI, CPOL=0, CPHA=0', 'SPI, CPOL=0, CPHA=1',
                           'SPI, CPOL=1, CPHA=0', 'SPI, CPOL=1, CPHA=1',
                           'TI frame format', 'National MicroWire']
    # Default values.
    hwFreqDefault       = 1000000
    hwModeDefault       = 0
    hwDataWidthDefault  = 8



    # Initialize the SSI port of the MCU.
    def __init__(self, mcuSer, port):
        self.port = port
        self.mcuSer = mcuSer
        self.errorCount = 0
        self.accessRead = 0
        self.accessWrite = 0
        self.dataRead = 0
        self.dataWritten = 0
        # Set up the SSI port using the default values.
        self.setup(self.hwFreqDefault, self.hwModeDefault, self.hwDataWidthDefault)



    # Send an SSI command to the MCU.
    def send_cmd(self, cmd):
        # Debug: Show command.
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Sending command to the SSI port {0:d}: ".format(self.port) + cmd)
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
            print(self.prefixError + "Error sending command to the SSI port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "Command sent to MCU: " + cmd)
                print(self.prefixError + "Response from MCU:")
                print(self.mcuSer.get_full())
            return ret
        return 0



    # Print details.
    def print_details(self):
        print(self.prefixDetails, end='')
        print("SSI port: {0:d}".format(self.port), end='')
        if self.debugLevel >= 1:
            print(self.separatorDetails + "Error count: {0:d}".format(self.errorCount), end='')
        if self.debugLevel >= 1:
            print(self.separatorDetails + "Read access count: {0:d}".format(self.accessRead), end='')
            print(self.separatorDetails + "Write access count: {0:d}".format(self.accessWrite), end='')
        if self.debugLevel >= 1:
            print(self.separatorDetails + "Data read: {0:d}".format(self.dataRead), end='')
            print(self.separatorDetails + "Data written: {0:d}".format(self.dataWritten), end='')
        print()
        return 0



    # Set up the SSI port.
    def setup(self, freq, mode, dataWidth):
        if freq < self.hwFreqMin or freq > self.hwFreqMax:
            self.errorCount += 1
            print(self.prefixError + "SSI frequency {0:d} outside of valid range {1:d}..{2:d}!".format(freq, self.hwFreqMin, self.hwFreqMax))
            return -1
        if mode < 0 or mode > len(self.hwMode) - 1:
            self.errorCount += 1
            print(self.prefixError + "SSI mode {0:d} outside of valid range {1:d}..{2:d}!".format(mode, 0, len(self.hwMode)-1))
            return -1
        if dataWidth < self.hwDataWidthMin or dataWidth > self.hwDataWidthMax:
            self.errorCount += 1
            print(self.prefixError + "SSI data width {0:d} outside of valid range {1:d}..{2:d}!".\
                format(dataWidth, self.hwDataWidthMin, self.hwDataWidthMax))
            return -1
        # Store SSI parameters.
        self.freq = freq
        self.mode = mode
        self.dataWidth = dataWidth
        # Assemble MCU command.
        cmd = "ssi-set {0:d} {1:d} {2:d} {3:d}".format(self.port, freq, mode, dataWidth)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Setting up the SSI port {0:d}.".format(self.port), end='')
            print(self.separatorDetails + "Frequency: {0:d}".format(freq), end='')
            print(self.separatorDetails + "Mode: {0:s}".format(self.hwMode[mode]), end='')
            print(self.separatorDetails + "Data width: {0:d}".format(dataWidth), end='')
            print()
        # Send command.
        ret = self.send_cmd(cmd)
        return ret



    # Clear the SSI RX buffer.
    def clear(self):
        # Clear the full SSI RX buffer of 8 = 0x08 words.
        cmd = "ssi {0:d} 1 {1:d}".format(self.port, 0x08)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Clearing the RX buffer of SSI port {0:d}.".format(self.port))
        # Send command.
        # CAUTION: Do not check for errors here, as this command will return a
        #          warning, when it tries to read more words than available.
        mcuSerDebugLevelBak = self.mcuSer.debugLevel
        self.mcuSer.debugLevel = 0
        self.mcuSer.send(cmd)
        self.mcuSer.debugLevel = mcuSerDebugLevelBak
        return 0



    # Write data to the SSI port.
    def write(self, data):
        if len(data) < 1:
            # Do not increase the error counter here!
            print(self.prefixError + "Error writing to the SSI port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "At least one data word must be provided!")
            return -1
        cmd = "ssi {0:d} 0".format(self.port)
        for datum in data:
            cmd += " 0x{0:02x}".format(datum & 0xffff)  # SSI data width can be 4..16.
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Writing data to the SSI port {0:d}.".format(self.port), end='')
            print(self.separatorDetails + "Data:", end='')
            for datum in data:
                print(" 0x{0:02x}".format(datum & 0xffff), end='')
            print()
        # Send command.
        ret = self.send_cmd(cmd)
        self.accessWrite += 1
        self.dataWritten += len(data)
        return ret



    # Read data from the SSI port.
    def read(self, cnt):
        if cnt < 1:
            # Do not increase the error counter here!
            print(self.prefixError + "Error reading from the SSI port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "At least one data word must be read!")
            return -1, []
        cmd = "ssi {0:d} 1 {1:d}".format(self.port, cnt)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Reading data from the SSI port {0:d}.".format(self.port))
        # Send command.
        ret = self.send_cmd(cmd)
        if ret:
            return ret, []
        # Get and parse response from MCU.
        dataStr = self.mcuSer.get()
        dataPos = dataStr.find(self.hwMarkData)
        if dataPos < 0:
            self.errorCount += 1
            print(self.prefixError + "Error parsing data read from the SSI port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "Command sent to MCU: " + cmd)
                print(self.prefixError + "Response from MCU:")
                print(self.mcuSer.get_full())
            return -1, []
        # Get sub-string containing the data. Add the length of hwMarkData to
        # point beyond the data mark.
        dataStr = dataStr[dataPos+len(self.hwMarkData):].strip()
        # Convert data string to list of data words.
        data = [int(i, 0) for i in filter(None, dataStr.split(" "))]
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Data read:", end='')
            for datum in data:
                print(" 0x{0:02x}".format(datum), end='')
            print()
        self.accessRead += 1
        self.dataRead += len(data)
        return 0, data



    # Read all data from the SSI port.
    def read_all(self):
        # Read all data available in the SSI RX buffer.
        cmd = "SSI {0:d} 1".format(self.port)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Reading all data from the SSI port {0:d}.".format(self.port))
        # Send command.
        self.mcuSer.send(cmd)
        # Evaluate response.
        # CAUTION: When no data are available, the command will return a
        #          warning. This must be ignored.
        ret = self.mcuSer.eval()
        if ret > self.mcuSer.mcuResponseCodeWarning:
            print(self.prefixError + "Error reading all data from the SSI port {0:d}!".format(self.port))
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
        # Convert data string to list of data words.
        data = [int(i, 0) for i in filter(None, dataStr.split(" "))]
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Data read:", end='')
            for datum in data:
                print(" 0x{0:02x}".format(datum), end='')
            print()
        self.accessRead += 1
        self.dataRead += len(data)
        return 0, data
