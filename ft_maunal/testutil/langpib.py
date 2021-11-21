#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
"""
langpib.py: The langpib module, which defines LanGpib class and its subclasses.
"""
import socket

_rs_fsw85_host = "192.168.10.109"    # The remote host
_rs_fsw85_port = 5025              # The remote port as used by the server


class LanGpib:
    """A LanGpib instance models a LAN-GPIB instrument"""

    def __init__(self, host=_rs_fsw85_host, port=_rs_fsw85_port):
        """ connect to a LAN GPIB instrument specified ip address and port number """
        self.host = host
        self.port = port
        self.timeout = 1

    def __str__(self):
        """Return a descriptive string for this instance, invoked by print() and str()"""
        return 'This is a LAN-GPIB instrument on %s:%s' % (self.host, self.port)

    def __repr__(self):
        """Return a command string that can be used to re-create this instance, invoked by repr()"""
        return 'LanGpib(host=%s, port=%s)' % (self.host, self.port)

    def __del__(self):
        self.close()

    def close(self):
        self.socket_inst.close()

    def write(self, command):
        self.socket_inst.sendall(bytes(command + "\n", "utf8"))

    def query(self, command):
        self.socket_inst.sendall(bytes(command + "\n", "utf8"))
        self.data = self.socket_inst.recv(1024)
        return self.data.decode('utf-8')

    def connect(self):
        self.socket_inst = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket_inst.connect((self.host, self.port))
        self.socket_inst.settimeout(self.timeout)

    def print(self):
        print(self.data.decode('utf-8'))


class RsFsw85(LanGpib):
    def __init__(self, host=_rs_fsw85_host, port=_rs_fsw85_port):
        super().__init__(host, port)

    def find_peak(self, freq):
        self.write("FREQ:CENT %s MHZ" % freq)
        self.write("INIT;*WAI")
        self.write("CALC:MARK:MAX") # set marker to max. peak
##        self.write("CALC:MARK:PEAK:EXC dB")
        self.freq = self.query("CALC:MARK:X?")
        self.power = self.query("CALC:MARK:Y?")


class E4440a(LanGpib):
    def __init__(self, host=_rs_fsw85_host, port=_rs_fsw85_port):
        super().__init__(host, port)

    def find_peak(self, freq):
        self.write("FREQ:CENT %s MHZ" % freq)
        self.write("INIT;*WAI")
        self.write("CALC:MARK:MAX") # set marker to max. peak
##        self.write("CALC:MARK:PEAK:EXC dB")
        self.freq = float(self.query("CALC:MARK:X?")) # float() required
        self.power = float(self.query("CALC:MARK:Y?")) # float() required

    def find_peak(self):
        # self.write("FREQ:CENT %s MHZ")
        self.write("INIT;*WAI")
        self.write("CALC:MARK:MAX") # set marker to max. peak
##        self.write("CALC:MARK:PEAK:EXC dB")
        self.freq = float(self.query("CALC:MARK:X?")) # float() required
        self.power = float(self.query("CALC:MARK:Y?")) # float() required
N9030b = E4440a
N9020b = E4440a

class N5222b(LanGpib):
    def __str__(self):
        """Return a descriptive string for this instance, invoked by print() and str()"""
        return 'This is Keysight N5222B VNA.'

    def __init__(self, host=_rs_fsw85_host, port=_rs_fsw85_port):
        super().__init__(host, port)

class DMM6500(LanGpib):
    def __str__(self):
        """Return a descriptive string for this instance, invoked by print() and str()"""
        return 'This is Keithley DMM6500 Digital Multi Meter.'

    def __init__(self, host=_rs_fsw85_host, port=_rs_fsw85_port):
        super().__init__(host, port)
        self.timeout = 0.5

    def query(self, command, timeout=0.5):
        self.socket_inst.settimeout(timeout)
        self.socket_inst.sendall(bytes(command + "\n", "utf8"))
        self.data = self.socket_inst.recv(1024)
        self.socket_inst.settimeout(self.timeout) # return to default setting
        return self.data.decode('utf-8')

    def big_query(self, command):
        self.socket_inst.sendall(bytes(command + "\n", "utf8"))
        incomingdata = ""
        self.data  = bytes("", "utf8")

        try:
            while True:
                incomingdata = self.socket_inst.recv(65536)
                if not incomingdata:
                    return self.data.decode('utf-8')
                else:
                    self.data += incomingdata
        except:
            pass
        return self.data.decode('utf-8')

# For Testing under Python interpreter
if __name__ == '__main__':
    lan1 = LanGpib()
    print(lan1.host, lan1.port)
    lan1.connect()
    print(lan1.query("*IDN?"))
    lan2 = LanGpib(_rs_fsw85_host, _rs_fsw85_port)
    print(lan2.host, lan2.port)
    lan2.connect()
    print(lan2.query("*IDN?"))


