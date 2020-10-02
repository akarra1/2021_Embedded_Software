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

    func = raw_args.function 
    f_data = df #assignment in the event that the user chose raw data

    if(func == 'avg'):
        f_data = das.avg(df)
    elif(func == 'relmax'):     #realtive maximum
        f_data = das.relmax(df)  
    elif(func == 'relmin'):
        f_data = das.relmin(df)  
    elif(func == 'absmax'):
        f_data = das.absExtremum(df, "max")
    elif(func == 'absmin'):
        f_data = das.absExtremum(df, "min")

    if(raw_args.sensor == "accel"):
        das.plotGraph(df, "Gs", f_data)        #g force
    elif(raw_args.sensor == 'gyro'):    
        das.plotGraph(f_data, "Dps")       #degrees per second
    elif(raw_args.sensor == "temp"):    
        das.plotGraph(f_data, "Fahrenheit")         #fahrenheit
    elif(raw_args.sensor == "wheelspeed"):
        das.plotGraph(f_data, "mph")       #miles per hour

if __name__ == '__main__':
    main()