#include"L3G4200D.h"
#include<Wire.h>

L3G4200D gyro;
double x,y,z;

void setup(){
  Wire.begin();
  gyro.initialize(250);
  Serial.begin(9600);
}

void loop(){
  x = gyro.getX();
  Serial.print("X:");
  Serial.print(x);

  y = gyro.getY();
  Serial.print(" Y:");
  Serial.print(y);

  z = gyro.getZ();
  Serial.print(" Z:");
  Serial.println(z);
  delay(300); //Just here to slow down the serial to make it more readable
}
