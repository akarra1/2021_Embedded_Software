# Data Acquisition Script

## Description
The DAS is a script designed to interpret the raw data collected from the SSA. Currently, it is run on the python interpreter using various arguments to change the behavior of the program

### Dependencies
Libraries: matplotlib.pyplot, numpy, pandas, argparse
```
pip3 install pandas numpy matplotlib argparse
```

## Usage

### Command line argument layout:
```
python3 das.py --csv <file> --sensor <SENSOR> --function <FUNCTION> --view <VIEW>
```

Example of command line argument:

### Read in Accelerometer data and plot the relative minimums over the raw data
```
python3 das.py --csv data_1.csv --sensor accel --function relmin --view standard
```

### Read in IR temperature data and plot the raw values
```
python3 das.py --csv data_1.csv --sensor temp --function raw --view standard
```

## Example of raw data visualization for the accelerometer
![accel_ex](SampleIMUGraph.png)
