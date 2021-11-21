#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
"""
util.py: includes save and load csv files utilities
"""
import csv

def save_to_csv(filename, data):
    with open(filename, 'a', newline='') as outfile:
        csvwriter = csv.writer(outfile)
        csvwriter.writerow([*data])

def save_results(filename, psr_setting, measured_results):
    with open(filename, 'a', newline='') as outfile:
        csvwriter = csv.writer(outfile)
        csvwriter.writerow([*psr_setting, *measured_results])

def load_psr_csv(filename):
    psr_sets = []
    # Retrieve the PSR settings from the csv file
    with open(filename, 'r') as infile:
        csvreader = csv.reader(infile, quoting=csv.QUOTE_NONNUMERIC)
        for line in csvreader:
            psr_sets.append(line)
    return psr_sets

def load_csv_setting(filename):
    settings = []
    with open(filename, 'r') as infile:
        csvreader = csv.reader(infile, quoting=csv.QUOTE_NONNUMERIC)
        for line in csvreader:
            settings.append(line)
    return settings

class LoadCal:
    def __init__(self, filename):
        self.filename = filename
        self.freq_fin = []
        self.freq_fout = []
        self.valid = []
        self.power_level = []
        self.pla_bias = []
        self.swp_bias = []
        self.load()

    def load(self):
        with open(filename, 'r') as infile:
            csvreader = csv.reader(infile, quoting=csv.QUOTE_NONNUMERIC)
            for line in csvreader:
                self.freq_fin.append(int(line[0]))
                self.freq_fout.append(int(line[1]))
                self.valid.append(float(line[2]))
                self.power_level.append(float(line[3]))
                self.pla_bias.append(int(line[4]))
                self.swp_bias.append(int(line[5]))

# For Testing under Python interpreter
if __name__ == '__main__':
    filename = r'../data/siggen_m40dbm_cal.csv'
    s = LoadCal(filename)
    print(s.freq_fin)
    print(s.freq_fout)
    print(s.valid)
    print(s.power_level)
    print(s.pla_bias)
    print(s.swp_bias)