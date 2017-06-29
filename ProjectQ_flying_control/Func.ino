//================================================= motion operating =================================================

void keep_height(){ //height keeping using PID
  float now_height=Altitude;
  while ( cmd_state == 'K' ){
     unsigned short int 
     BT_trans(); // and get BT values 
     Adjust();
     
     //PID
     
  }
}

void landing(){
    for( int i = 0 ; i <= 3 ; i++ ){
       if( motor[i].speedVal - DECAYtime >= 0 ){
         motor[i].speedVal -= DECAYval; 
         motor[i].set_speed(); 
       }
    }
    delay(DECAYtime);
}

void shut_down(){
  launch=false;
  digitalWrite(RELAY_Pin,HIGH);
}

void back_home(){
  // no GPS
}

void start_motor(){
  if ( !launch ){
    launch = true;
    //Serial.println("start_motor");
    /*for( int i = 0 ; i <= 3 ; i++ ){
       motor[i].speedVal = cmd_power;
       motor[i].set_speed();
       Serial.println(cmd_power);
    }*/
    //gospeed();
    digitalWrite( RELAY_Pin , LOW );
    cmd_power=0;
    
    
  }
 
}


//================================================= motor adjusting =================================================

void Adjust(){ //adjust pitch and roll using PID
  if(launch){
    
  
  }
}

void do_rotate(){
    motor[0].speedVal += pROTATE * cmd_rotate; motor[0].set_speed();
    motor[1].speedVal -= pROTATE * cmd_rotate; motor[1].set_speed();
    motor[2].speedVal += pROTATE * cmd_rotate; motor[2].set_speed();
    motor[3].speedVal -= pROTATE * cmd_rotate; motor[3].set_speed();
}


//================================================= value reading =================================================

bool BT_trans(){
  bool legal=false;
  int insize=I2CBT.available();
  if (insize==17){ // 讀取藍牙訊息
     for(unsigned int i = 0;i<=insize-1;i++) {cmd[i]=(char)I2CBT.read();Serial.print(cmd[i]);}
     Serial.print("  ");Serial.print(cmd_power);
     Serial.println();
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
