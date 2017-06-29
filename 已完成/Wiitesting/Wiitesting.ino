//done

#include "Wire.h"
#include "WiiClassic.h"	// 引用Wii傳統手把的程式庫
 
WiiClassic wiiClassy = WiiClassic();
 
void setup() {
  Wire.begin();
  Serial.begin(9600);
  wiiClassy.begin();
  wiiClassy.update();
}
 
void loop() {
  delay(50);
  wiiClassy.update();
 


    Serial.print("L: ");   // L被按下
    Serial.println(wiiClassy.leftShouldPressure());
  
 
    Serial.print("R: ");	// R被按下
    Serial.println(wiiClassy.rightShouldPressure());

  
    Serial.print("Lx: ");  // x被按下
    Serial.println(wiiClassy.leftStickX());
  
 
  
    Serial.print("Ly: ");  // y被按下
    Serial.println(wiiClassy.leftStickY());

    Serial.print("Rx: ");  // x被按下
    Serial.println(wiiClassy.rightStickX());
    
    Serial.print("Ry: ");  // y被按下
    Serial.println(wiiClassy.rightStickY());
 
      Serial.println("===================");

    
      delay(50);
  /*
  if (wiiClassy.lzPressed()) {
    Serial.println("LZ");	// LZ被按下
  }
  if (wiiClassy.rzPressed()) {
    Serial.println("RZ");	// RZ被按下
  }
  if (wiiClassy.leftDPressed()) {
    Serial.println("LD");	// 十字左被按下
  }
 
  if (wiiClassy.rightDPressed()) {
    Serial.println("RD");	// 十字右被按下
  }
 
  if (wiiClassy.upDPressed()) {
    Serial.println("UD");	// 十字上被按下	
  }
  if (wiiClassy.downDPressed()) {
    Serial.println("DD");	// 十字下被按下
  }
 
  if (wiiClassy.selectPressed()) {
    Serial.println("select");  // SELECT (-)被按下
  }
 
  if (wiiClassy.homePressed()) {
    Serial.println("home.");	// HOME被按下
  }
  if (wiiClassy.startPressed()) {
    Serial.println("start.");	// START (+)被按下
  }
 

 
  if (wiiClassy.aPressed()) {
    Serial.println("a");	// a被按下
  }
 
  if (wiiClassy.bPressed()) {
    Serial.println("b");	// b被按下
  }*/
}
