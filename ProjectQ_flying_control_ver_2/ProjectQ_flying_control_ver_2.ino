#include<SoftwareSerial.h>
#include<Wire.h>
#include<Servo.h>
#include "showay_servo.h"
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
// L3G4200
int gyroRaw[3];                         // raw sensor data, each axis, pretty useless really but here it is.
double gyroDPS[3];                      // gyro degrees per second, each axis
int gyroZeroRate[3];                    // Calibration data.  Needed because the sensor does center at zero, but rather always reports a small amount of rotation on each axis.
int gyroThreshold[3];                   // Raw rate change data less than the statistically derived threshold is discarded.
float dpsPerDigit=.00875f;              // for conversion to degrees per second
//L3G4200
char cmd[18]={0}; // recieve data from Android device
char cmd_state; // cmd[1] : special command
unsigned short int cmd_power , cmd_x , cmd_y , cmd_rotate ; // and get data from cmd[]
double pitch , roll , Altitude ; // get data from sensors
double ini_pitch , ini_roll , ini_Altitude ; // initialize the data from sensors as a conference
double fXg , fYg , fZg ; // for low pass filter

bool launch;
unsigned short int lost_time;

//declare classes
QuadraCopter Q;
//Servo motor[4];
SoftwareSerial I2CBT(TxD_Pin,RxD_Pin);
ADXL345 acc;
Adafruit_BMP085 bmp;



void debug_info(){
  /*
      // L3G4200
        int gyroRaw[3];                         // raw sensor data, each axis, pretty useless really but here it is.
        double gyroDPS[3];                      // gyro degrees per second, each axis
        int gyroZeroRate[3];                    // Calibration data.  Needed because the sensor does center at zero, but rather always reports a small amount of rotation on each axis.
        int gyroThreshold[3];                   // Raw rate change data less than the statistically derived threshold is discarded.
        float dpsPerDigit=.00875f;              // for conversion to degrees per second
      // L3G4200
        char cmd[18]={0}; // recieve data from Android device
        char cmd_state; // cmd[1] : special command
        unsigned short int cmd_power , cmd_x , cmd_y , cmd_rotate ; // and get data from cmd[]
        double pitch , roll , Altitude ; // get data from sensors
        double ini_pitch , ini_roll , ini_Altitude ; // initialize the data from sensors as a conference
        double fXg , fYg , fZg ; // for low pass filter
        
        bool launch;
        unsigned short int lost_time;
  */
  
  
  Serial.print("cmd="); Serial.println(cmd);
  Serial.print("cmd_power="); Serial.print(cmd_power);
  Serial.print("  cmd_rotate="); Serial.print(cmd_rotate);
  Serial.print("  cmd_x="); Serial.print(cmd_x);
  Serial.print("  cmd_y="); Serial.println(cmd_y);
  Serial.print("ini_pitch="); Serial.print(ini_pitch);
  Serial.print("  ini_roll="); Serial.print(ini_roll);
  Serial.print("  ini_Altitude="); Serial.println(ini_Altitude); 
  
  Serial.print("pitch="); Serial.print(pitch);
  Serial.print("  roll="); Serial.print(roll);
  Serial.print("  Altitude="); Serial.println(Altitude);
  Serial.print("fXg="); Serial.print(fXg);
  Serial.print("  fYg="); Serial.print(fYg);
  Serial.print("  fZg="); Serial.println(fZg);
  Serial.print("gyroDPS[0]="); Serial.print(gyroDPS[0]);
  Serial.print("  gyroDPS[1]="); Serial.print(gyroDPS[1]);
  Serial.print("  gyroDPS[2]="); Serial.println(gyroDPS[2]);
//  delay(500);
  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");     // cursor to home command
  
}

//================================================= main processes =================================================

void setup(){
  pinMode(RELAY_Pin,OUTPUT);  digitalWrite(RELAY_Pin,HIGH);
  pinMode(TxD_Pin,INPUT);  pinMode(RxD_Pin,OUTPUT);
  acc.begin();  bmp.begin();  I2CBT.begin(9600);  Serial.begin(9600);
  Wire.begin();  setupGyro();  calibrateGyro();
  Q.motor[0].attach(3);  Q.motor[1].attach(5);  Q.motor[2].attach(6);  Q.motor[3].attach(9);
  
}

void loop(){
   debug_info();
   bool rec=BT_trans();
   get_sensor_value();
   if(rec){ // and get BT value
   }
}




/*
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
*/

//================================================= value reading =================================================

bool BT_trans(){
  for(int i = 0;i<=17;i++) cmd[i]=0;
  bool legal=false;
  int insize=I2CBT.available();
  if (insize>=17){ // 讀取藍牙訊息
     for(unsigned int i = 0;i<=insize-1;i++) {cmd[i]=(char)I2CBT.read();/*Serial.print(cmd[i]);*/}
     //Serial.print("  "); Serial.print(cmd_power);
     //Serial.println();
       get_BT_value();
       lost_time=-1;
       legal=true;
  }
  else{

    if(insize>0){
      for(unsigned int i = 0;i<=insize-1;i++) {I2CBT.read();} 
    }

    if(launch){
      if ( lost_time == -1 ) lost_time=millis();
      else if ( millis() - lost_time >= LOSTMAX ) {Serial.println("landging()QQ");landing();}
    }
  }
  return legal;
}

char get_BT_value(){
    cmd_state=cmd[1];
    cmd_power = ( cmd[3] - 48 ) * 100 + ( cmd[4] - 48 ) * 10 + ( cmd[5] - 48 ) * 1 ;
    cmd_power=map(cmd_power,0,160,10,160);
    cmd_x = ( cmd[7] - 48 ) * 10 + ( cmd[8] - 48 ) * 1 ;
    cmd_y = ( cmd[10] - 48 ) * 10 + ( cmd[11] - 48 ) * 1 ;
    cmd_rotate = ( cmd[13] - 48 ) * 10 + ( cmd[14] - 48 ) * 1 ;
}

void get_sensor_value(){
    updateGyroValues();
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
    if(ini_Altitude==-1 && launch==1) ini_Altitude=Altitude;
    if(ini_pitch==-1 && launch==1) ini_pitch=pitch;
    if(ini_roll==-1 && launch==1) ini_roll=roll;
}

void landing(){
  
}


