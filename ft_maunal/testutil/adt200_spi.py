import serial
import sys
import serial.tools.list_ports

def selftest():
    # if serial port name is unkown, use the following
    adt200 = Adt200Spi(listPorts=True)

    adt200 = Adt200Spi("COM8")
    print(adt200.get_current())

class Adt200Spi:
    def __init__(self, serial_port="", listPorts=False):
        if listPorts:
            self.list_ports()
        else:
            self.serail_port = serial_port
            self.connect()

    def connect(self):
        self.ser = serial.Serial(self.serail_port, 115200, timeout=0.1)

    def list_ports(self):
        """ Lists serial port names
            :raises EnvironmentError:
                On unsupported or unknown platforms
            :returns:
                A list of the serial ports available on the system
        """
        if sys.platform.startswith('win'):
            ports = ['COM%s' % (i + 1) for i in range(256)]
        elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
            # this excludes your current terminal "/dev/tty"
            ports = glob.glob('/dev/tty[A-Za-z]*')
        elif sys.platform.startswith('darwin'):
            ports = glob.glob('/dev/tty.*')
        else:
            raise EnvironmentError('Unsupported platform')

        result = []
        for port in ports:
            try:
                s = serial.Serial(port)
                s.close()
                result.append(port)
            except (OSError, serial.SerialException):
                pass
        print(result)

    def write(self, str):
        self.ser.write((str.encode('utf-8')))

    def query(self, str):
        self.write(str)
        return self.ser.read(1000).decode('utf-8').strip()

    def writeLUT0(self, i_n, i_p, q_n, q_p):
        if not (0 <= q_n <= 31 and
                0 <= q_p <= 31 and
                0 <= i_n <= 31 and
                0 <= i_p <= 31) :
            raise ValueError('Valid value range is: 0 to 31')

        self.reg0 = (q_n << 3) + (q_p >> 2)
        self.reg1 = ((q_p & 0b11) << 6)+ (i_n << 1) + (i_p >> 4)
        self.reg128 = (i_p & 0b1111) << 4

        self.write("w7d00{:02x}".format(self.reg0))
        self.write("w7d01{:02x}".format(self.reg1))
        self.write("w7d80{:02x}".format(self.reg128))

    def printLUT0(self):
        print(self.reg0, self.reg1, self.reg128)
        print("w00{:02x} {:02x} {:02x}".format(self.reg0, self.reg1, self.reg128))

    def set_ch4_psr_to_lut0():
        self.write("w7c0900")

    def set_ch12_psr_to_lut0():
        self.write("w7c1900")

    def set_iout(self, iout_index, value):
        regIndex_offset = 0x4c
        reg_no = regIndex_offset + iout_index
        string = "w7c{:02x}{:02x}".format(reg_no, value)
        self.write(string)

    # def set_ch15_pla(self, value):
    #     self.set_iout(28, value)

    # def set_ch15_swp(self, value):
    #     self.set_iout(32, value)

    # def set_ch0_pla(self, value):
    #     self.set_iout(1, value)

    # def set_ch0_swp(self, value):
    #     self.set_iout(5, value)

    # def set_ch15_en(self, on):
    #     value = 0
    #     if on == 1:
    #         value = 128
    #     else:
    #         vavlue = 0

    #     string = "wf8{:02x}{:02x}".format(252, value)
    #     self.write(string)

    # def set_ch0_psr(self, value):
    #     self.set_channel_psr(0, value)

    # def set_ch4_psr(self, value):
    #     self.set_channel_psr(4, value)

    # def set_ch15_psr(self, value):
    #     self.set_channel_psr(15, value)

    def set_svga_stia(self, svga_en, svga, stia):
        # if isinstance(value, int) == False \
        #     or value > 31 or value < 0:
        #     raise ValueError("invalid value!")
        regIndex_offset = 0xad
        value = (svga_en << 7) + (stia << 5) + svga;
        string = "w7c{:02x}{:02x}".format(regIndex_offset, value)
        self.write(string)

    def hw_npsrld(self):
        self.write("p") # HW LOAD

    def spi_npsrld(self):
        self.write("w7c2001") # SPI LOAD

    def set_channel_psr(self, channel, value):
        if isinstance(value, int) == False \
            or value > 63 or value < 0:
            raise ValueError("valid PSR range: 0-63")
        regIndex_offset = 2 * channel + 1
        string = "w7c{:02x}{:02x}".format(regIndex_offset, value)
        self.write(string)

    def set_channel_atn(self, channel, value):
        if isinstance(value, int) == False \
            or value > 31 or value < 0:
            raise ValueError("valid ATN range: 0-31")
        regIndex_offset = 2 * channel
        string = "w7c{:02x}{:02x}".format(regIndex_offset, value)
        self.write(string)

    def get_current(self):
        return self.query("m")

    # def set_user_mode(self):
    #     self.write("wffa5")

    def set_rx(self, on):
        if on == 1:
            self.write("r")
        else:
            self.write("t")

    # def set_ch0_3_swp(self, value):
    #     self.set_iout(33, value)

    # def set_ch12_15_swp(self, value):
    #     self.set_iout(28, value)

if __name__ == '__main__':
    selftest()