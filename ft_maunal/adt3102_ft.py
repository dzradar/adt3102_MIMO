"""
Test considerations by wuhao 2021.5.20
1, set the port number of DUT as "COM3" and master as "COM4" through the PC
2, uart control master and dut, follow document uart_control_adt3102.xlsx
3, When the master runs the test case, do not reset the dut, which may cause
   the clock of the master to break and connot continue to control the master,
   You should reset the master first and then reset the DUT.
4,
"""
from datetime import datetime
import math
import os
import xlsxwriter
import time
import pyvisa as visa
from testutil.adt3102_wr_register import adt3102_wr_register
from testutil.save_result import data_record
from testutil.usbgpib import UsbGpib
from testutil.jlink_util import jlink_util
# from matplotlib import pyplot as plt

now_time = datetime.now().strftime("%Y%m%d-%H%M%S")
current_path = os.path.abspath(".")
excel_work = xlsxwriter.Workbook(current_path + "/adt3102_small_batch_final_test_report_" + now_time + ".xls")
SAMPLE_COUNT = 50000
CURRENT_RANGE_IN_AMP = 3
SAMPLE_RATE = 500000


class adt3102_ft(adt3102_wr_register, data_record):
    def __init__(self, *args, **kwargs):
        # super().__init__()
        jlink_util.__init__(self, *args, **kwargs)
        adt3102_wr_register.__init__(self, *args, **kwargs)
        data_record.__init__(self, *args, **kwargs)
        # noinspection PyBroadException
        try:
            MSO_X3104T_DEVICE_PORT = self.find_device("1770")
            self.MSO_X3104T = UsbGpib(MSO_X3104T_DEVICE_PORT)
            self.MSO_X3104T.write(f":MEASure:SOURce CHANnel{1}")
            self.MSO_X3104T.write(f":MEASure:SOURce CHANnel{2}")
            self.MSO_X3104T.write(f":MEASure:SOURce CHANnel{3}")
            self.MSO_X3104T.write(f":MEASure:SOURce CHANnel{4}")
        except:
            print("connot find drive MSO_X3104T")
        # noinspection PyBroadException
        try:
            RIGOL_DS2202E_DEVICE_PORT = self.find_device("04B0")
            self.RIGOL_DS2202E = UsbGpib(RIGOL_DS2202E_DEVICE_PORT)
            self.RIGOL_DS2202E.write(f":MEASure:SOURce CHANnel{1}")
            self.RIGOL_DS2202E.write(f":MEASure:SOURce CHANnel{2}")
        except:
            print("connot find drive RIGOL_DS2202E")
        # noinspection PyBroadException
        try:
            DMM6500_DEVICE_PORT = self.find_device("6500")
            self.DMM6500 = UsbGpib(DMM6500_DEVICE_PORT)
            self.DMM6500.write("reset()")
        except:
            print("connot find drive DMM6500")
        # noinspection PyBroadException
        try:
            self.uart_init("COM3")
            self.write_register_uart(0x0000fff0, 0x0000005a)
        except:
            print("connot find COM3")
        self.excel_init()

    def init_test(self):
        pass

    def mbist(self):
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.5)
        self.write_register_uart(0x0000fff0, 0x00000A01)
        time.sleep(1)
        self.uart_init("COM4")
        self.write_register_uart(0x0000fff0, 0x00000A03)
        time.sleep(1)
        iopad_9 = self.read_register_uart(0x4002002c)
        print(iopad_9)

    @staticmethod
    def find_device(device_id):
        # 1. find all resources
        FIND_DEVICE = ""
        rm = visa.ResourceManager()
        list_sour = rm.list_resources()
        for i in list_sour:
            device_exit = i.find(device_id)
            if device_exit != -1:
                FIND_DEVICE = i
                print("Current device is {}".format(i))
                break
            else:
                pass
                # print("device not exit")
        # print(rm.list_resources())
        # 2. check communication with the device
        # measurement = DEVICE.query(':FETCh?')
        CURRENT_DEVICE = rm.open_resource(FIND_DEVICE)
        # measurement = CURRENT_DEVICE.query('*IDN?')
        # print(measurement)
        CURRENT_DEVICE.close()
        # rm.close()
        return FIND_DEVICE

    def uart_connect(self):
        self.write_register_uart(0x20008000, 0x5a5a5a5a)
        uart_connect_flag = self.read_register_uart(0x20008000)
        if uart_connect_flag == 0x5a5a5a5a:
            self.digital_mode_sheet.write(0, 1, "uart connect normal")
            # print("uart connect normal")

    def sweep_plln_freq(self):
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.5)
        self.write_register_uart(0x0000fff0, 0x00000721)
        time.sleep(2)
        lock_plli_1g = self.read_register_uart(0x20008004)
        lock_75G = self.read_register_uart(0x20008008)
        lock_76G = self.read_register_uart(0x20009004)
        lock_77G = self.read_register_uart(0x20009008)
        lock_78G = self.read_register_uart(0x2000900c)
        lock_79G = self.read_register_uart(0x20009010)
        lock_80G = self.read_register_uart(0x20009014)
        lock_81G = self.read_register_uart(0x20009018)
        vco_sw = (self.read_register_uart(0x40003188) & (0xf << 8)) >> 8
        # print(lock_76G, lock_77G, lock_78G, lock_79G, lock_80G, lock_81G)
        self.pll_mode_sheet.write(2, 1, lock_plli_1g)
        self.pll_mode_sheet.write(4, 1, lock_75G)
        self.pll_mode_sheet.write(5, 1, lock_76G)
        self.pll_mode_sheet.write(6, 1, lock_77G)
        self.pll_mode_sheet.write(7, 1, lock_78G)
        self.pll_mode_sheet.write(8, 1, lock_79G)
        self.pll_mode_sheet.write(9, 1, lock_80G)
        self.pll_mode_sheet.write(10, 1, lock_81G)
        self.pll_mode_sheet.write(11, 1, vco_sw)
        time.sleep(1)

    def dut_adc_test(self):
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.05)
        self.write_register_uart(0x0000fff0, 0x00000A01)
        time.sleep(2)
        self.uart_init("COM4")
        self.write_register_uart(0x0000fff0, 0x00000A02)
        time.sleep(2)
        self.uart_init("COM3")
        self.write_register_uart(0x0000fff0, 0x00000A04)
        time.sleep(6)
        det_index = self.read_register_uart(0x20009000)
        dut_freq = det_index * (125 / 15 / 512)
        # print(dut_freq, "MHz")
        self.trx1_sheet.write(3, 1, str(dut_freq) + "MHz")
        adc_signal = math.log10(self.read_register_uart(0x2001A000))
        print(adc_signal)
        noise = math.log10(self.read_register_uart(0x2001A010))
        self.uart_init("COM4")
        print(noise)
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.1)

    def master_adc_test(self):
        self.uart_init("COM3")
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.5)
        self.write_register_uart(0x0000fff0, 0x00000A01)
        time.sleep(2)
        self.uart_init("COM4")
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(2)
        self.write_register_uart(0x0000fff0, 0x00000A02)
        time.sleep(2)
        self.write_register_uart(0x0000fff0, 0x00000A04)
        time.sleep(2)
        det_index = self.read_register_uart(0x20009000)
        dut_freq = det_index * (125 / 15 / 512)
        # print(dut_freq, "MHz")
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.5)
        self.trx1_sheet.write(4, 1, str(dut_freq) + "MHz")

    def power_detector(self):
        self.uart_init("COM3")
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.05)
        self.write_register_uart(0x0000fff0, 0x00000816)
        self.write_register_uart(0x0000fff0, 0x00000815)
        pa0_pd = self.read_register_uart(0x20009030)
        pa1_pd = self.read_register_uart(0x2000902c)
        # print(pa0_pd, pa1_pd)
        self.trx1_sheet.write(6, 1, pa0_pd)
        self.trx1_sheet.write(7, 1, pa1_pd)

    def master_dut_test_lpf_hpf(self):
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.5)
        self.write_register_uart(0x0000fff0, 0x00000A01)
        time.sleep(1)
        self.uart_init("COM4")
        self.pa_bias(14, 14)
        self.write_register_uart(0x0000fff0, 0x00000A02)
        time.sleep(1)
        self.uart_init("COM3")
        self.pa_enable(0, 0)
        self.pa_bias(0, 0)
        self.rc_hpf_enable(1, 1)
        self.tia_setting_uart(1, 1, 1, 1)
        self.pga_setting_uart(0, 0, 0, 0)
        if_signal_list = [76.0002, 76.0005, 76.001, 76.002, 76.004, 76.008]
        # if_signal_freq_list = [200000, 500000, 1000000, 2000000, 4000000, 8000000]
        # if_signal_rx0_i_vpp_list = list()
        # if_signal_rx0_q_vpp_list = list()
        # if_signal_rx1_i_vpp_list = list()
        # if_signal_rx1_q_vpp_list = list()
        column_index = 0
        for if_signal in if_signal_list:
            self.ADT300_config_Uart(if_signal)
            self.MSO_X3104T.write(f":AUTOscale")
            time.sleep(0.1)
            self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{1}")
            time.sleep(0.05)
            vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
            self.trx2_sheet.write(4, 1+column_index, (float("%.5f" % float(vpp))))
            # if_signal_rx0_i_vpp_list.append(float(vpp))
            self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{2}")
            time.sleep(0.05)
            vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
            self.trx2_sheet.write(5, 1 + column_index, (float("%.5f" % float(vpp))))
            # if_signal_rx0_q_vpp_list.append(float(vpp))
            self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{3}")
            time.sleep(0.05)
            vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
            self.trx2_sheet.write(6, 1 + column_index, (float("%.5f" % float(vpp))))
            # if_signal_rx1_i_vpp_list.append(float(vpp))
            self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{4}")
            time.sleep(0.05)
            vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
            self.trx2_sheet.write(7, 1 + column_index, (float("%.5f" % float(vpp))))
            # if_signal_rx1_q_vpp_list.append(float(vpp))
            column_index = column_index+1
        # plt.figure()
        # plt.title("LPF2M, RC_HPF_400kHz")
        # div_x = [*map(lambda x: math.log10(x), if_signal_freq_list)]
        # plt.plot(div_x, if_signal_rx0_i_vpp_list, label="rx0_i")
        # plt.plot(div_x, if_signal_rx0_q_vpp_list, label="rx0_q")
        # plt.plot(div_x, if_signal_rx1_i_vpp_list, label="rx1_i")
        # plt.plot(div_x, if_signal_rx1_q_vpp_list, label="rx1_q")
        # plt.legend()
        # plt.ylabel("GAIN/db")
        # plt.xlabel("frequency/Hz")
        # plt.savefig(current_path + "/master_dut_test_lpf_hpf.png")
        # self.book_sheet_ms.insert_image(4, 0, current_path + "/master_dut_test_lpf_hpf.png")
        # excel_work.close()
        # plt.show()
        # return if_signal_rx0_i_vpp_list, if_signal_rx0_i_vpp_list, if_signal_rx1_i_vpp_list, if_signal_rx1_q_vpp_list

    def rx_tia_test(self):
        self.pa_enable(0, 0)
        # vpp_value_list_tia0_i = list()
        # vpp_value_list_tia0_q = list()
        # vpp_value_list_tia1_i = list()
        # vpp_value_list_tia1_q = list()
        self.ADT300_config_Uart(76.002)
        self.pga_setting_uart(0, 0, 0, 0)
        for rx_pga_test_i in range(0, 4):
            self.tia_setting_uart(rx_pga_test_i, rx_pga_test_i, rx_pga_test_i, rx_pga_test_i)
            self.MSO_X3104T.write(f":AUTOscale")
            time.sleep(0.1)
            self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{1}")
            time.sleep(0.05)
            vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
            self.trx2_sheet.write(16, 1, (float("%.5f" % float(vpp))))
            # vpp_value_list_tia0_i.append(float(vpp))
            self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{2}")
            time.sleep(0.05)
            vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
            self.trx2_sheet.write(17, 1, (float("%.5f" % float(vpp))))
            # vpp_value_list_tia0_q.append(float(vpp))
            self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{3}")
            time.sleep(0.05)
            vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
            self.trx2_sheet.write(18, 1, (float("%.5f" % float(vpp))))
            # vpp_value_list_tia1_i.append(float(vpp))
            self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{4}")
            time.sleep(0.05)
            vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
            self.trx2_sheet.write(19, 1, (float("%.5f" % float(vpp))))
            # vpp_value_list_tia1_q.append(float(vpp))
        # plt.figure()
        # plt.plot(vpp_value_list_tia0_i, label="rx0_i")
        # plt.plot(vpp_value_list_tia0_q, label="rx0_q")
        # plt.plot(vpp_value_list_tia1_i, label="rx1_i")
        # plt.plot(vpp_value_list_tia1_q, label="rx1_q")
        # plt.legend()
        # plt.xlabel("tia_step/6db")
        # plt.ylabel("tia_gain/db")
        # plt.title("tia gain test")
        # plt.savefig(current_path + "/tia_gain_test.png")
        # for j in range(0, len(vpp_value_list_tia0_i)):
        #     book_sheet.write(0 + 1, j + 1, vpp_value_list_tia0_i[j])
        #     book_sheet.write(1 + 1, j + 1, vpp_value_list_tia0_q[j])
        #     book_sheet.write(2 + 1, j + 1, vpp_value_list_tia1_i[j])
        #     book_sheet.write(3 + 1, j + 1, vpp_value_list_tia1_q[j])
        # book_sheet.insert_image(5, 0, current_path + "/tia_gain_test.png")
        # excel_work.close()
        # plt.show()
        # return vpp_value_list_tia0_i, vpp_value_list_tia0_q, vpp_value_list_tia1_i, vpp_value_list_tia1_q

    def rx_pga_test(self):
        # vpp_list_pga0_i = list()
        # vpp_list_pga0_q = list()
        # vpp_list_pga1_i = list()
        # vpp_list_pga1_q = list()
        self.ADT300_config_Uart(76.002)
        self.tia_setting_uart(0, 0, 0, 0)
        time.sleep(1)
        for rx_pga_test_i in range(0, 16):
            self.pga_setting_uart(rx_pga_test_i, rx_pga_test_i, rx_pga_test_i, rx_pga_test_i)
            self.MSO_X3104T.write(f":AUTOscale")
            time.sleep(0.1)
            self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{1}")
            time.sleep(0.05)
            vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
            self.trx2_sheet.write(10, 1+rx_pga_test_i, (float("%.5f" % float(vpp))))
            # vpp_list_pga0_i.append(float(vpp))
            self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{2}")
            time.sleep(0.05)
            vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
            self.trx2_sheet.write(11, 1 + rx_pga_test_i, (float("%.5f" % float(vpp))))
            # vpp_list_pga0_q.append(float(vpp))
            self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{3}")
            time.sleep(0.05)
            vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
            self.trx2_sheet.write(12, 1 + rx_pga_test_i, (float("%.5f" % float(vpp))))
            # vpp_list_pga1_i.append(float(vpp))
            self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{4}")
            time.sleep(0.05)
            vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
            self.trx2_sheet.write(13, 1 + rx_pga_test_i, (float("%.5f" % float(vpp))))
            # vpp_list_pga1_q.append(float(vpp))
        # plt.figure()
        # plt.plot(vpp_list_pga0_i, label="pga0")
        # plt.plot(vpp_list_pga0_q, label="pga1")
        # plt.plot(vpp_list_pga1_i, label="pga0")
        # plt.plot(vpp_list_pga1_q, label="pga1")
        # plt.legend()
        # plt.xlabel("pga_step/2db")
        # plt.ylabel("pga_gain/db")
        # plt.title("pga gain test")
        # plt.savefig(current_path + "/pga_gain_test.png")
        # for j in range(0, len(vpp_list_pga0_i)):
        #     book_sheet.write(1, j, vpp_list_pga0_i[j])
        #     book_sheet.write(2, j, vpp_list_pga0_q[j])
        #     book_sheet.write(3, j, vpp_list_pga1_i[j])
        #     book_sheet.write(4, j, vpp_list_pga1_q[j])
        # book_sheet.insert_image(5, 0, current_path + "/pga_gain_test.png")
        self.uart_init("COM4")
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.05)
        # excel_work.close()
        # plt.show()
        # return vpp_list_pga0_i, vpp_list_pga0_q, vpp_list_pga1_i, vpp_list_pga1_q

    def master_dut_test_80G(self):
        self.uart_init("COM3")
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.5)
        self.write_register_uart(0x0000fff0, 0x00000A01)
        time.sleep(1)
        self.ADT300_config_Uart(80)
        self.manu_plln_vco_sw(8)
        self.uart_init("COM4")
        self.write_register_uart(0x0000fff0, 0x00000A02)
        self.ADT300_config_Uart(80.002)
        self.manu_plln_vco_sw(8)
        self.MSO_X3104T.write(f":AUTOscale")
        time.sleep(1)
        self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{1}")
        time.sleep(0.05)
        vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
        vpp_80 = float(vpp)
        self.trx3_sheet.write(3, 2, vpp_80)
        qresult = self.MSO_X3104T.query(f":MEASure:FREQuency? CHANnel{1}")
        time.sleep(0.05)
        freq = float(qresult) / 1e6
        freq_80 = freq
        self.trx3_sheet.write(3, 1, freq_80)
        self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{2}")
        time.sleep(0.05)
        vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
        vpp_80 = float(vpp)
        self.trx3_sheet.write(4, 2, vpp_80)
        qresult = self.MSO_X3104T.query(f":MEASure:FREQuency? CHANnel{2}")
        time.sleep(0.05)
        freq = float(qresult) / 1e6
        freq_80 = freq
        self.trx3_sheet.write(4, 1, freq_80)
        self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{3}")
        time.sleep(0.05)
        vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
        vpp_80 = float(vpp)
        self.trx3_sheet.write(4, 2, vpp_80)
        qresult = self.MSO_X3104T.query(f":MEASure:FREQuency? CHANnel{3}")
        time.sleep(0.05)
        freq = float(qresult) / 1e6
        freq_80 = freq
        self.trx3_sheet.write(4, 1, freq_80)
        self.write_register_uart(0x0000fff0, 0x0000005a)
        self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{4}")
        time.sleep(0.05)
        vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
        vpp_80 = float(vpp)
        self.trx3_sheet.write(5, 2, vpp_80)
        qresult = self.MSO_X3104T.query(f":MEASure:FREQuency? CHANnel{4}")
        time.sleep(0.05)
        freq = float(qresult) / 1e6
        freq_80 = freq
        self.trx3_sheet.write(5, 1, freq_80)
        # print(vpp_80, freq_80)

    def rc_32kHz(self):
        self.uart_init("COM3")
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.05)
        rc32 = self.write_register_uart(0x0000fff0, 0x00000530)
        # # rc32 = self.data_receive_while()
        # print(rc32)
        pmu_pmu_state = rc32[-8:-2]
        k32_prd = int(pmu_pmu_state, 16)
        f_32k = 64 / k32_prd * 50000 / 4
        self.pll_mode_sheet.write(0, 1, str(f_32k) + "kHz")

    def bist_rf_test(self):
        self.uart_init("COM3")
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.1)
        self.write_register_uart(0x0000fff0, 0x00000A00)
        time.sleep(3)
        self.tia_setting_uart(1, 1, 1, 1)
        self.pga_setting_uart(10, 10, 10, 10)
        self.pga_band_width_uart(1, 1)
        # vpp_list_vpp_pga0 = list()
        # vpp_list_vpp_pga1 = list()
        # vpp_list_freq_pga0 = list()
        # vpp_list_freq_pga1 = list()
        # bistdiv_list = [0x15, 0x0A, 0x06, 0x09, 0x02]
        bistdiv_list = [0x06]
        div_num = 0
        for fttest_pag_bandwidth_i in bistdiv_list:
            self.bistdiv_setting_uart(fttest_pag_bandwidth_i)
            self.MSO_X3104T.write(f":AUTOscale")
            time.sleep(0.1)
            self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{1}")
            time.sleep(0.05)
            vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
            self.bist_mode_sheet.write(2, 1, vpp)
            # vpp_list_vpp_pga0.append(float(vpp))
            qresult = self.MSO_X3104T.query(f":MEASure:FREQuency? CHANnel{1}")
            time.sleep(0.05)
            freq = float(qresult) / 1e6
            self.bist_mode_sheet.write(1, 1, freq)
            # vpp_list_freq_pga0.append(freq)
            self.MSO_X3104T.write(f":FFT:SOURce1 CHANnel{3}")
            time.sleep(0.05)
            vpp = self.MSO_X3104T.query(":MEASure:VMAX? FFT")
            self.bist_mode_sheet.write(1, 2, vpp)
            # vpp_list_vpp_pga1.append(float(vpp))
            qresult = self.MSO_X3104T.query(f":MEASure:FREQuency? CHANnel{3}")
            time.sleep(0.05)
            freq = float(qresult) / 1e6
            self.bist_mode_sheet.write(2, 2, freq)
            # vpp_list_freq_pga1.append(freq)
            div_num = div_num + 1
        # plt.figure()
        # plt.title("BIST")
        # div = range(0, 5)
        # div_x = [*map(lambda x: math.log10(50000000 / pow(2, (10 - x))), div)]
        # plt.plot(div_x, vpp_list_vpp_pga0, label="pga0")
        # plt.plot(div_x, vpp_list_vpp_pga1, label="pga1")
        # plt.legend()
        # plt.ylabel("GAIN/db")
        # plt.xlabel("frequency/Hz")
        # plt.savefig("BIST.png")
        # plt.show()
        # for j in range(0, len(vpp_list_vpp_pga1)):
        #     book_sheet.write(0, j, vpp_list_vpp_pga0[j])
        #     book_sheet.write(1, j, vpp_list_vpp_pga1[j])
        #     book_sheet.write(2, j, vpp_list_freq_pga0[j])
        #     book_sheet.write(3, j, vpp_list_freq_pga1[j])
        # book_sheet.insert_image(4, 0, current_path + "/BIST.png")
        # # excel_work.close()
        # return vpp_list_vpp_pga0, vpp_list_vpp_pga1, vpp_list_freq_pga0, vpp_list_freq_pga1

    def plli_plln_ldo_test(self):
        self.uart_init("COM3")
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.5)
        plli_pll_ldo_list = list()
        plli_vco_ldo_list = list()
        plln_pll_ldo_list = list()
        plln_vco_ldo_list = list()
        self.write_register_uart(0x0000fff0, 0x00000231)
        self.DMM6500.write("dmm.measure.func = dmm.FUNC_DC_VOLTAGE")
        self.DMM6500.write("dmm.measure.autorange = dmm.ON")
        for vpp_index in range(0, 4):
            self.write_register_uart(0x0000fff0, 0x00000231+vpp_index)
            self.write_register_uart(0x0000fff0, 0x00000235+vpp_index)
            self.write_register_uart(0x0000fff0, 0x00000421+vpp_index)
            self.write_register_uart(0x0000fff0, 0x00000425+vpp_index)
            time.sleep(1)
            vpp = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))
            plli_pll_ldo_list.append((float(vpp)))
            self.ldo_dc_test_sheet.write(1, 1 + vpp_index, (float("%.5f" % float(vpp))))
            vpp = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))
            plli_vco_ldo_list.append((float(vpp)))
            self.ldo_dc_test_sheet.write(2, 1 + vpp_index, (float("%.5f" % float(vpp))))
            vpp = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))
            plln_pll_ldo_list.append((float(vpp)))
            self.ldo_dc_test_sheet.write(3, 1 + vpp_index, (float("%.5f" % float(vpp))))
            vpp = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))
            plln_vco_ldo_list.append((float(vpp)))
            self.ldo_dc_test_sheet.write(4, 1 + vpp_index, (float("%.5f" % float(vpp))))
        excel_work.close()
        # print(plli_pll_ldo_list, plli_vco_ldo_list, plln_pll_ldo_list, plln_vco_ldo_list)

    def bb_vdd12_aio1_test(self):
        self.uart_init("COM3")
        self.DMM6500.write("dmm.measure.func = dmm.FUNC_DC_VOLTAGE")
        self.DMM6500.write("dmm.measure.autorange = dmm.ON")
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.5)
        bb0_ldo_list = list()
        bb1_ldo_list = list()
        vdd12_ldo_list = list()
        for vpp_index in range(0, 4):
            self.write_register_uart(0x0000fff0, 0x00000431 + vpp_index)
            self.write_register_uart(0x0000fff0, 0x00000221 + vpp_index)
            time.sleep(1)
            vpp = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))
            self.ldo_dc_test_sheet.write(6, 1+vpp_index, (float("%.5f" % float(vpp))))
            bb0_ldo_list.append(float(vpp))
            vpp = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))
            self.ldo_dc_test_sheet.write(7, 1+vpp_index, (float("%.5f" % float(vpp))))
            bb1_ldo_list.append(float(vpp))
            vpp = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))
            self.ldo_dc_test_sheet.write(8, 1+vpp_index, (float("%.5f" % float(vpp))))
            vdd12_ldo_list.append(float(vpp))

        # for vpp_index in range(0, 6):
        #     self.write_register_uart(0x0000fff0, 0x0000005a)
        #     time.sleep(1)
        #     self.write_register_uart(0x0000fff0, 0x00000921 + vpp_index)
        #     self.MSO_X3104T.write(f":AUTOscale")
        #     self.MSO_X3104T.write(f":CHANnel{4}:SCALe 800mv")
        #     time.sleep(4)
        #     vpp = self.MSO_X3104T.query(f":MEASure:VAVerage? DISPlay,CHANnel{4}")
        #     time.sleep(3)
        #     aio1_ldo_list.append((float(vpp)))
        #     self.book_sheet_dc.write(6, 1 + vpp_index, (float(vpp)))
        # print(bb0_ldo_list, bb1_ldo_list, vdd12_ldo_list)

    def pll_current(self):
        self.DMM6500.write("dmm.measure.func = dmm.FUNC_DC_CURRENT")
        self.DMM6500.write("dmm.measure.autorange = dmm.ON")
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.5)
        self.write_register_uart(0x0000fff0, 0x00000540)
        plli_lock_current = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000
        self.write_register_uart(0x0000fff0, 0x00000551)
        plln_lock_current = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000
        plln_lock_current_temp = plln_lock_current
        self.write_register_uart(0x0000fff0, 0x00000552)
        plln_block_current = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000
        plln_block_current_temp = plln_block_current
        plln_block_current = plln_lock_current_temp - plln_block_current
        self.write_register_uart(0x0000fff0, 0x00000553)
        plln_vco_buff_current = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000
        plln_vco_buff_current_temp = plln_vco_buff_current
        plln_vco_buff_current = plln_block_current_temp - plln_vco_buff_current
        self.write_register_uart(0x0000fff0, 0x00000554)
        plln_vco_ldo_current = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000
        plln_vco_ldo_current_temp = plln_vco_ldo_current
        plln_vco_ldo_current = plln_vco_buff_current_temp - plln_vco_ldo_current
        self.write_register_uart(0x0000fff0, 0x00000555)
        plln_pll_ldo_current = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000
        plln_pll_ldo_current = plln_vco_ldo_current_temp - plln_pll_ldo_current
        self.block_current_sheet.write(1, 1, float("%.5f" % plli_lock_current))
        self.block_current_sheet.write(2, 1, float("%.5f" % plln_block_current))
        self.block_current_sheet.write(3, 1, float("%.5f" % plln_vco_buff_current))
        self.block_current_sheet.write(4, 1, float("%.5f" % plln_vco_ldo_current))
        self.block_current_sheet.write(5, 1, float("%.5f" % plln_pll_ldo_current))
        # print(plli_lock_current, plln_block_current, plln_vco_buff_current, plln_vco_ldo_current,
        # plln_pll_ldo_current)

    def vddrf_current(self, store_flag):
        self.DMM6500.write("dmm.measure.func = dmm.FUNC_DC_CURRENT")
        self.DMM6500.write("dmm.measure.autorange = dmm.ON")
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.5)
        self.write_register_uart(0x0000fff0, 0x00000610)
        rf_basic = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000
        self.write_register_uart(0x0000fff0, 0x00000611)
        time.sleep(0.5)
        rf_20g = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000
        rf_20g_temp = rf_20g
        rf_20g = rf_20g - rf_basic
        self.write_register_uart(0x0000fff0, 0x00000612)
        time.sleep(0.5)
        rf_doubler = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000
        rf_doubler_temp = rf_20g_temp
        rf_doubler = rf_doubler - rf_20g_temp
        self.write_register_uart(0x0000fff0, 0x00000613)
        time.sleep(0.5)
        rf_1to4 = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000
        rf_1to4_temp = rf_1to4
        rf_1to4 = rf_1to4 - rf_doubler_temp
        self.write_register_uart(0x0000fff0, 0x00000614)
        time.sleep(0.5)
        rf_pa = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000
        rf_pa_temp = rf_pa
        rf_pa = (rf_pa - rf_1to4_temp)/2
        self.write_register_uart(0x0000fff0, 0x00000615)
        time.sleep(0.5)
        rf_lna = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000
        rf_lna_temp = rf_lna
        rf_lna = (rf_lna - rf_pa_temp)/2
        self.block_current_sheet.write(7,  1, float("%.5f" % rf_20g))
        self.block_current_sheet.write(8,  1, float("%.5f" % rf_doubler))
        self.block_current_sheet.write(9,  1, float("%.5f" % rf_1to4))
        self.block_current_sheet.write(10, 1, float("%.5f" % rf_pa))
        self.block_current_sheet.write(11, 1, float("%.5f" % rf_pa))
        self.block_current_sheet.write(12, 1, float("%.5f" % rf_lna))
        self.block_current_sheet.write(13, 1, float("%.5f" % rf_lna))
        if store_flag == 1:
            print(float("%.3f" % rf_20g), float("%.3f" % rf_doubler))
            print(float("%.3f" % rf_1to4), float("%.3f" % rf_pa), float("%.3f" % rf_lna))
        else:
            pass
        return rf_lna_temp

    def baseband_current(self):
        self.DMM6500.write("dmm.measure.func = dmm.FUNC_DC_CURRENT")
        self.DMM6500.write("dmm.measure.autorange = dmm.ON")
        rf_lna_temp = self.vddrf_current(0)
        self.write_register_uart(0x0000fff0, 0x00000621)
        time.sleep(0.5)
        rf_tia = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000
        rf_tia_temp = rf_tia
        rf_tia = (rf_tia - rf_lna_temp) / 2
        self.write_register_uart(0x0000fff0, 0x00000622)
        time.sleep(0.5)
        rf_pga = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000
        rf_pga_temp = rf_pga
        rf_pga = (rf_pga - rf_tia_temp) / 2
        self.write_register_uart(0x0000fff0, 0x00000623)
        time.sleep(0.5)
        rf_adc = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000
        rf_adc = (rf_adc - rf_pga_temp) / 2
        self.block_current_sheet.write(15, 1, float("%.5f" % rf_tia))
        self.block_current_sheet.write(18, 1, float("%.5f" % rf_tia))
        self.block_current_sheet.write(16, 1, float("%.5f" % rf_pga))
        self.block_current_sheet.write(19, 1, float("%.5f" % rf_pga))
        self.block_current_sheet.write(17, 1, float("%.5f" % rf_adc))
        self.block_current_sheet.write(20, 1, float("%.5f" % rf_adc))
        # print(float("%.3f" % rf_tia), float("%.3f" % rf_pga), float("%.3f" % rf_adc))

    def aio1_voltage(self):
        self.DMM6500.write("dmm.measure.func = dmm.FUNC_DC_VOLTAGE")
        self.DMM6500.write("dmm.measure.autorange = dmm.ON")
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.5)
        self.write_register_uart(0x0000fff0, 0x00000441)
        aio1_vbe = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))
        self.aio1_dc_sheet.write(1, 1, float("%.5f" % aio1_vbe))
        self.write_register_uart(0x0000fff0, 0x00000442)
        aio1_vbg = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))
        self.aio1_dc_sheet.write(2, 1, float("%.5f" % aio1_vbg))
        for vpp_index in range(0, 6):
            self.write_register_uart(0x0000fff0, 0x0000005a)
            time.sleep(0.5)
            self.write_register_uart(0x0000fff0, 0x00000921 + vpp_index)
            time.sleep(1)
            vpp = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))
            self.aio1_dc_sheet.write(3+vpp_index, 1, (float("%.5f" % vpp)))

    def aio1_current(self):
        self.DMM6500.write("dmm.measure.func = dmm.FUNC_DC_CURRENT")
        self.DMM6500.write("dmm.measure.autorange = dmm.ON")
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.1)
        self.write_register_uart(0x0000fff0, 0x00000541)
        plli_dac12_reg = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000000
        self.aio1_dc_sheet.write(10, 1, (float("%.5f" % plli_dac12_reg)))
        self.write_register_uart(0x0000fff0, 0x00000542)
        plli_reg2_en_vco_itest = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000000
        self.aio1_dc_sheet.write(11, 1, (float("%.5f" % plli_reg2_en_vco_itest)))
        self.write_register_uart(0x0000fff0, 0x00000556)
        plln_dac12_reg = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000000
        self.aio1_dc_sheet.write(12, 1, (float("%.5f" % plln_dac12_reg)))
        self.write_register_uart(0x0000fff0, 0x00000557)
        plln_reg2_en_vco_itest = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000000
        self.aio1_dc_sheet.write(13, 1, (float("%.5f" % plln_reg2_en_vco_itest)))
        self.write_register_uart(0x0000fff0, 0x0000005a)
        time.sleep(0.1)
        self.write_register_uart(0x0000fff0, 0x00000630)
        bias2_din19 = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000000
        self.aio1_dc_sheet.write(15, 1, (float("%.5f" % bias2_din19)))
        self.write_register_uart(0x0000fff0, 0x00000443)
        bias1_din24 = float(self.DMM6500.query("print(dmm.measure.read())", timeout=3))*1000000
        self.aio1_dc_sheet.write(16, 1, (float("%.5f" % bias1_din24)))

    def main_test_flow(self):
        # uart connect test
        self.uart_connect()
        # Memory bist
        # self.mbist()
        # PLLN sweep range 76G~80G(19G-20G)
        self.sweep_plln_freq()
        # dut adc
        self.dut_adc_test()
        # master adc
        self.master_adc_test()
        # pa powerdector
        self.power_detector()
        # # TRx test1 DUT set Rx, Tx test from Master chip
        self.master_dut_test_lpf_hpf()
        # # # TRx test2
        self.rx_tia_test()
        self.rx_pga_test()
        # # bist rf
        self.bist_rf_test()
        # master and dut 80G
        self.master_dut_test_80G()
        # # # 32kHz osc test with internal frequency counter
        self.rc_32kHz()


if __name__ == '__main__':
    test = adt3102_ft()
    # test.main_test_flow()
    # uart connect test
    test.uart_connect()
    # Memory bist
    # self.mbist()
    # PLLN sweep range 76G~80G(19G-20G)
    # test.sweep_plln_freq()
    # dut adc
    # test.dut_adc_test()
    # master adc
    # test.master_adc_test()
    # pa powerdector
    # test.power_detector()
    # TRx test1 DUT set Rx, Tx test from Master chip
    # test.master_dut_test_lpf_hpf()
    # # # TRx test2
    # test.rx_tia_test()
    # test.rx_pga_test()
    # bist rf
    # test.bist_rf_test()
    # master and dut 80G
    # test.master_dut_test_80G()
    # # # 32kHz osc test with internal frequency counter
    # test.rc_32kHz()

    # test.plli_plln_ldo_test()
    # test.dut_adc_test()
    # test.bb_vdd12_aio1_test()
    # test.pll_current()
    # test.vddrf_current(0)
    # test.baseband_current()
    # test.plli_plln_ldo_test()
    # test.aio1_voltage()
    # test.aio1_current()
    test.excel_work.close()
