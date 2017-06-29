#include<Servo.h>

Servo servoOut;

void setup(){
  servoOut.attach(4);
  pinMode(8,INPUT);
  Serial.begin(9600);
  servoOut.write(180);
}
//522 2355
void loop(){
  int dur;
  dur=pulseIn(8,HIGH);
  Serial.println(dur,DEC);
  delay(200);  
  
}
