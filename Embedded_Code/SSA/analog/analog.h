#ifndef _ANALOG_
#define _ANALOG_

class IRsensors
{
    public:
        IRsensors()
        {
        }; //default constructor
        //Functions:
        void analogSetup();
        void analogData();
        float analogConvert(int analog_read);
        float calcTemp(float Volts);
        void printAnalogData(float temp1, float temp2, float temp3); 
        float getTemps(int x) { return temps[x-1]; } //scaling to 0, 1, 2
    private:
        float temps[3];
};

#endif
