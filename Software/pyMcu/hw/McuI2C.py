# File: McuI2C.py
# Auth: M. Fras, Electronics Division, MPI for Physics, Munich
# Mod.: M. Fras, Electronics Division, MPI for Physics, Munich
# Date: 28 Mar 2020
# Rev.: 21 Apr 2020
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

    # Hardware parameters.
    hwMarkData          = "Data:"
    hwMarkDevAdr        = "I2C device(s) found at slave address:"



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
        ret = self.mcuSer.eval()
        if ret:
            self.errorCount += 1
            print(self.prefixError + "Error sending command to the I2C master port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "Command sent to MCU: " + cmd)
                print(self.prefixError + "Response from MCU:")
                print(self.mcuSer.get_full())
            return ret
        return 0



    # Print details.
    def print_details(self):
        print(self.prefixDetails, end='')
        print("I2C master port: {0:d}".format(self.port), end='')
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



    # Write data to the I2C master port.
    def ms_write(self, slaveAddr, data):
        return self.ms_write_adv(slaveAddr, data, False, True)



    # Write data to the I2C master port (advanced).
    def ms_write_adv(self, slaveAddr, data, repeatedStart, stop):
        if len(data) < 1:
            # Do not increase the error counter here!
            print(self.prefixError + "Error writing to the I2C master port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "At least one data byte must be provided!")
            return -1
        accMode = 0x00 | (0x02 if repeatedStart else 0) | (0x04 if not stop else 0)
        cmd = "i2c {0:d} 0x{1:02x} 0x{2:02x}".format(self.port, slaveAddr & 0x7f, accMode)
        for datum in data:
            cmd += " 0x{0:02x}".format(datum & 0xff)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Writing data to the I2C master port {0:d}.".format(self.port), end='')
            print(self.separatorDetails + "Slave address: 0x{0:02x}".format(slaveAddr), end='')
            print(self.separatorDetails + "Access mode: 0x{0:01x}".format(accMode), end='')
            print(self.separatorDetails + "Data:", end='')
            for datum in data:
                print(" 0x{0:02x}".format(datum & 0xff), end='')
            print()
        # Send command.
        ret = self.ms_send_cmd(cmd)
        self.accessWrite += 1
        self.bytesWritten += len(data)
        return ret



    # Read data from the I2C master port.
    def ms_read(self, slaveAddr, cnt):
        return self.ms_read_adv(slaveAddr, cnt, False, True)



    # Read data from the I2C master port (advanced).
    def ms_read_adv(self, slaveAddr, cnt, repeatedStart, stop):
        if cnt < 1:
            # Do not increase the error counter here!
            print(self.prefixError + "Error reading from the I2C master port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "At least one data byte must be read!")
            return -1, []
        accMode = 0x01 | (0x02 if repeatedStart else 0) | (0x04 if not stop else 0)
        cmd = "i2c {0:d} 0x{1:02x} 0x{2:02x} {3:d}".format(self.port, slaveAddr & 0x7f, accMode, cnt)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Reading data from the I2C master port {0:d}.".format(self.port), end='')
            print(self.separatorDetails + "Slave address: 0x{0:02x}".format(slaveAddr), end='')
            print(self.separatorDetails + "Access mode: 0x{0:01x}".format(accMode), end='')
            print()
        # Send command.
        ret = self.ms_send_cmd(cmd)
        if ret:
            return ret, []
        # Get and parse response from MCU.
        dataStr = self.mcuSer.get()
        dataPos = dataStr.find(self.hwMarkData)
        if dataPos < 0:
            self.errorCount += 1
            print(self.prefixError + "Error parsing data read from the I2C master port {0:d}!".format(self.port))
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



    # Send a quick command.
    def ms_quick_cmd(self, slaveAddr, read):
        return self.ms_quick_cmd_adv(slaveAddr, read, False)



    # Send a quick command (advanced).
    def ms_quick_cmd_adv(self, slaveAddr, read, repeatedStart):
        accMode = 0x08 | (0x01 if read else 0) | (0x02 if repeatedStart else 0)
        cmd = "i2c {0:d} 0x{1:02x} 0x{2:02x}".format(self.port, slaveAddr & 0x7f, accMode)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Sending quick command to the I2C master port {0:d}.".format(self.port), end='')
            print(self.separatorDetails + "Slave address: 0x{0:02x}".format(slaveAddr), end='')
            print(self.separatorDetails + "Read/write: {0:d}".format(read & 0x01), end='')
            print()
        # Send command.
        ret = self.ms_send_cmd(cmd)
        if read:
            self.accessRead += 1
        else:
            self.accessWrite += 1
        return ret



    # Detect devices on the I2C bus.
    def ms_detect_devices(self):
        cmd = "i2c-det {0:d}".format(self.port)
        if self.debugLevel >= 2:
            print(self.prefixDebug + "Detecting devices on I2C master port {0:d}.".format(self.port), end='')
            print()
        # Temporarily increase the timeout of the serial device.
        serTimeoutBackup = self.mcuSer.ser.timeout
        self.mcuSer.ser.timeout = 0.01
        self.ms_send_cmd(cmd)
        self.mcuSer.ser.timeout = serTimeoutBackup
        ret = self.mcuSer.eval()
        if ret:
            return ret, []
        # Get and parse response from MCU.
        devAdrStr = self.mcuSer.get()
        devAdrPos = devAdrStr.find(self.hwMarkDevAdr)
        if devAdrPos < 0:
            self.errorCount += 1
            print(self.prefixError + "Error parsing device addresses while detecting I2C devices on the I2C master port {0:d}!".format(self.port))
            if self.debugLevel >= 1:
                print(self.prefixError + "Command sent to MCU: " + cmd)
                print(self.prefixError + "Response from MCU:")
                print(self.mcuSer.get_full())
            return -1, []
        # Get sub-string containing the data. Add the length of hwMarkDevAdr to
        # point beyond the data mark.
        devAdrStr = devAdrStr[devAdrPos+len(self.hwMarkDevAdr):].strip()
        # Convert device address string to list of address bytes.
        devAdr = [int(i, 0) for i in filter(None, devAdrStr.split(" "))]
        if self.debugLevel >= 2:
            print(self.prefixDebug + "I2C device{0:s} found at slave address:".format('' if len(devAdr) == 1 else 's'), end='')
            for adr in devAdr:
                print(" 0x{0:02x}".format(adr), end='')
            print()
        return 0, devAdr

