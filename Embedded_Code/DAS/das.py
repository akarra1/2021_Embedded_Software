# DAS script revision 1
# Written by Andrew Kettle
# Last Revision: Sept 4th, 2020

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import sys 
import argparse
import daslib as das

def main():
    #get command line args
    raw_args = das.getargs()
    #read csv file
    df = das.getdf(raw_args[0]) #gets dataframe from csv file
    #filter for desired sensor
    df = das.filterSensor(df, raw_args[1])
    #starting with only one function at a time, will adapt later on    
     


