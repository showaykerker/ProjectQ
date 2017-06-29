void get_value(){ //HOME 不用call
  unsigned int tmp_power=0;
  unsigned int i;
  for(i = 0;i<=strlen(cmd)-1;i++) if(cmd[i]>=48 && cmd[i]<=57) break;
  while(1){ //power
    if(cmd[i]>=48 && cmd[i]<=57) tmp_power=tmp_power*10+int(cmd[i])-48;
    else break;
    i++;
  }
  power=tmp_power;
  //==============================================================
  i++;
  float tmp_axis=0,times=0.1;
  bool negative=false,dec=false;
  if(cmd[i]=='-') {negative=true;i++;}
  while(1){
    if(cmd[i]==',') break;
    else if(cmd[i]=='.') {i++;dec=true;}
    else{
      if(dec==false) tmp_axis=tmp_axis*10+int(cmd[i])-48;
      else {tmp_axis+=times*float(cmd[i]-48);times*=0.1;}
      i++;
    }
  }
  if(negative) x_axis=-1*tmp_axis;
  else x_axis=tmp_axis;
  //===================================================== y_axis is wrong
  i++; 
  tmp_axis=0;times=0.1;negative=false;dec=false;
 if(cmd[i]=='-') {negative=true;i++;}
  while(1){
    if(i==strlen(cmd))break;
    else if(cmd[i]=='.') {i++;dec=true;}
    else{
      if(dec==false) tmp_axis=tmp_axis*10+int(cmd[i])-48;
      else {tmp_axis+=times*float(cmd[i]-48);times*=0.1;}
      i++;
    }
  }
  if(negative) y_axis=-1*tmp_axis;
  else y_axis=tmp_axis;
    
}

void Landing(){
      while ( speed_more_than(mLOW) ) {
        for(int i=0;i<=3;i++) {motor_speed[i]-=DECAY;}
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
  launch=true;
  //delay(500);
  digitalWrite(4,LOW);
}

void Shut(){
  launch=false;
  digitalWrite(4,HIGH);
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
  //delay(100);
}


