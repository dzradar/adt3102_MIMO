#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
"""
UsbGpib.py: The UsbGpib module, which defines a USB-GPIB class.
"""
import pyvisa
"""an example port for an instrument"""
_gpib_port = 'GPIB0::19::INSTR'

class UsbGpib:
    """A UsbGpib instance models a USB-GPIB instrument"""

    def __init__(self, port=_gpib_port):
        """ connect to a GPIB instrument specified the port """
        self.port = port
        self.connect()

    def __str__(self):
        """Return a descriptive string for this instance, invoked by print() and str()"""
        return 'This is a USB-GPIB instrument with port of %s' % self.port

    def __repr__(self):
        """Return a command string that can be used to re-create this instance, invoked by repr()"""
        return 'UsbGpib(port=%s)' % self.port

    def connect(self):
        # open instrument via USB GPIB
        rm = pyvisa.ResourceManager()
        # print(rm.list_resources())
        self.gpib_instance = rm.open_resource(self.port)
        return self.gpib_instance

    def close(self):
        self.gpib_instance.close()

    def disconnect(self):
        self.write("SYST:LOC")
        self.gpib_instance.close

    def query(self, string, timeout=25):
        self.gpib_instance.timeout = timeout * 1000
        res = self.gpib_instance.query(string)
        self.gpib_instance.timeout = 2500 * 1000
        return res

    def write(self, string):
        self.gpib_instance.write(string)


class Hp83630b(UsbGpib):
    def __init__(self, port=_gpib_port):
        """ connect to a GPIB instrument specified the port """
        super().__init__(port)

    def set_freq_mhz(self, freq):
        self.write("FREQ:CENT %s MHZ" % freq)
        self.write("*WAI")

    def set_power_dbm(self, power):
        self.write("POWER %s DBM" % power)


class Kei2231a(UsbGpib):
    def __init__(self, port='ASRL8::INSTR'):
        """ connect to a GPIB instrument specified the port """
        super().__init__(port)
        self.write("SYST:REM") # remote
        self.write("*RST") # reset

    def set_ch_volt_curr(self, ch, volt, curr):
        self.write("APPL CH%d, %f, %f" % (ch, volt, curr))

    def set_ch_state(self, channel, on_off=0):
        self.write("INST:SEL CH%d" % channel)
        self.write("CHAN:OUTP %d" % on_off)

    def set_voltage(self, volt):
        self.write("VOLT %f" % volt)

    def set_current(self, curr):
        self.write("CURR %f" % curr)

    def set_output_all(self, state):
        self.write("OUTP %d" % state)

    def get_ch_curr(self, ch):
        self.write("INST:SEL CH%d" % ch)
        return self.query("MEAS:CURR?")

# For Testing under Python interpreter
if __name__ == '__main__':
    g1 = UsbGpib()
    print(g1.port)
    print(g1.query("*IDN?"))
    g2 = Hp83630b(_gpib_port)
    print(g2.port)
    print(g2.query("*IDN?"))
