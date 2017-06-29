#include <Wire.h>
#include "ADXL345.h" //姿態感測
#include <Adafruit_BMP085.h>
#define alpha 0.5 //low pass filter parameter

double pitch,roll,fXg,fYg,fZg,Altitude;

ADXL345 acc;
Adafruit_BMP085 bmp;

void setup(){
    acc.begin();
    bmp.begin();
    Serial.begin(9600);
    delay(100);
}

void loop(){
    
    getSensorValue();
 
    Serial.print("Pitch = ");Serial.print(pitch);Serial.print("  ");
    Serial.print("Roll = ");Serial.print(roll);Serial.print("  ");
    Serial.print("Altitude = ");Serial.print(Altitude);
    Serial.println(" meters");
    Serial.println();
    delay(50);

}

void getSensorValue(){
    double Xg, Yg, Zg;
    acc.read(&Xg, &Yg, &Zg);
    //Low Pass Filter to smooth out data
    fXg = Xg * alpha + (fXg * (1.0 - alpha));
    fYg = Yg * alpha + (fYg * (1.0 - alpha));
    fZg = Zg * alpha + (fZg * (1.0 - alpha));
   //Roll and Pitch Equations
    roll  = (atan2(-fYg, fZg)*180.0)/M_PI;
    pitch = (atan2(fXg, sqrt(fYg*fYg + fZg*fZg))*180.0)/M_PI;
    
    Altitude = bmp.readAltitude();
    
}

