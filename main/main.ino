#include<SoftwareSerial.h>
#include<Wire.h>
#include<Servo.h>
#define MAX 25
#define mLOW 20 //油門可控制最小輸出
#define mHIGH 160 //油門可控制最小輸出
#define DECAY 8//landing遞減參數
#define DECAYTIME 1000 //landing遞減時間參數(毫秒)
#define Turn 10 //z軸旋轉增減參數
SoftwareSerial I2CBT(10,11);
Servo motor[4];
unsigned int motor_speed[4];

void setup(){
  pinMode(4,INPUT); digitalWrite(4,LOW);
  pinMode(10,INPUT);
  pinMode(11,OUTPUT);
  I2CBT.begin(9600);
  Serial.begin(9600);
  motor[0].attach(3);
  motor[1].attach(5);
  motor[2].attach(6);
  motor[3].attach(9);
}

char cmd[MAX]={0}; // received 128 bytes from an Android system
int insize=0;

void loop(){
    switch (BT_trans()){
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
      
      
    }
}

void Landing(){
      while ( speed_more_than(mLOW) ) {
        for(int i=0;i<=3;i++) {motor_speed[i]=DECAY;}
        set_speed(-1,-1,-1,-1);
        delay(DECAYTIME);
      }
}

bool speed_more_than(unsigned int a){
  for(int i = 0;i<=3;i++){
     if(motor_speed[i]<=a) return false; 
  }
  return true;
}

void CCW(){
    set_speed(motor_speed[0]+Turn,motor_speed[1]-Turn,motor_speed[2]-Turn,motor_speed[3]+Turn);
}

void CW(){
    set_speed(motor_speed[0]-Turn,motor_speed[1]+Turn,motor_speed[2]+Turn,motor_speed[3]-Turn);
}

void Start(){
  for(int i = 0;i<=3;i++) motor[i].write(0);
  digitalWrite(4,HIGH);
}

void Shut(){
  digitalWrite(4,LOW);
}

void set_speed(int a,int b,int c,int d){
  if(a == -1)
    for(int i = 0;i<=3;i++) motor[i].write(motor_speed[i]);
  else{
    motor[0].write(a);
    motor[1].write(b);
    motor[2].write(c);
    motor[3].write(d);
  }
}

char BT_trans(){
  insize=I2CBT.available();
  cmd[0]=0;
  if (insize>17 ){ // 讀取藍牙訊息
      cmd[0]=(char)I2CBT.read();
      if(cmd[0]=='D'){
        for(int i = 1;i<=insize-1;i++) {cmd[i]=(char)I2CBT.read();Serial.print(cmd[i]);}
        Serial.println();
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
      }
  }
}

