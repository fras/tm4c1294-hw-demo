# File: I2COpt3001.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 31 Mar 2020
# Rev.: 15 Apr 2020
#
# Python class for communicating with the OPT3001 ambient light sensor.
#



import McuI2C



class I2COpt3001:

    # Message prefixes and separators.
    prefixDetails       = " - "
    separatorDetails    = " - "
    prefixError         = "ERROR: {0:s}: ".format(__file__)
    prefixDebug         = "DEBUG: {0:s}: ".format(__file__)

    # Debug configuration.
    debugLevel = 0                 # Debug verbosity.



    # Initialize the I2C device.
    def __init__(self, mcuI2C, slaveAddr):
        self.mcuI2C = mcuI2C
        self.slaveAddr = slaveAddr
        self.errorCount = 0
        self.accessRead = 0
        self.accessWrite = 0        
        self.bytesRead = 0
        self.bytesWritten = 0        



    # Write data to the OPT3001 sensor IC.
    def write(self, data):
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Writing data to the OPT3001 sensor IC.", end='')
            self.print_details()
        ret = self.mcuI2C.ms_write(self.slaveAddr, data)
        if ret != 0:
            self.errorCount += 1
            print(self.prefixError + "Error writing data to the OPT3001 sensor IC!", end='')
            self.print_details()
            return ret
        self.accessWrite += 1
        self.bytesWritten += len(data)
        return ret



    # Read data from the OPT3001 sensor IC.
    def read(self, cnt):
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Reading data from the OPT3001 sensor IC.", end='')
            self.print_details()
        data = self.mcuI2C.ms_read(self.slaveAddr, cnt)
        if len(data) <= 0:
            self.errorCount += 1
            print(self.prefixError + "Error reading data from the OPT3001 sensor IC!", end='')
            self.print_details()
            return data
        self.accessRead += 1
        self.bytesRead += len(data)
        return data



    # Print details.
    def print_details(self):
        print(self.prefixDetails, end='')
        print("I2C master port: {0:d}".format(self.mcuI2C.port), end='')
        print(self.separatorDetails + "Slave address: 0x{0:02x}".format(self.slaveAddr), end='')
        if self.debugLevel >= 1:
            print(self.separatorDetails + "Error count: {0:d}".format(self.errorCount), end='')                
        if self.debugLevel >= 2:
            print(self.separatorDetails + "Read access count: {0:d}".format(self.accessRead), end='')
            print(self.separatorDetails + "Write access countn: {0:d}".format(self.accessWrite), end='')
        if self.debugLevel >= 2:
            print(self.separatorDetails + "Bytes read: {0:d}".format(self.bytesRead), end='')
            print(self.separatorDetails + "Bytes written: {0:d}".format(self.bytesWritten), end='')        
        print()



    # Initialize the OPT3001 sensor IC.
    def init(self):
        # Debug info.
        if self.debugLevel >= 1:
            print(self.prefixDebug + "Initializing the OPT3001 sensor IC.", end='')
            self.print_details()
        # Write data.
        ret = self.write([0x01, 0xce, 0x10])
        # Evaluate response.
        if ret != 0:
            print(self.prefixError + "Error initializing the OPT3001 sensor IC!", end='')
            self.print_details()
            print(self.prefixError + "Error code: {0:d}: ".format(ret))
            return -1
        return 0



    # Reset the OPT3001 sensor IC.
    def reset(self):
        # Debug info.
        if self.debugLevel >= 1:
            print(self.prefixDebug + "Resetting the OPT3001 sensor IC.", end='')
            self.print_details()
        # The OPT3001 sensor IC accepts the general-call reset command. This is
        # done by writing the value 0x06 to slave address 0x00. For details see
        # the OPT3001 datasheet, section 7.5.1.2 "General-Call Reset Command".
        slaveAddrBak = self.slaveAddr
        self.slaveAddr = 0x00
        ret = self.write([0x06])
        self.slaveAddr = slaveAddrBak
        # Evaluate response.
        if ret != 0:
            print(self.prefixError + "Error resetting the OPT3001 sensor IC!", end='')
            self.print_details()
            print(self.prefixError + "Error code: {0:d}: ".format(ret))
            return -1
        return 0



    # Read the light illuminance from the OPT3001 sensor IC.
    # Returns the light illuminance in lux.    
    def read_illuminance(self):
        # Debug info.
        if self.debugLevel >= 1:
            print(self.prefixDebug + "Reading illuminance from the OPT3001 sensor IC.", end='')
            self.print_details()
        # Set the pointer register of the OPT3001 IC to the result register (0x00).
        ret = self.write([0x00])
        # Read the ambient temperature register.
        data = self.read(2)
        # Evaluate response.
        if ret != 0 or len(data) != 2:
            print(self.prefixError + "Error reading the illuminance from the OPT3001 sensor IC!", end='')
            self.print_details()
            print(self.prefixError + "Error code: {0:d}: ".format(ret))
            return -255.0
        # Calculate the light illuminance in lux.
        illuminanceRaw = (data[0] << 8) | data[1]
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Raw illuminance value: 0x{0:04x}".format(illuminanceRaw))
        illuminanceMantissa = illuminanceRaw & 0x0fff;
        illuminanceExponent = (illuminanceRaw & 0xf000) >> 12;
        illuminance = float(illuminanceMantissa * 0.01 * (1 << illuminanceExponent))
        return illuminance



    # Read the manufacturer ID from the OPT3001 sensor IC.
    def read_manufacturer_id(self):
        # Debug info.
        if self.debugLevel >= 1:
            print(self.prefixDebug + "Reading manufacturer ID from the OPT3001 sensor IC.", end='')
            self.print_details()
        # Set the pointer register of the OPT3001 IC to the manufacturer ID register (0x7e).
        ret = self.write([0x7e])
        # Read the manufacturer ID register.
        data = self.read(2)
        # Evaluate response.
        if ret != 0 or len(data) != 2:
            print(self.prefixError + "Error reading the manufacturer ID from the OPT3001 sensor IC!", end='')
            self.print_details()
            print(self.prefixError + "Error code: {0:d}: ".format(ret))
            return 0xffff
        # Assemble the manufacturer ID.
        manufacturerId = (data[0] << 8) | data[1]
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Manufacturer ID: 0x{0:04x}".format(manufacturerId))
        return manufacturerId



    # Read the device ID from the OPT3001 sensor IC.
    def read_device_id(self):
        # Debug info.
        if self.debugLevel >= 1:
            print(self.prefixDebug + "Reading device ID from the OPT3001 sensor IC.", end='')
            self.print_details()
        # Set the pointer register of the OPT3001 IC to the device ID register (0x7f).
        ret = self.write([0x7f])
        # Read the device ID register.
        data = self.read(2)
        # Evaluate response.
        if ret != 0 or len(data) != 2:
            print(self.prefixError + "Error reading the device ID from the OPT3001 sensor IC!", end='')
            self.print_details()
            print(self.prefixError + "Error code: {0:d}: ".format(ret))
            return 0xffff
        # Assemble the device ID.
        deviceId = (data[0] << 8) | data[1]
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Device ID: 0x{0:04x}".format(deviceId))
        return deviceId

