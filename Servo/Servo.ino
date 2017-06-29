#include<Servo.h>
#include <SoftwareSerial.h>
#define Servo_Pin 3
  Servo m[4];
void setup() {
  Serial.begin(9600);

  m[0].attach(3);
    m[1].attach(5);
      m[2].attach(6);
        m[3].attach(9);
}

void loop() {
for(int i = 0;i<=3;i++) m[i].write(5);
delay(3000);
for(int j = 10;j<=160;j++){
  for(int i = 0;i<=3;i++) m[i].write(j);
  delay(200);
  if(j==160) delay(10000);
}


}
