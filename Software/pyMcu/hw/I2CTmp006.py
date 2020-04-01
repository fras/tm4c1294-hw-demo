# File: I2CTmp006.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 28 Mar 2020
# Rev.: 31 Mar 2020
#
# Python class for communicating with the TMP006 I2C temperature sensor IC.
#



import McuI2C



class I2CTmp006:

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



    # Write data to the TMP006 sensor IC.
    def write(self, data):
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Writing data to the TMP006 sensor IC.", end = '')
            self.print_details()
        ret = self.mcuI2C.ms_write(self.slaveAddr, data)
        if ret != 0:
            self.errorCount += 1
            print(self.prefixError + "Error writing data to the TMP006 sensor IC!", end = '')
            self.print_details()
            return ret
        self.accessWrite += 1
        self.bytesWritten += len(data)
        return ret



    # Read data from the TMP006 sensor IC.
    def read(self, cnt):
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Reading data from the TMP006 sensor IC.", end = '')
            self.print_details()
        data = self.mcuI2C.ms_read(self.slaveAddr, cnt)
        if len(data) <= 0:
            self.errorCount += 1
            print(self.prefixError + "Error reading data from the TMP006 sensor IC!", end = '')
            self.print_details()
            return data
        self.accessRead += 1
        self.bytesRead += len(data)
        return data



    # Print details.
    def print_details(self):
        print(self.prefixDetails, end = '')
        print("I2C master port: {0:d}".format(self.mcuI2C.port), end = '')
        print(self.separatorDetails + "Slave address: 0x{0:02x}".format(self.slaveAddr), end = '')
        if self.debugLevel >= 1:
            print(self.separatorDetails + "Error count: {0:d}".format(self.errorCount), end = '')                
        if self.debugLevel >= 2:
            print(self.separatorDetails + "Read access count: {0:d}".format(self.accessRead), end = '')
            print(self.separatorDetails + "Write access countn: {0:d}".format(self.accessWrite), end = '')
        if self.debugLevel >= 2:
            print(self.separatorDetails + "Bytes read: {0:d}".format(self.bytesRead), end = '')
            print(self.separatorDetails + "Bytes written: {0:d}".format(self.bytesWritten), end = '')        
        print()



    # Initialize the TMP006 sensor IC.
    def init(self):
        # Debug info.
        if self.debugLevel >= 1:
            print(self.prefixDebug + "Initializing the TMP006 sensor IC.", end = '')
            self.print_details()
        # Write data.
        ret = self.write([0x02, 0x75, 0x00])
        # Evaluate response.
        if ret != 0:
            print(self.prefixError + "Error initializing the TMP006 sensor IC!", end = '')
            self.print_details()
            print(self.prefixError + "Error code: {0:d}: ".format(ret))
            return -1
        return 0



    # Reset the TMP006 sensor IC.
    def reset(self):
        # Debug info.
        if self.debugLevel >= 1:
            print(self.prefixDebug + "Resetting the TMP006 sensor IC.", end = '')
            self.print_details()
        # Write data.
        ret = self.write([0x02, 0x80, 0x00])
        # Evaluate response.
        if ret != 0:
            print(self.prefixError + "Error resetting the TMP006 sensor IC!", end = '')
            self.print_details()
            print(self.prefixError + "Error code: {0:d}: ".format(ret))
            return -1
        return 0



    # Read the sensor voltage from the TMP006 sensor IC.
    # Returns the sensor voltage in nano volts (nV).
    def read_voltage(self):
        # Debug info.
        if self.debugLevel >= 1:
            print(self.prefixDebug + "Reading voltage from the TMP006 sensor IC.", end = '')
            self.print_details()
        # Set the pointer register of the TMP006 IC to the sensor voltage register (0x00).
        ret = self.write([0x00])
        # Read the sensor voltage register.
        data = self.read(2)
        # Evaluate response.
        if ret != 0 or len(data) != 2:
            print(self.prefixError + "Error reading the sensor voltage from the TMP006 sensor IC!", end = '')
            self.print_details()
            print(self.prefixError + "Error code: {0:d}: ".format(ret))
            return -255.0
        # Calculate the sensor voltage in nano Volts (nV).
        voltageRaw = (data[0] << 8) | data[1]
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Raw sensor voltage value: 0x{0:04x}".format(voltageRaw))
        # Negative sensor voltage.
        if voltageRaw & 0x8000:
            voltageRaw = (~voltageRaw + 1) & 0x7fff
            voltageSign = -1
        else:
            voltageSign = 1
        voltage = float(voltageRaw * 156.25 * voltageSign)
        return voltage



    # Read the temperature from the TMP006 sensor IC.
    # Returns the temperature in degree Celcius.
    def read_temperature(self):
        # Debug info.
        if self.debugLevel >= 1:
            print(self.prefixDebug + "Reading temperature from the TMP006 sensor IC.", end = '')
            self.print_details()
        # Set the pointer register of the TMP006 IC to the ambient temperature register (0x01).
        ret = self.write([0x01])
        # Read the ambient temperature register.
        data = self.read(2)
        # Evaluate response.
        if ret != 0 or len(data) != 2:
            print(self.prefixError + "Error reading the temperature from the TMP006 sensor IC!", end = '')
            self.print_details()
            print(self.prefixError + "Error code: {0:d}: ".format(ret))
            return -255.0
        # Calculate the temperature in degree Celcius.
        temperatureRaw = (data[0] << 6) | (data[1] >> 2)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Raw temperature value: 0x{0:04x}".format(temperatureRaw))
        # Negative temperature.
        if temperatureRaw & 0x2000 != 0:
            temperatureRaw = (~temperatureRaw + 1) & 0x1fff
            temperatureSign = -1
        else:
            temperatureSign = 1
        temperature = float(temperatureRaw / 32.0 * temperatureSign)
        return temperature



    # Read the manufacturer ID from the TMP006 sensor IC.
    def read_manufacturer_id(self):
        # Debug info.
        if self.debugLevel >= 1:
            print(self.prefixDebug + "Reading manufacturer ID from the TMP006 sensor IC.", end = '')
            self.print_details()
        # Set the pointer register of the TMP006 IC to the manufacturer ID register (0xfe).
        ret = self.write([0xfe])
        # Read the manufacturer ID register.
        data = self.read(2)
        # Evaluate response.
        if ret != 0 or len(data) != 2:
            print(self.prefixError + "Error reading the manufacturer ID from the TMP006 sensor IC!", end = '')
            self.print_details()
            print(self.prefixError + "Error code: {0:d}: ".format(ret))
            return 0xffff
        # Assemble the manufacturer ID.
        manufacturerId = (data[0] << 8) | data[1]
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Manufacturer ID: 0x{0:04x}".format(manufacturerId))
        return manufacturerId



    # Read the device ID from the TMP006 sensor IC.
    def read_device_id(self):
        # Debug info.
        if self.debugLevel >= 1:
            print(self.prefixDebug + "Reading device ID from the TMP006 sensor IC.", end = '')
            self.print_details()
        # Set the pointer register of the TMP006 IC to the device ID register (0xff).
        ret = self.write([0xff])
        # Read the device ID register.
        data = self.read(2)
        # Evaluate response.
        if ret != 0 or len(data) != 2:
            print(self.prefixError + "Error reading the device ID from the TMP006 sensor IC!", end = '')
            self.print_details()
            print(self.prefixError + "Error code: {0:d}: ".format(ret))
            return 0xffff
        # Assemble the device ID.
        deviceId = (data[0] << 8) | data[1]
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Device ID: 0x{0:04x}".format(deviceId))
        return deviceId
