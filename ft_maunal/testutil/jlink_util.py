import pylink


class jlink_util(pylink.JLink):
    def __init__(self, *args, **kwargs):
        pylink.JLink.__init__(self, *args, **kwargs)
        pass

    def jlink_connect(self):
        self.open()
        self.set_tif(pylink.enums.JLinkInterfaces.SWD)
        self.connect('Cortex-M3')
        self.set_speed(50000)

    def read_memory(self, start_address, read_num):
        # t = time.time()
        # t1 = int(round(t * 1000))
        memory_value = self.memory_read32(start_address, read_num)
        # t = time.time()
        # t2 = int(round(t * 1000))
        # print('读取内存耗时为：%d ms' % (t2 - t1))
        return memory_value

    def write_memory(self, start_address, write_data):
        value = [write_data]
        self.memory_write32(start_address, value)


if __name__ == '__main__':
    pass
