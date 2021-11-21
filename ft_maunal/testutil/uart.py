import time
import serial
import serial.tools.list_ports


class uartutil:
    def __init__(self,  *args, **kwargs):
        # super().__init__()
        self.ser = serial.Serial(*args, **kwargs)

    @staticmethod
    def port_check():
        # 检测所有存在的串口，将信息存储在字典中
        Com_Dict = {}
        port_list = list(serial.tools.list_ports.comports())
        for port in port_list:
            Com_Dict["%s" % port[0]] = "%s" % port[1]
        if len(Com_Dict) == 0:
            print(" no com")
        # print("[0]com list", self.Com_Dict)
        return Com_Dict

    # 打开串口
    def port_open(self, input_com_list, com_num):
        current_com = ""
        # com_list = self.port_check()
        for i in input_com_list:
            com_ch340 = input_com_list[i].find(com_num)
            if com_ch340 != -1:
                current_com = i
                break
        self.ser.port = current_com
        self.ser.baudrate = 115200
        self.ser.bytesize = 8
        self.ser.stopbits = 1
        # noinspection PyBroadException
        try:
            self.ser.open()
        except:
            print("Port Error", "此串口不能被打开！")
            return None

        if self.ser.isOpen():
            print("串口状态（已开启）")

    # 关闭串口
    def port_close(self):
        # noinspection PyBroadException
        try:
            self.ser.close()
            print("串口状态（已关闭）")
        except:
            pass

    # 发送数据
    def data_send(self, send_str):
        if self.ser.isOpen():
            input_s = send_str
            if input_s != "":
                # ascii发送
                input_s = (input_s + '\r\n').encode('utf-8')
                self.ser.write(input_s)
            time.sleep(0.1)
            return self.data_receive()
        else:
            pass

    # 接收数据
    def data_receive(self):
        # noinspection PyBroadException
        try:
            num = self.ser.inWaiting()
        except:
            return None
        if num > 0:
            data = self.ser.read(num)
            # num = len(data)
            # 串口接收到的字符串为b'123',要转化成unicode字符串才能输出到窗口中去
            receive_data = data.decode('iso-8859-1')
            return receive_data
        else:
            pass

    # 接收数据
    def data_receive_waitting(self):
        while True:
            # noinspection PyBroadException
            try:
                num = self.ser.inWaiting()
            except:
                return None
            if num > 0:
                data = self.ser.read(num)
                # num = len(data)
                # 串口接收到的字符串为b'123',要转化成unicode字符串才能输出到窗口中去
                receive_data = data.decode('iso-8859-1')
                return receive_data
            else:
                pass


if __name__ == '__main__':
    test_uart = uartutil()
    current_Com_Dict = uartutil.port_check()
    print(current_Com_Dict)
    test_uart.port_open(current_Com_Dict, "COM3")
    # test_uart.data_send("w2000800012345678")
    test_uart.data_send("r20008000")
    # test_uart.data_send("uart send test")
    # test_uart_receive = test_uart.data_receive()
    # print(test_uart_receive)
