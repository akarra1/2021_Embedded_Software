# DAS script revision 1
# Written by Andrew Kettle
# Last Revision: Sept 27th, 2020

# NOTE: das.py was originally designed for the command-line version of the DAS,
# and so this file is NOT USED in the DAS web app. For code directly affecting the web app
# see app.py

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
    print(df.head(3)) #added so I can see what the df looks like (first 3 entries)
    #filter for desired sensor
    df = das.filterSensor(df, raw_args.sensor)

    func = raw_args.function 
    f_data = df #assignment in the event that the user chose raw data

    #a bunch of if elses to satisfy the function argument
    if(func == 'avg'):
        f_data = das.avg(df)
        print(f_data)
    elif(func == 'relmax'):     #realtive maximum
        f_data = das.relmax(df)  
    elif(func == 'relmin'):
        f_data = das.relmin(df)  
    elif(func == 'absmax'):
        f_data = das.absExtremum(df, "max")
    elif(func == 'absmin'):
        f_data = das.absExtremum(df, "min")

    #a bunch of if elses to satisfy the sensor argument
    #broken up becasue the raw data option doesn't need the optional arguments 

    if((raw_args.function == "raw") or (raw_args.function == "relmin") or (raw_args.function == "relmax")): #plot these
        if(raw_args.function == "raw"):             #plots raw graphs without optional arguments
            if(raw_args.sensor == "accel"):
                das.plotGraph(f_data, "Gs")         #g force
            elif(raw_args.sensor == 'gyro'):    
                das.plotGraph(f_data, "Dps")        #degrees per second
            elif(raw_args.sensor == "temp"):    
                das.plotGraph(f_data, "Fahrenheit") #fahrenheit
            elif(raw_args.sensor == "wheelspeed"):
                das.plotGraph(f_data, "mph")        #miles per hour
        else:
            if(raw_args.sensor == "accel"):
                das.plotGraph(df, "Gs", f_data)         #g force
            elif(raw_args.sensor == 'gyro'):    
                das.plotGraph(df, "Dps", f_data)        #degrees per second
            elif(raw_args.sensor == "temp"):    
                das.plotGraph(df, "Fahrenheit", f_data) #fahrenheit
            elif(raw_args.sensor == "wheelspeed"):
                das.plotGraph(df, "mph", f_data)        #miles per hour
    else:
        if (raw_args.function == "avg"):
            das.print_avg(df, f_data)
        elif (raw_args.function == "absmin"):
            das.print_absmin(df, f_data)
        elif (raw_args.function == "absmax"):
            das.print_absmax(df, f_data)


if __name__ == '__main__':
    main()