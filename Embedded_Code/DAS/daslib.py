# DAS library revision 1
# Written by Andrew Kettle
# Last Revision: Sept 4th, 2020

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import sys 
import argparse

####### INPUT & FILTERING FUNCTIONS ########

def getargs():
    parser = argparse.ArgumentParser(description = "Data anaylsis script for the EV racecar program")
    parser.add_argument('--csv', dest="csvfile", action='store', help="CSV file name with .csv extension")
    parser.add_argument('--sensor', dest="sensor", action='store', help="Available options: all, imu, temp, or wheelspeed")
    parser.add_argument('--function', dest="function", action='store', help="Available options: raw, absmax, absmin, avg, relmax, relmin")
    parser.add_argument('--view', dest="view", action='store', help="Available options: standard") #maybe add items to this later

    if(len(sys.argv) < 9):
        print(len(sys.argv))
        print("Incorrect command line usage, the correct usage is on the line below\n")
        print("python3 das.py --csv <file> --sensor <SENSOR> --function <FUNCTION> --view <VIEW>\n")
        print("An example would be: python3 das.py --csv file --sensor imu --function raw --view normal")
        exit() #quits if the arguments are invalid
    else:
        args = parser.parse_args()
        return args

def getdf(filename, headernames):
    df = pd.read_csv(filename, names=headernames)
    return df
   
def filterSensor(df, sensor):
    if(sensor=='all'):
        return df #leaves df unchagned 
    elif(sensor=='imu'):
        return df[["Accel_X", "Accel_Y", "Accel_Z", "Gyro_X", "Gyro_Y", "Gyro_Z"]] #filters for imu 
    elif(sensor=='temp'):
        return df[["IR_1", "IR_2", "IR_3"]] #filters for temp sensors
    elif(sensor=='wheelspeed'):
        return df["Wheelspeed"] #filters for wheelspeed 

####### END INPUT & FILTERING   ########

####### DATA ANALYSIS FUNCTIONS ########

def avg(df, sensor): 
    return df.sensor.mean()

def relmax(df, sensor):
    pct = df.pct_change()
    max_data = pct.sensor[(pct.sensor.shift(-1) > 0) & (pct.sensor.shift(-1) < 0)] #finds relative maximum
    return max_data

def relmin(df, sensor):
    pct = df.pct_change()
    min_data = pct.sensor[(pct.sensor.shift(-1) < 0) & (pct.sensor.shift(-1) > 0)] #finds relative minimum
    return min_data

def absExtremum(data, extype):
    if(extype=="min"):
        return data.min()
    elif(extype=="max"):
        return data.max()
    else:
        return -1

####### END DATA ANALYSIS #######

####### BEGIN VISUALIZATION FUCNTIONS #######
