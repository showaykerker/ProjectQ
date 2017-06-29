#include <SoftwareSerial.h>
#include <Wire.h>
#include <Servo.h>
#include "showay_servo.ino"
#include "ADXL345.ino" //姿態感測
#include <Adafruit_BMP085.h>

#define RELAY_Pin 4
#define TxD_Pin 10
#define RxD_Pin 11
#define Front_Left_Pin 3 //blue
#define Front_Right_Pin 5 //green
#define Back_Left_Pin 6 //grey
#define Back_Right_Pin 9 //white

#define alpha 0.5 //low pass filter for ADXL345

