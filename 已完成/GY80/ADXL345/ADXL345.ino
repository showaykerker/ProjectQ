#include "Wire.h"
#include "ADXL345.h"
const float alpha = 0.5;
double fXg = 0;
double fYg = 0;
double fZg = 0;

ADXL345 acc;
void setup(){
    acc.begin();
    Serial.begin(9600);
    delay(100);
}


void loop(){
    double pitch, roll, Xg, Yg, Zg;
    acc.read(&Xg, &Yg, &Zg);
  //Low Pass Filter to smooth out data
    fXg = Xg * alpha + (fXg * (1.0 - alpha));
    fYg = Yg * alpha + (fYg * (1.0 - alpha));
    fZg = Zg * alpha + (fZg * (1.0 - alpha));

   //Roll and Pitch Equations
    roll  = (atan2(-fYg, fZg)*180.0)/M_PI;
    pitch = (atan2(fXg, sqrt(fYg*fYg + fZg*fZg))*180.0)/M_PI;
    //Serial.print("Xg=");Serial.print(Xg);Serial.print(" ");
    //Serial.print("Yg=");Serial.print(Yg);Serial.print(" ");
    //Serial.print("Zg=");Serial.print(Zg);Serial.println(" ");
    //Serial.print("fXg=");Serial.print(fXg);Serial.print(" ");
    //Serial.print("fYg=");Serial.print(fYg);Serial.print(" ");
    //Serial.print("fZg=");Serial.print(fZg);Serial.println(" ");
    Serial.print("pitch= ");Serial.print(pitch);Serial.print("  ");
    Serial.print("roll= ");Serial.print(roll);Serial.println(" ");Serial.println();
    delay(50);

}

