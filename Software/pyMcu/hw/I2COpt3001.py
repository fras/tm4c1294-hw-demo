# File: I2COpt3001.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 31 Mar 2020
# Rev.: 04 May 2020
#
# Python class for communicating with the OPT3001 ambient light sensor.
#



import McuI2C
import I2CDevice



class I2COpt3001:

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
        self.deviceName = "OPT3001 sensor IC"
        self.i2cDevice = I2CDevice.I2CDevice(self.mcuI2C, self.slaveAddr, self.deviceName)
        self.i2cDevice.debugLevel = self.debugLevel



    # Initialize the OPT3001 sensor IC.
    def init(self):
        self.i2cDevice.debugLevel = self.debugLevel
        # Debug info.
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Initializing the {0:s}.".format(self.deviceName), end='')
            self.i2cDevice.print_details()
        # Write data.
        ret = self.i2cDevice.write([0x01, 0xce, 0x10])
        # Evaluate response.
        if ret:
            print(self.prefixError + "Error initializing the {0:s}.".format(self.deviceName), end='')
            self.i2cDevice.print_details()
            print(self.prefixError + "Error code: {0:d}: ".format(ret))
            return ret
        return 0



    # Reset the OPT3001 sensor IC.
    def reset(self):
        self.i2cDevice.debugLevel = self.debugLevel
        # Debug info.
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Resetting the {0:s}.".format(self.deviceName), end='')
            self.i2cDevice.print_details()
        # The OPT3001 sensor IC accepts the general-call reset command. This is
        # done by writing the value 0x06 to slave address 0x00. For details see
        # the OPT3001 datasheet, section 7.5.1.2 "General-Call Reset Command".
        slaveAddrBak = self.slaveAddr
        self.slaveAddr = 0x00
        ret = self.i2cDevice.write([0x06])
        self.slaveAddr = slaveAddrBak
        # Evaluate response.
        if ret:
            print(self.prefixError + "Error resetting the {0:s}.".format(self.deviceName), end='')
            self.i2cDevice.print_details()
            print(self.prefixError + "Error code: {0:d}: ".format(ret))
            return ret
        return 0



    # Print error message if an error occurs when reading data.
    def print_data_read_error(self, strRegister, errSetPointer, errRead):
        self.i2cDevice.debugLevel = self.debugLevel
        print(self.prefixError + "Error reading the {0:s} from the {1:s}.".format(strRegister, self.deviceName), end='')
        self.i2cDevice.print_details()
        if errSetPointer:
            print(self.prefixError + "Setting of pointer register returned error code {0:d}.".format(errSetPointer))
        if errRead:
            print(self.prefixError + "Reading of data returned error code {0:d}.".format(errRead))



    # Read the light illuminance from the OPT3001 sensor IC.
    # Returns the light illuminance in lux.
    def read_illuminance(self):
        self.i2cDevice.debugLevel = self.debugLevel
        # Debug info.
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Reading illuminance from the {0:s}.".format(self.deviceName), end='')
            self.i2cDevice.print_details()
        # Set the pointer register of the OPT3001 IC to the result register (0x00).
        retSetPointer = self.i2cDevice.write([0x00])
        # Read the ambient temperature register.
        retRead, data = self.i2cDevice.read(2)
        # Evaluate response.
        if retSetPointer or retRead or len(data) != 2:
            self.print_data_read_error("illuminance", retSetPointer, retRead)
            return -1, -255.0
        # Calculate the light illuminance in lux.
        illuminanceRaw = (data[0] << 8) | data[1]
        if self.debugLevel >= 3:
            print(self.prefixDebug + "Raw illuminance value: 0x{0:04x}".format(illuminanceRaw))
        illuminanceMantissa = illuminanceRaw & 0x0fff
        illuminanceExponent = (illuminanceRaw & 0xf000) >> 12
        illuminance = float(illuminanceMantissa * 0.01 * (1 << illuminanceExponent))
        return 0, illuminance



    # Read the manufacturer ID from the OPT3001 sensor IC.
    def read_manufacturer_id(self):
        self.i2cDevice.debugLevel = self.debugLevel
        # Debug info.
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Reading manufacturer ID from the {0:s}.".format(self.deviceName), end='')
            self.i2cDevice.print_details()
        # Set the pointer register of the OPT3001 IC to the manufacturer ID register (0x7e).
        retSetPointer = self.i2cDevice.write([0x7e])
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



    # Read the device ID from the OPT3001 sensor IC.
    def read_device_id(self):
        self.i2cDevice.debugLevel = self.debugLevel
        # Debug info.
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Reading device ID from the {0:s}.".format(self.deviceName), end='')
            self.i2cDevice.print_details()
        # Set the pointer register of the OPT3001 IC to the device ID register (0x7f).
        retSetPointer = self.i2cDevice.write([0x7f])
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

