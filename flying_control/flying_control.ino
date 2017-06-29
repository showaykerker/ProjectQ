#include<SoftwareSerial.h>
#include<Wire.h>
#include<Servo.h>
#define RELAY_Pin 4
#define TxD_Pin 10
#define RxD_Pin 11
#define MAX 25
#define mLOW 20 //油門可控制最小輸出
#define mHIGH 160 //油門可控制最小輸出
#define DECAY 8//landing遞減參數
#define DECAYTIME 1000 //landing遞減時間參數(毫秒)
#define Turn 10 //z軸旋轉增減參數
#define LOSTMAX 10000 //10秒無訊號就landing
#define BALANCEX 12 //平衡參數
#define BALANCEY 8 //平衡參數
#define BALANCEPARA 1 //平衡參數 bPARA * bX + bPARA * bY must<180-mHIGH

int lost_time=-1;
float x_axis=0,y_axis=0;
unsigned int power=0;
bool launch=false; //是否start
SoftwareSerial I2CBT(TxD_Pin,RxD_Pin);
Servo motor[4];
float motor_speed[4]={0}; 

void setup(){
  pinMode(RELAY_Pin,OUTPUT); digitalWrite(RELAY_Pin,HIGH);
  pinMode(TxD_Pin,INPUT);
  pinMode(RxD_Pin,OUTPUT);
  I2CBT.begin(9600);
  Serial.begin(9600);
  motor[0].attach(3);
  motor[1].attach(5);
  motor[2].attach(6);
  motor[3].attach(9);
}

char cmd[MAX]={0}; // received 128 bytes from an Android system
int insize=0;
char command_state=0;

void loop(){
   command_state=BT_trans(); 
   
   
   if(launch){
      get_value();
      //power_write(); //寫入油門數值
      
    
      set_speed(-1,-1,-1,-1);
      //Serial.println(command_state);
      
      if(command_state!=-1) lost_time=-1;
      else{
        if(lost_time==-1) lost_time=millis();
        else if(millis()-lost_time>=LOSTMAX) Landing();
      }
    }
    
    switch (command_state){
      case 'S': Start(); break; //Motor start       
      case 'n': break;//no motor launching
      case 'L': Landing();break;//Landing
      case 'X': Shut(); break;//shut down 
      case 'H': break;//go back home
      case 'K': break;//Keep height
      case 'N': CCW();break;//CCW
      case 'C': CW();break;//CW
      case 's': break;//setting mode
      case 'r': break;//motor running
      case 'Q' : break;//no signal
    }
}

void power_write(){
   for(unsigned int i = 0;i<=3;i++) motor_speed[i]=map(power,0,160,mLOW,mHIGH);
   // 0 1
   // 2 3
   float x=map(x_axis,-30,30,-BALANCEX,BALANCEX);
   float y=map(y_axis,-20,20,-BALANCEY,BALANCEY);
   motor_speed[0]=motor_speed[0]+x*BALANCEPARA-y*BALANCEPARA;
   motor_speed[1]=motor_speed[0]-x*BALANCEPARA-y*BALANCEPARA;
   motor_speed[2]=motor_speed[0]+x*BALANCEPARA+y*BALANCEPARA;
   motor_speed[3]=motor_speed[0]-x*BALANCEPARA+y*BALANCEPARA;
}

char BT_trans(){
  insize=I2CBT.available();
  cmd[0]=0;
  if (insize>17 ){ // 讀取藍牙訊息
      cmd[0]=(char)I2CBT.read();
      if(cmd[0]=='D'){
        for(unsigned int i = 1;i<=insize-1;i++) {cmd[i]=(char)I2CBT.read();}//Serial.print(cmd[i]);}
        
        for(int i =0;i<=3;i++) motor_speed[i]=map(power,0,160,10,160);
      
        Serial.print("  power="); Serial.println(power);
        Serial.print("m0="); Serial.print(motor_speed[0]);Serial.print("  m1="); Serial.print(motor_speed[1]);
        Serial.print("  m2="); Serial.print(motor_speed[2]);Serial.print("  m3="); Serial.println(motor_speed[3]);  
        
        if(cmd[1]=='S') {I2CBT.write('S'); return 'S'; } //Motor start
        else if(cmd[1]=='n') {I2CBT.write('n'); return 'n'; } //no motor launching
        else if(cmd[1]=='L') {I2CBT.write('L'); return 'L'; } //Landing
        else if(cmd[1]=='X') {I2CBT.write('X'); return 'X'; } //shut down
        else if(cmd[1]=='H') {I2CBT.write('H'); return 'H'; } //go back home
        else if(cmd[1]=='K') {I2CBT.write('K'); return 'K'; } //Keep height
        else if(cmd[1]=='N') {I2CBT.write('N'); return 'N'; } //CCW
        else if(cmd[1]=='C') {I2CBT.write('C'); return 'C'; } //CW
        else if(cmd[1]=='s') {I2CBT.write('s'); return 's'; } //setting mode
        else if(cmd[1]=='r') {I2CBT.write('r'); return 'r'; } //motor running
        else return 'Q';
      }
  }
  else {I2CBT.write('T'); return 'Q';}
}


