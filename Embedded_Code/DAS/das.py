# DAS script revision 1
# Written by Andrew Kettle
# Last Revision: Sept 27th, 2020

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import sys 
import argparse
import daslib as das

def main():
    #get command line args (returned as a namespace)
    raw_args = das.getargs()
    headernames = ["Accel_X", "Accel_Y", "Accel_Z", "Gyro_X", "Gyro_Y", "Gyro_Z", "IR_1", "IR_2", "IR_3", "Wheelspeed"]
    #read csv file, append headers
    df = das.getdf(raw_args.csvfile, headernames) #gets dataframe from csv file
    #filter for desired sensor
    df = das.filterSensor(df, raw_args.sensor)
    print(df)
    #df = das.filterSensor(df, raw_args[1])
    #starting with only one function at a time, will adapt later on   

if __name__ == '__main__':
    main()