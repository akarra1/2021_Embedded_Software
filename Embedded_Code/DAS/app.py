# DAS Web App
# Written by Michael Felizardo
# Last Revision: 3/1/2021 


import os
from flask import Flask, render_template, url_for, request, redirect, flash
# from flask_sqlalchemy import SQLAlchemy
from werkzeug.utils import secure_filename
import io

import daslib as das

UPLOAD_FOLDER = './uploads'
ALLOWED_EXTENSIONS = {'csv'}

app = Flask(__name__)
app.secret_key = "racecar"
app.config["CSV_UPLOADS"] = UPLOAD_FOLDER

# app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///data.db'
# db = SQLAlchemy(app)

'''
class Data(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    content = db.Column
    date_created = db.Column(db.DateTime, default=datetime.utcnow)
'''

# used to make sure the user can only upload csv files
def allowed_file(filename):
    return '.' in filename and \
        filename.rsplit('.',1)[1].lower() in ALLOWED_EXTENSIONS

@app.route('/', methods=['GET','POST'])
def index():

    show_data = False

    #handle file upload
    if request.method == 'POST':
        # check if the post request has the file part
        if 'file' not in request.files:
            flash('No file part')
            return redirect(request.url)
        file = request.files['file']
        # handle not uploading file 
        if file.filename == '':
            flash('No selected file')
            return redirect(request.url)
        #handle invalid file type
        if not allowed_file(file.filename):
            flash("Invalid file type")
            return redirect(request.url)
        if file and allowed_file(file.filename):
            filename = secure_filename(file.filename)
            file.save(os.path.join(app.config["CSV_UPLOADS"], file.filename))
            sensor = request.form["sensor"]
            func = request.form["function"]
            (averages, absmins, absmaxes) = handle_data(filename, sensor, func)
            show_data = True
            '''
            return redirect(url_for('handle_data',
                                    filename=filename,
                                    sensor=sensor,
                                    function=func))'''

    if show_data:
        return render_template('index.html', filename = filename, sensor=sensor, function=func,
        averages = averages, absmins = absmins, absmaxes = absmaxes, data= True)
    else:
        return render_template('index.html')


def handle_data(filename,sensor,function):

    headernames = ["Accel_X", "Accel_Y", "Accel_Z", "Gyro_X", "Gyro_Y", "Gyro_Z", "IR_1", "IR_2", "IR_3", "Wheelspeed"]

    #read csv file, append headers
    df = das.getdf("./uploads/" + filename, headernames) #gets dataframe from csv file
    #filter for desired sensor
    df = das.filterSensor(df, sensor)

    f_data = df #assignment in the event that the user chose raw data
    if(function == 'avg'):
        f_data = das.avg(df)
        print(f_data)
    elif(function == 'relmax'):     #relative maximum
        f_data = das.relmax(df)  
    elif(function == 'relmin'):
        f_data = das.relmin(df)  
    elif(function == 'absmax'):
        f_data = das.absExtremum(df, "max")
    elif(function == 'absmin'):
        f_data = das.absExtremum(df, "min")


    #a bunch of if elses to satisfy the sensor argument
    #broken up becasue the raw data option doesn't need the optional arguments 

    if((function == "raw") or (function == "relmin") or (function == "relmax")): #plot these
        if(function == "raw"):             #plots raw graphs without optional arguments
            if(sensor == "accel"):
                fig = das.plotGraph(f_data, "Gs")         #g force
            elif(sensor == 'gyro'):    
                fig = das.plotGraph(f_data, "Dps")        #degrees per second
            elif(sensor == "temp"):    
                fig = das.plotGraph(f_data, "Fahrenheit") #fahrenheit
            elif(sensor == "wheelspeed"):
                fig = das.plotGraph(f_data, "mph")        #miles per hour
        else:
            if(sensor == "accel"):
                fig = das.plotGraph(df, "Gs", f_data)         #g force
            elif(sensor == 'gyro'):    
                fig = das.plotGraph(df, "Dps", f_data)        #degrees per second
            elif(sensor == "temp"):    
                fig = das.plotGraph(df, "Fahrenheit", f_data) #fahrenheit
            elif(sensor == "wheelspeed"):
                fig = das.plotGraph(df, "mph", f_data)        #miles per hour

        
        averages = das.get_avg(df)
       
        absmins = das.get_absmin(df)
        
        absmaxes = das.get_absmax(df)

        return (averages, absmins, absmaxes)
    # plot = '/static/images/plot.png' previously used for static graph image


    

if __name__ == "__main__":
    app.run(debug=True)
    