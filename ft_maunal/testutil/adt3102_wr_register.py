import time

from testutil.jlink_util import jlink_util
from testutil.uart import uartutil


class adt3102_wr_register(jlink_util, uartutil):
    def __init__(self, *args, **kwargs):
        jlink_util.__init__(self, *args, **kwargs)
        uartutil.__init__(self, *args, **kwargs)

    def uart_init(self, switch_com):
        current_Com_Dict = self.port_check()
        # noinspection PyBroadException
        try:
            self.port_close()
        except:
            pass
        self.port_open(current_Com_Dict, switch_com)

    def read_register_uart(self, read_register_address):
        read_register_address_temp = str(hex(read_register_address))
        read_register_address_temp = read_register_address_temp[2:]
        if len(read_register_address_temp) != 8:
            read_register_address_temp = (8 - len(read_register_address_temp)) * "0" + read_register_address_temp
        read_register_value = self.data_send("r" + read_register_address_temp)
        read_register_value = int(read_register_value[16:26], 16)
        return read_register_value

    def write_register_uart(self, write_register_address, write_register_value):
        write_register_address_temp = str(hex(write_register_address))
        write_register_address_temp = write_register_address_temp[2:]
        if len(write_register_address_temp) != 8:
            write_register_address_temp = (8 - len(write_register_address_temp)) * "0" + write_register_address_temp
        return_info = self.data_send("w" + write_register_address_temp + self.supplement_hex(write_register_value))
        return return_info

    @staticmethod
    def supplement_hex(str_hex):
        str_hex = hex(str_hex)
        str_hex_temp = str_hex[2:]
        write_data = (8 - len(str_hex_temp)) * '0' + str_hex_temp
        return write_data

    def tia_setting(self, tia0IGain, tia0QGain, tia1IGain, tia1IQain):
        regVel = self.read_memory(0x40003218, 1)
        regVel[0] &= ~(((1 << 2) - 1) << 8)
        regVel[0] |= tia0IGain << 8

        regVel[0] &= ~(((1 << 2) - 1) << 10)
        regVel[0] |= tia0QGain << 10
        self.write_memory(0x40003218, regVel[0])

        regVel = self.read_memory(0x4000321c, 1)
        regVel[0] &= ~(((1 << 2) - 1) << 8)
        regVel[0] |= tia1IGain << 8

        regVel[0] &= ~(((1 << 2) - 1) << 10)
        regVel[0] |= tia1IQain << 10
        self.write_memory(0x4000321c, regVel[0])

    def pga_setting(self, pga0IGain, pga0QGain, pga1IGain, pga1QGain):
        regVel = self.read_memory(0x40003220, 1)
        regVel[0] &= ~(((1 << 5) - 1) << 16)
        regVel[0] |= pga0IGain << 16

        regVel[0] &= ~(((1 << 5) - 1) << 21)
        regVel[0] |= pga0QGain << 21
        self.write_memory(0x40003220, regVel[0])

        regVel = self.read_memory(0x40003224, 1)
        regVel[0] &= ~(((1 << 5) - 1) << 16)
        regVel[0] |= pga1IGain << 16

        regVel[0] &= ~(((1 << 5) - 1) << 21)
        regVel[0] |= pga1QGain << 21
        self.write_memory(0x40003224, regVel[0])

    def bistdiv_setting(self, bistDivSel):
        regVel = self.read_memory(0x40003268, 1)
        regVel[0] &= ~(((1 << 9) - 1) << 8)
        regVel[0] |= bistDivSel << 8
        self.write_memory(0x40003268, regVel[0])

    def pga_band_width(self, pga0LpfParameter, pga1LpfParameter):
        regVel = self.read_memory(0x40003220, 1)
        regVel[0] &= ~(0x3 << 3)
        regVel[0] |= pga0LpfParameter << 3
        self.write_memory(0x40003220, regVel[0])

        regVel = self.read_memory(0x40003224, 1)
        regVel[0] &= ~(0x3 << 3)
        regVel[0] |= pga1LpfParameter << 3
        self.write_memory(0x40003224, regVel[0])

    def ADT300_config(self, Fstart_actual):
        freq_start = int(round(pow(2, 18) * ((Fstart_actual - 64) / 2), 0))
        value = [(freq_start & 0xff)]
        self.memory_write32(0x40003830, value)
        value = [(freq_start & 0xff00) >> 8]
        self.memory_write32(0x40003834, value)
        value = [(freq_start & 0x7f0000) >> 16]
        self.memory_write32(0x40003838, value)

    def ADT300_config_Uart(self, Fstart_actual):
        Fstart_actual = int(round(pow(2, 18) * ((Fstart_actual - 64) / 2), 0))
        value = (Fstart_actual & 0xff)
        self.write_register_uart(0x40003830, value)
        value = (Fstart_actual & 0xff00) >> 8
        self.write_register_uart(0x40003834, value)
        value = (Fstart_actual & 0x7f0000) >> 16
        self.write_register_uart(0x40003838, value)
        time.sleep(0.1)
        # self.write_register_uart(0x0000fff0, 0x00000b00)
        self.write_register_uart(0x4000381c, 0x00000005)
        self.write_register_uart(0x40003800, 0x000000d0)
        self.write_register_uart(0x40003800, 0x000000d1)
        self.write_register_uart(0x40003800, 0x000000d0)
        self.write_register_uart(0x40003800, 0x000000d1)
        self.write_register_uart(0x40003800, 0x000000d0)

    def tia_setting_uart(self, tia0IGain, tia0QGain, tia1IGain, tia1IQain):
        regVel = self.read_register_uart(0x40003218)
        regVel &= ~(((1 << 2) - 1) << 8)
        regVel |= tia0IGain << 8

        regVel &= ~(((1 << 2) - 1) << 10)
        regVel |= tia0QGain << 10
        self.write_register_uart(0x40003218, regVel)

        regVel = self.read_register_uart(0x4000321c)
        regVel &= ~(((1 << 2) - 1) << 8)
        regVel |= tia1IGain << 8

        regVel &= ~(((1 << 2) - 1) << 10)
        regVel |= tia1IQain << 10
        self.write_register_uart(0x4000321c, regVel)

    def pga_setting_uart(self, pga0IGain, pga0QGain, pga1IGain, pga1QGain):
        regVel = self.read_register_uart(0x40003220)
        regVel &= ~(((1 << 5) - 1) << 16)
        regVel |= pga0IGain << 16

        regVel &= ~(((1 << 5) - 1) << 21)
        regVel |= pga0QGain << 21
        self.write_register_uart(0x40003220, regVel)

        regVel = self.read_register_uart(0x40003224)
        regVel &= ~(((1 << 5) - 1) << 16)
        regVel |= pga1IGain << 16

        regVel &= ~(((1 << 5) - 1) << 21)
        regVel |= pga1QGain << 21
        self.write_register_uart(0x40003224, regVel)

    def bistdiv_setting_uart(self, bistDivSel):
        regVel = self.read_register_uart(0x40003268)
        regVel &= ~(((1 << 9) - 1) << 8)
        regVel |= bistDivSel << 8
        self.write_register_uart(0x40003268, regVel)

    def pga_band_width_uart(self, pga0LpfParameter, pga1LpfParameter):
        regVel = self.read_register_uart(0x40003220)
        regVel &= ~(0x3 << 3)
        regVel |= pga0LpfParameter << 3
        self.write_register_uart(0x40003220, regVel)

        regVel = self.read_register_uart(0x40003224)
        regVel &= ~(0x3 << 3)
        regVel |= pga1LpfParameter << 3
        self.write_register_uart(0x40003224, regVel)

    def pa_enable(self, pa0Enable, pa1Enable):
        regVel = self.read_register_uart(0x4000320c)
        if pa0Enable == 1:
            regVel |= (1 << 0)
        else:
            regVel &= ~(1 << 0)
        if pa1Enable == 1:
            regVel |= (1 << 1)
        else:
            regVel &= ~(1 << 1)
        self.write_register_uart(0x4000320c, regVel)

    def pa_bias(self, pa0Bias, pa1Bias):
        regVel = self.read_register_uart(0x40003114)
        regVel &= ~(((1 << 4) - 1) << 4)
        regVel |= pa0Bias << 4
        regVel &= ~((1 << 4) - 1)
        regVel |= pa1Bias
        self.write_register_uart(0x40003114, regVel)

    def rc_hpf_enable(self, rcHpf0Enable, rcHpf1Enable):
        regVel = self.read_register_uart(0x40003218)
        if rcHpf0Enable == 1:
            regVel &= ~(1 << 4)
        else:
            regVel |= (1 << 4)
        self.write_register_uart(0x40003218, regVel)

        regVel = self.read_register_uart(0x4000321c)
        if rcHpf1Enable == 1:
            regVel &= ~(1 << 4)
        else:
            regVel |= (1 << 4)
        self.write_register_uart(0x4000321c, regVel)

    def manu_plln_vco_sw(self, vco_sw):
        regVel = self.read_register_uart(0x40003188)
        regVel &= ~(0xf << 8)
        regVel |= vco_sw << 8
        self.write_register_uart(0x40003188, regVel)

    # ldo vdd12
    def ldo_vdd12(self):
        pass


if __name__ == '__main__':
    test_uart_wr = adt3102_wr_register()
    # test_uart_wr.uart_init()
    # test_uart_wr.ADT300_config_Uart(76.002)
    # test_uart_wr.write_register_uart(0x0000fff0, 0x00000a01)
    # print(test_uart_wr.read_register_uart(0x0000fff0))
    # print(test_uart_wr.read_register_uart(0x20008000))
    # temp = test_uart_wr.supplement_hex(0x29)
    # test_uart_wr.data_send("w20008000" + temp)
    # time.sleep(3)
    # temp = test_uart_wr.supplement_hex(0x87654321)
    # test_uart_wr.data_send("w20008000" + temp)
    # test_uart_wr.data_send("r20008000")
    # test_uart_wr.data_receive()
    # test_uart_wr.write_register_uart(0x0000fff0, 0x00000a00)
    # test_uart_wr.write_register_uart(0x0000fff0, 0x0000005a)
    # time.sleep(0.1)
    # test_uart_wr.write_register_uart(0x0000fff0, 0x00000a00)
    # time.sleep(3)
    # print(test_uart_wr.read_register_uart(0x0000fff0))
    # test_uart_wr.ADT300_config_Uart(77.005)

    # test_uart_wr.write_register_uart(0x20000000, 0x12345678)
    # rc32 = test_uart_wr.write_register_uart(0x0000fff0, 0x00000530)
    # print(rc32)
    # test_uart_wr.tia_setting_uart(1, 1, 1, 1)

    # test_uart_wr.uart_init("COM3")
    # test_uart_wr.write_register_uart(0x0000fff0, 0x0000005a)
    # time.sleep(0.5)
    # test_uart_wr.write_register_uart(0x0000fff0, 0x00000A01)
    # time.sleep(3)
    # test_uart_wr.ADT300_config_Uart(76.001)
    # test_uart_wr.uart_init("COM4")
    # test_uart_wr.write_register_uart(0x0000fff0, 0x00000A03)
    # time.sleep(3)
    # test_uart_wr.uart_init("COM3")
    #
    # iopad_9 = test_uart_wr.read_register_uart(0x20009000)
    # print(iopad_9)
    # test_uart_wr.rc_hpf_enable(1, 1)

    test_uart_wr.uart_init("COM3")
    # test_uart_wr.write_register_uart(0x0000fff0, 0x0000005a)
    # time.sleep(0.5)
    # test_uart_wr.write_register_uart(0x0000fff0, 0x00000A01)
    # time.sleep(1)
    # test_uart_wr.uart_init("COM4")
    # test_uart_wr.write_register_uart(0x0000fff0, 0x0000005a)
    # time.sleep(0.5)
    # test_uart_wr.write_register_uart(0x0000fff0, 0x00000A02)
    # time.sleep(1)
    # test_uart_wr.write_register_uart(0x0000fff0, 0x00000A04)
    # time.sleep(5)
    # det_index = test_uart_wr.read_register_uart(0x20009000)
    # dut_freq = det_index * (125 / 15 / 512)
    # print(dut_freq, "MHz")
    # test_uart_wr.uart_init("COM4")
    # test_uart_wr.write_register_uart(0x0000fff0, 0x00000A02)
    # test_uart_wr.manu_plln_vco_sw(8)
    # test_uart_wr.ADT300_config_Uart(80)
    test_uart_wr.write_register_uart(0x0000fff0, 0x00000425 + 1)
