# File: McuI2C.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 28 Mar 2020
# Rev.: 31 Mar 2020
#
# Python class for using the I2C ports of the TM4C1294NCPDT MCU.
#



import McuSerial



class McuI2C:

    # Message prefixes and separators.
    prefixDetails       = " - "
    separatorDetails    = " - "    
    prefixError         = "ERROR: {0:s}: ".format(__file__)
    prefixDebug         = "DEBUG: {0:s}: ".format(__file__)

    # Debug configuration.
    debugLevel = 0                 # Debug verbosity.



    # Initialize the I2C port of the MCU.
    def __init__(self, mcuSer, port):
        self.port = port
        self.mcuSer = mcuSer
        self.errorCount = 0
        self.accessRead = 0
        self.accessWrite = 0
        self.bytesRead = 0
        self.bytesWritten = 0



    # Send an I2C command to the MCU.
    def ms_send_cmd(self, cmd):
        # Debug: Show command.
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Sending command to the I2C master port {0:d}: ".format(self.port) + cmd)
        # Send command.
        self.mcuSer.send(cmd)
        # Debug: Show response.
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Response from MCU:")
            print(self.mcuSer.get_full())
        # Evaluate response.
        if self.mcuSer.eval():
            self.errorCount += 1
            print(self.prefixError + "Error sending command to the I2C master port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "Command sent to MCU: " + cmd)
                print(self.prefixError + "Response from MCU:")
                print(self.mcuSer.get_full())
            return -1
        return 0



    # Print details.
    def print_details(self):
        print(self.prefixDetails, end = '')
        print("I2C master port: {0:d}".format(self.port), end = '')
        if self.debugLevel >= 1:
            print(self.separatorDetails + "Error count: {0:d}".format(self.errorCount), end = '')        
        if self.debugLevel >= 1:
            print(self.separatorDetails + "Read access count: {0:d}".format(self.accessRead), end = '')
            print(self.separatorDetails + "Write access countn: {0:d}".format(self.accessWrite), end = '')
        if self.debugLevel >= 1:
            print(self.separatorDetails + "Bytes read: {0:d}".format(self.bytesRead), end = '')
            print(self.separatorDetails + "Bytes written: {0:d}".format(self.bytesWritten), end = '')
        print()



    # Write data to the I2C master port.
    def ms_write(self, slaveAddr, data):
        if len(data) < 1:
            # Do not increase the error counter here!
            print(self.prefixError + "Error writing to the I2C master port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "At least one data byte must be provided!")
            return -1
        cmd = "i2c {0:d} 0x{1:02x} 0".format(self.port, slaveAddr & 0x7f)
        for i in range(0, len(data)):
            cmd += " 0x{0:02x}".format(data[i] & 0xff)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Writing data to the I2C master port {0:d}.".format(self.port), end = '')
            print(self.separatorDetails + "Slave address: 0x{0:02x}".format(slaveAddr), end = '')
            print(self.separatorDetails + "Data:", end = '')
            for i in range(0, len(data)):
                print(" 0x{0:02x}".format(data[i] & 0xff), end = '')
            print()
        # Send command.
        ret = self.ms_send_cmd(cmd)
        self.accessWrite += 1
        self.bytesWritten += len(data)
        return ret



    # Read data from the I2C master port.
    def ms_read(self, slaveAddr, cnt):
        if cnt < 1:
            # Do not increase the error counter here!
            print(self.prefixError + "Error reading from the I2C master port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "At least one data byte must be read!")
            return []
        cmd = "i2c {0:d} 0x{1:02x} 1 {2:d}".format(self.port, slaveAddr & 0x7f, cnt)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Reading data from the I2C master port {0:d}.".format(self.port))
        # Send command.
        if self.ms_send_cmd(cmd):
            return []
        # Get response.
        ret = self.mcuSer.get()
        dataPos = ret.find("0x")
        if dataPos < 0:
            self.errorCount += 1
            print(self.prefixError + "Error parsing data read from the I2C master port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "Command sent to MCU: " + cmd)
                print(self.prefixError + "Response from MCU:")
                print(self.mcuSer.get_full())
            return []
        dataStr = ret[dataPos:].rstrip()
        dataStrList = dataStr.split(" ")
        data = []
        for i in range(0, len(dataStrList)):
            data.append(int(dataStrList[i], 0))
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Data read:", end = '')
            for i in range(0, len(data)):
                print(" 0x{0:02x}".format(data[i]), end = '')
            print()
        self.accessRead += 1
        self.bytesRead += len(data)
        return data

