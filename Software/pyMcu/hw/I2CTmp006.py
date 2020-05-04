# File: I2CTmp006.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 28 Mar 2020
# Rev.: 04 May 2020
#
# Python class for communicating with the TMP006 I2C temperature sensor IC.
#



import McuI2C
import I2CDevice



class I2CTmp006:

    # Message prefixes and separators.
    prefixDetails       = " - "
    separatorDetails    = " - "
    prefixDebug         = "DEBUG: {0:s}: ".format(__file__)
    prefixError         = "ERROR: {0:s}: ".format(__file__)

    # Debug configuration.
    debugLevel = 0                 # Debug verbosity.



    # Initialize the I2C device.
    def __init__(self, mcuI2C, slaveAddr):
        self.mcuI2C = mcuI2C
        self.slaveAddr = slaveAddr
        self.deviceName = "TMP006 sensor IC"
        self.i2cDevice = I2CDevice.I2CDevice(self.mcuI2C, self.slaveAddr, self.deviceName)
        self.i2cDevice.debugLevel = self.debugLevel



    # Initialize the TMP006 sensor IC.
    def init(self):
        self.i2cDevice.debugLevel = self.debugLevel
        # Debug info.
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Initializing the {0:s}.".format(self.deviceName), end='')
            self.i2cDevice.print_details()
        # Write data.
        ret = self.i2cDevice.write([0x02, 0x75, 0x00])
        # Evaluate response.
        if ret:
            print(self.prefixError + "Error initializing the {0:s}!".format(self.deviceName), end='')
            self.i2cDevice.print_details()
            print(self.prefixError + "Error code: {0:d}: ".format(ret))
            return -1
        return 0



    # Reset the TMP006 sensor IC.
    def reset(self):
        self.i2cDevice.debugLevel = self.debugLevel
        # Debug info.
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Resetting the {0:s}.".format(self.deviceName), end='')
            self.i2cDevice.print_details()
        # Write data.
        ret = self.i2cDevice.write([0x02, 0x80, 0x00])
        # Evaluate response.
        if ret:
            print(self.prefixError + "Error resetting the {0:s}!".format(self.deviceName), end='')
            self.i2cDevice.print_details()
            print(self.prefixError + "Error code: {0:d}: ".format(ret))
            return -1
        return 0



    # Print error message if an error occurs when reading data.
    def print_data_read_error(self, strRegister, errSetPointer, errRead):
        self.i2cDevice.debugLevel = self.debugLevel
        print(self.prefixError + "Error reading the {0:s} from the {1:s}!".format(strRegister, self.deviceName), end='')
        self.i2cDevice.print_details()
        if errSetPointer:
            print(self.prefixError + "Setting of pointer register returned error code {0:d}.".format(errSetPointer))
        if errRead:
            print(self.prefixError + "Reading of data returned error code {0:d}.".format(errRead))



    # Read the sensor voltage from the TMP006 sensor IC.
    # Returns the sensor voltage in nano volts (nV).
    def read_voltage(self):
        self.i2cDevice.debugLevel = self.debugLevel
        # Debug info.
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Reading voltage from the {0:s}.".format(self.deviceName), end='')
            self.i2cDevice.print_details()
        # Set the pointer register of the TMP006 IC to the sensor voltage register (0x00).
        retSetPointer = self.i2cDevice.write([0x00])
        # Read the sensor voltage register.
        retRead, data = self.i2cDevice.read(2)
        # Evaluate response.
        if retSetPointer or retRead or len(data) != 2:
            self.print_data_read_error("sensor voltage", retSetPointer, retRead)
            return -1, -255.0
        # Calculate the sensor voltage in nano Volts (nV).
        voltageRaw = (data[0] << 8) | data[1]
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Raw sensor voltage value: 0x{0:04x}".format(voltageRaw))
        # Negative sensor voltage.
        if voltageRaw & 0x8000:
            voltageRaw = (~voltageRaw + 1) & 0x7fff
            voltageSign = -1
        else:
            voltageSign = 1
        voltage = float(voltageRaw * 156.25 * voltageSign)
        return 0, voltage



    # Read the temperature from the TMP006 sensor IC.
    # Returns the temperature in degree Celcius.
    def read_temperature(self):
        self.i2cDevice.debugLevel = self.debugLevel
        # Debug info.
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Reading temperature from the {0:s}.".format(self.deviceName), end='')
            self.i2cDevice.print_details()
        # Set the pointer register of the TMP006 IC to the ambient temperature register (0x01).
        retSetPointer = self.i2cDevice.write([0x01])
        # Read the ambient temperature register.
        retRead, data = self.i2cDevice.read(2)
        # Evaluate response.
        if retSetPointer or retRead or len(data) != 2:
            self.print_data_read_error("temperature", retSetPointer, retRead)
            return -1, -255.0
        # Calculate the temperature in degree Celcius.
        temperatureRaw = (data[0] << 6) | (data[1] >> 2)
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Raw temperature value: 0x{0:04x}".format(temperatureRaw))
        # Negative temperature.
        if temperatureRaw & 0x2000 != 0:
            temperatureRaw = (~temperatureRaw + 1) & 0x1fff
            temperatureSign = -1
        else:
            temperatureSign = 1
        temperature = float(temperatureRaw / 32.0 * temperatureSign)
        return 0, temperature



    # Read the manufacturer ID from the TMP006 sensor IC.
    def read_manufacturer_id(self):
        self.i2cDevice.debugLevel = self.debugLevel
        # Debug info.
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Reading manufacturer ID from the {0:s}.".format(self.deviceName), end='')
            self.i2cDevice.print_details()
        # Set the pointer register of the TMP006 IC to the manufacturer ID register (0xfe).
        retSetPointer = self.i2cDevice.write([0xfe])
        # Read the manufacturer ID register.
        retRead, data = self.i2cDevice.read(2)
        # Evaluate response.
        if retSetPointer or retRead or len(data) != 2:
            self.print_data_read_error("manufacturer ID", retSetPointer, retRead)
            return -1, 0xffff
        # Assemble the manufacturer ID.
        manufacturerId = (data[0] << 8) | data[1]
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Manufacturer ID: 0x{0:04x}".format(manufacturerId))
        return 0, manufacturerId



    # Read the device ID from the TMP006 sensor IC.
    def read_device_id(self):
        self.i2cDevice.debugLevel = self.debugLevel
        # Debug info.
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Reading device ID from the {0:s}.".format(self.deviceName), end='')
            self.i2cDevice.print_details()
        # Set the pointer register of the TMP006 IC to the device ID register (0xff).
        retSetPointer = self.i2cDevice.write([0xff])
        # Read the device ID register.
        retRead, data = self.i2cDevice.read(2)
        # Evaluate response.
        if retSetPointer or retRead or len(data) != 2:
            self.print_data_read_error("device ID", retSetPointer, retRead)
            return -1, 0xffff
        # Assemble the device ID.
        deviceId = (data[0] << 8) | data[1]
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Device ID: 0x{0:04x}".format(deviceId))
        return 0, deviceId

