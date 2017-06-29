#include<SoftwareSerial.h>
#include<Wire.h>
#include<Servo.h>
#include "ADXL345.h" //姿態感測
#include <Adafruit_BMP085.h>


//================================================= define =================================================

// Arduino pin define
#define RELAY_Pin 4
#define TxD_Pin 10
#define RxD_Pin 11

// parameters
#define alpha 0.5 // low pass filter parameter for ADXL345
#define LOSTMAX 10000 // call function landing() if losing signal for 5000 milliseconds
#define DECAYtime 1000 // in landing(), decay motor.speedVal DECAYval every DECAYtime
#define DECAYval 1
#define pROTATE 0.4 //parameter of rotation


//================================================= declaration =================================================

char cmd[18]={0}; // recieve data from Android device
char cmd_state; // cmd[1] : special command
unsigned short int cmd_power , cmd_x , cmd_y , cmd_rotate ; // and get data from cmd[]
double pitch , roll , Altitude ; // get data from sensors
double ini_pitch , ini_roll , ini_Altitude ; // initialize the data from sensors as a conference
double fXg , fYg , fZg ; // for low pass filter

bool launch;
unsigned short int lost_time;

//declare classes
Servo motor[4];
SoftwareSerial I2CBT(TxD_Pin,RxD_Pin);
ADXL345 acc;
Adafruit_BMP085 bmp;



//================================================= main processes =================================================

void setup(){
  pinMode(RELAY_Pin,OUTPUT);  digitalWrite(RELAY_Pin,HIGH);
  pinMode(TxD_Pin,INPUT);  pinMode(RxD_Pin,OUTPUT);
  acc.begin();  bmp.begin();  I2CBT.begin(9600);  Serial.begin(9600);
  motor[0].attach(3);  motor[1].attach(5);  motor[2].attach(6);  motor[3].attach(9);
  
  // initialize flags
  launch=false;
  lost_time=-1;
  ini_pitch=-1; ini_roll=-1; ini_Altitude=-1; 
  
}



void loop(){
   get_sensor_value();
   bool rec=BT_trans();
   if(rec){ // and get BT value
       process();
       if(launch) {}
   }
}


void process(){
  Adjust(); 
  switch (cmd_state){
      case 'G':
        start_motor();  break;
      case 'L':
        landing();  break;
      case 'X':
        shut_down();  break;
      case 'B':
        back_home();  break;
      case 'K':
        keep_height();  break;
  }
  if(cmd_rotate!=45){
      do_rotate();
  }
}





