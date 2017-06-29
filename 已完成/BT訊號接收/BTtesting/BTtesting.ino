#include<SoftwareSerial.h>
#include<String.h>
#include<Wire.h>
using namespace std;
#define MAX 25
SoftwareSerial I2CBT(10,11);

void setup(){
  pinMode(10,INPUT);
  pinMode(11,OUTPUT);
  I2CBT.begin(9600);
  Serial.begin(9600);
}

char cmd[MAX]={0}; // received 128 bytes from an Android system
int insize=0;

void loop(){
    //Serial.println(BT_trans());
    BT_trans();
}

char BT_trans(){ //return ',' if not starteds
  insize=I2CBT.available();
  cmd[0]=0;
  if (insize>17 ){ // 讀取藍牙訊息
      cmd[0]=(char)I2CBT.read();
      if(cmd[0]=='A'){
        for(int i = 1;i<=insize-1;i++) {cmd[i]=(char)I2CBT.read();Serial.print(cmd[i]);}
        Serial.print(" ");Serial.print(cmd[1]);
        Serial.println();
        
        if(cmd[1]=='S') {I2CBT.write('S'); return 'S'; } //Motor start
        else if(cmd[1]=='n') {I2CBT.write('n'); return 'n'; } //no motor lunching
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
  return 'Q'; //fail

}

