import serial
import sys
import serial.tools.list_ports

def selftest():
    # if serial port name is unkown, use the following
    adt300 = Adt300Digital(listPorts=True)

    adt300 = Adt300Digital("COM3")

class Adt300Digital:
    def __init__(self, serial_port="", listPorts=False):
        self.start_freq_19p5_ghz = 0x1c0000
        self.start_freq_20_ghz = 0x200000
        if listPorts:
            self.list_ports()
        else:
            self.serail_port = serial_port
            self.connect()

    def connect(self):
        self.ser = serial.Serial(self.serail_port, 115200, timeout=0.1)
        if self.get_manu_id() != "00171032":
            raise Exception("manu_id not matched")
        if self.get_prod_id() != "1981":
            raise Exception("prod_id not matched")

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

    def send(self, string):
        self.ser.write((string.encode('utf-8')))

    def query(self, string):
        self.send(string)
        res = self.ser.read(1000).decode('utf-8').strip()
        # print(res)
        return res

    def write(self, addr, value):
        string = "w{:08x}{:08x}".format(addr, value)
        self.send(string)

    def read(self, addr):
        string = "r{:08x}".format(addr)
        return self.query(string)[-8:]

    def get_manu_id(self):
        id0 = self.read(0x40003b84)
        id1 = self.read(0x40003b88)
        id2 = self.read(0x40003b8c)
        id3 = self.read(0x40003b90)
        return "{:s}{:s}{:s}{:s}".format(id3[-2:], id2[-2:], id1[-2:], id0[-2:])

    def get_prod_id(self):
        id0 = self.read(0x40003b94)
        id1 = self.read(0x40003b98)
        return "{:s}{:s}".format(id1[-2:], id0[-2:])

    # def get_buf_tx1(self):
    #     return self.read(0x40003118)

    def get_gpadc(self):
        return self.read(0x400030d4)
        
    def get_bias1_dinb(self):
        return self.read(0x40003118)

    def set_bias1_dinb(self, old_value, buf_20g, buf_1to4_tx, buf_1to4_lo):
        # old_value = self.get_bias1_dinb()
        old_value = int(old_value, 16);
        old_value_msb = old_value >> 16
        old_value_lsb = old_value & 0xf
        
        new_value = (buf_20g << 12) + (buf_1to4_tx << 8) + (buf_1to4_lo << 4)
        new_value += (old_value_msb << 16)+ old_value_lsb

        addr = 0x40003118
        self.write(addr, new_value)

    def set_freq_start_lsb(self, value):
        addr = 0x40003830
        self.write(addr, value)

    def set_freq_start_mid(self, value):
        addr = 0x40003834
        self.write(addr, value)

    def set_freq_start_msb(self, value):
        addr = 0x40003838
        self.write(addr, value)

    def get_freq_start_lsb(self):
        addr = 0x40003830
        return self.read(addr)

    def get_freq_start_mid(self):
        addr = 0x40003834
        return self.read(addr)

    def get_freq_start_msb(self):
        addr = 0x40003838
        return self.read(addr)

    def get_freq_start(self):
        lsb = self.get_freq_start_lsb()
        mid = self.get_freq_start_mid()
        msb = self.get_freq_start_msb()
        # print(msb, mid, lsb)
        return "{:s}{:s}{:s}".format(msb[-2:], mid[-2:], lsb[-2:])

    def set_freq_start(self, value):
        start_lsb = (value >> 0) & 0xff
        start_mid = (value >> 8) & 0xff
        start_msb = (value >> 16) & 0xff
        # print("{:x},{:x},{:x}".format(start_lsb, start_mid, start_msb))
        self.set_freq_start_lsb(start_lsb)
        self.set_freq_start_mid(start_mid)
        self.set_freq_start_msb(start_msb)

    # def writeLUT0(self, i_n, i_p, q_n, q_p):
    #     if not (0 <= q_n <= 31 and
    #             0 <= q_p <= 31 and
    #             0 <= i_n <= 31 and
    #             0 <= i_p <= 31) :
    #         raise ValueError('Valid value range is: 0 to 31')

    #     self.reg0 = (q_n << 3) + (q_p >> 2)
    #     self.reg1 = ((q_p & 0b11) << 6)+ (i_n << 1) + (i_p >> 4)
    #     self.reg128 = (i_p & 0b1111) << 4

    #     self.send("w7d00{:02x}".format(self.reg0))
    #     self.send("w7d01{:02x}".format(self.reg1))
    #     self.send("w7d80{:02x}".format(self.reg128))

    # def printLUT0(self):
    #     print(self.reg0, self.reg1, self.reg128)
    #     print("w00{:02x} {:02x} {:02x}".format(self.reg0, self.reg1, self.reg128))

    # def set_ch4_psr_to_lut0():
    #     self.send("w7c0900")

    # def set_ch12_psr_to_lut0():
    #     self.send("w7c1900")

    # def set_iout(self, iout_index, value):
    #     regIndex_offset = 0x4c
    #     reg_no = regIndex_offset + iout_index
    #     string = "w7c{:02x}{:02x}".format(reg_no, value)
    #     self.send(string)

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
    #     self.send(string)

    # def set_ch0_psr(self, value):
    #     self.set_channel_psr(0, value)

    # def set_ch4_psr(self, value):
    #     self.set_channel_psr(4, value)

    # def set_ch15_psr(self, value):
    #     self.set_channel_psr(15, value)

    # def spi_npsrld(self):
    #     self.send("w7c2001") # SPI LOAD

    # def set_channel_psr(self, channel, value):
    #     if isinstance(value, int) == False \
    #         or value > 63 or value < 0:
    #         raise ValueError("valid PSR range: 0-63")
    #     regIndex_offset = 2 * channel + 1
    #     string = "w7c{:02x}{:02x}".format(regIndex_offset, value)
    #     self.send(string)

    # def set_channel_atn(self, channel, value):
    #     if isinstance(value, int) == False \
    #         or value > 31 or value < 0:
    #         raise ValueError("valid ATN range: 0-31")
    #     regIndex_offset = 2 * channel
    #     string = "w7c{:02x}{:02x}".format(regIndex_offset, value)
    #     self.send(string)

    # def set_ch0_3_swp(self, value):
    #     self.set_iout(33, value)

    # def set_ch12_15_swp(self, value):
    #     self.set_iout(28, value)

if __name__ == '__main__':
    selftest()