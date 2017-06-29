#include <SoftwareSerial.h>
#include <Servo.h>
#include <Wire.h>
#include <String.h>
using namespace std;
#include "QuadraCopter.h"
#include "ADXL345.h" //姿態感測
#include "L3G4200.h"
#include "Adafruit_BMP085.h"

// Arduino pin define
#define RELAY_Pin 8
#define TxD_Pin 10
#define RxD_Pin 11
#define Front_Left_Pin 3 //blue
#define Front_Right_Pin 5 //green
#define Back_Left_Pin 6 //grey
#define Back_Right_Pin 9 //white

// CONSTS
#define MAX 70 //for BT command
#define cP 0.05
#define cI 2
#define cD 0.3
#define ROTATE 20

QuadraCopter Q(RELAY_Pin, cP, cI, cD);
SoftwareSerial I2CBT(TxD_Pin,RxD_Pin);
char cmd[17]={0}; // received 128 bytes from an Android system
int previous_power=0;

void setup(){

  pinMode(RELAY_Pin,OUTPUT); digitalWrite(RELAY_Pin,HIGH);
  pinMode(TxD_Pin,INPUT); pinMode(RxD_Pin,OUTPUT);
  I2CBT.begin(9600); Serial.begin(9600);  
  Q.begin(Front_Left_Pin, Front_Right_Pin, Back_Left_Pin, Back_Right_Pin);
  
}

void loop(){
   if(BT_trans()=='G' && Q.is_launch()==false)  Q.Launching();
   //BT_trans();
  //Q.print_debug_info();
  if(Q.is_launch()) Q.print_servo_speed();
}



