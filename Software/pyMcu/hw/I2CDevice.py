# File: I2CDevice.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 30 Apr 2020
# Rev.: 04 May 2020
#
# Python class implementing generic hardware access for I2C devices.
#



import McuI2C



class I2CDevice:

    # Message prefixes and separators.
    prefixDetails       = " - "
    separatorDetails    = " - "
    prefixDebug         = "DEBUG: {0:s}: ".format(__file__)
    prefixError         = "ERROR: {0:s}: ".format(__file__)

    # Debug configuration.
    debugLevel = 0                 # Debug verbosity.



    # Initialize the I2C device.
    def __init__(self, mcuI2C, slaveAddr, deviceName):
        self.mcuI2C = mcuI2C
        self.slaveAddr = slaveAddr
        self.deviceName = deviceName
        self.prefixDebugDevice = self.prefixDebug + self.deviceName + ": "
        self.prefixErrorDevice = self.prefixError + self.deviceName + ": "
        self.errorCount = 0
        self.accessRead = 0
        self.accessWrite = 0
        self.bytesRead = 0
        self.bytesWritten = 0



    # Write data to the I2C device.
    def write(self, dataWr):
        if self.debugLevel >= 3:
            print(self.prefixDebugDevice + "Writing data.", end='')
            print(self.prefixDetails + "Data:", end='')
            for datum in dataWr:
                print(" 0x{0:02x}".format(datum), end='')
            self.print_details()
        ret = self.mcuI2C.ms_write(self.slaveAddr, dataWr)
        if ret:
            self.errorCount += 1
            print(self.prefixErrorDevice + "Error writing data!", end='')
            self.print_details()
            return ret
        self.accessWrite += 1
        self.bytesWritten += len(dataWr)
        return 0



    # Read data from the I2C device.
    def read(self, cnt):
        if self.debugLevel >= 3:
            print(self.prefixDebugDevice + "Reading data.", end='')
            self.print_details()
        ret, dataRd = self.mcuI2C.ms_read(self.slaveAddr, cnt)
        if ret or len(dataRd) <= 0:
            self.errorCount += 1
            print(self.prefixErrorDevice + "Error reading data!", end='')
            self.print_details()
            print(self.prefixErrorDevice + "Error code: {0:d}: ".format(ret))
            return ret, dataRd
        self.accessRead += 1
        self.bytesRead += len(dataRd)
        if self.debugLevel >= 3:
            print(self.prefixDebugDevice + "Data read:", end='')
            for datum in dataRd:
                print(" 0x{0:02x}".format(datum), end='')
            print()
        return 0, dataRd



    # Write followed by a read with repeated start. This is required for SMBus
    # and PMBus read access.
    def write_read(self, dataWr, readCnt):
        if self.debugLevel >= 3:
            print(self.prefixDebugDevice + "Writing data.", end='')
            print(self.prefixDetails + "Data:", end='')
            for datum in dataWr:
                print(" 0x{0:02x}".format(datum), end='')
            self.print_details()
        # No repeated start, omit stop condition after write.
        ret = self.mcuI2C.ms_write_adv(self.slaveAddr, dataWr, False, False)
        if ret:
            self.errorCount += 1
            print(self.prefixErrorDevice + "Error writing data!", end='')
            self.print_details()
            return ret
        self.accessWrite += 1
        self.bytesWritten += len(dataWr)
        # Read access with repeated start.
        if self.debugLevel >= 3:
            print(self.prefixDebugDevice + "Reading data  with repeated start.", end='')
            self.print_details()
        # Repeated start, generate stop condition after write.
        ret, dataRd = self.mcuI2C.ms_read_adv(self.slaveAddr, readCnt, True, True)
        if ret or len(dataRd) <= 0:
            self.errorCount += 1
            print(self.prefixErrorDevice + "Error reading data!", end='')
            self.print_details()
            print(self.prefixErrorDevice + "Error code: {0:d}: ".format(ret))
            return ret, dataRd
        self.accessRead += 1
        self.bytesRead += len(dataRd)
        if self.debugLevel >= 3:
            print(self.prefixDebugDevice + "Data read:", end='')
            for datum in dataRd:
                print(" 0x{0:02x}".format(datum), end='')
            print()
        return 0, dataRd



    # Print details.
    def print_details(self):
        print(self.prefixDetails, end='')
        print("I2C master port: {0:d}".format(self.mcuI2C.port), end='')
        print(self.separatorDetails + "Slave address: 0x{0:02x}".format(self.slaveAddr), end='')
        if self.debugLevel >= 1:
            print(self.separatorDetails + "Error count: {0:d}".format(self.errorCount), end='')
        if self.debugLevel >= 2:
            print(self.separatorDetails + "Read access count: {0:d}".format(self.accessRead), end='')
            print(self.separatorDetails + "Write access count: {0:d}".format(self.accessWrite), end='')
        if self.debugLevel >= 2:
            print(self.separatorDetails + "Bytes read: {0:d}".format(self.bytesRead), end='')
            print(self.separatorDetails + "Bytes written: {0:d}".format(self.bytesWritten), end='')
        print()
        return 0

