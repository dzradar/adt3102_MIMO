# Python script to communicate with Keysight U8489a Power Sensor
# Works on Rapsberry Pi
# prerequisite: sudo pip install pyusb
# git clone https://github.com/alexforencich/python-usbtmc
# sudo python setup.py install

#import csv

import usbtmc
import time

_USBPORT = 'USB::10893::12289::INSTR'

class Usb_Dev:
    """Python script to communicate with Keysight U8489a Power Sensor"""

    def __init__(self, port=_USBPORT):
        self.port = port
        self.inst = self.connect()
        self.write('SYST:PRES DEF')
        self.write("INST:CONT ON")

    def connect(self):
        return usbtmc.Instrument(self.port)

    def query(self, string):
        return self.inst.ask(string)

    def write(self, string):
        self.inst.write(string)

    def get_pow(self, sleep=1):
        time.sleep(sleep)
        return float(self.query("FETC?"))

U8489A = Usb_Dev

# For Testing under Python interpreter
if __name__ == '__main__':
    u = U8489A()
    print(u.query('*IDN?'))
    while(1):
        print(u.get_pow())