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

def avg(df): 
    return df.mean()

def relmax(df):
    mx = pd.DataFrame() #empty dataframe
    pct = df.pct_change() #calulating percent change and storing in pct
    for col in df.columns: #iterates over pct to find relative max indecies
        max_data = (df[col]).index[((pct[col]).shift(-1) > 0) & ((pct[col]).shift(1) < 0)].tolist() #finds relative maxima
        mx = pd.concat([mx, pd.Series(max_data).rename(col)], axis=1, sort=False)
    return mx

def relmin(df):
    mn = pd.DataFrame() #empty dataframe
    pct = df.pct_change() #calulating percent change and storing in pct
    for col in df.columns: #iterates over pct to find relative max indecies
        max_data = (df[col]).index[((pct[col]).shift(-1) < 0) & ((pct[col]).shift(1) > 0)].tolist() #finds relative maxima
        mn = pd.concat([mn, pd.Series(max_data).rename(col)], axis=1, sort=False)
    return mn

def absExtremum(data, extype):
    if(extype=="min"):
        return data.min()
    elif(extype=="max"):
        return data.max()
    else:
        return -1

####### END DATA ANALYSIS #######

####### BEGIN VISUALIZATION FUCNTIONS #######

####### END VISUALIZATION FUCNTIONS #######
