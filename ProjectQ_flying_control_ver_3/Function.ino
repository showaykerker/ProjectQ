char BT_trans(){
  char tmp_cmd[MAX]={0};
  int insize=I2CBT.available();
  for(int i = 0;i<=MAX-1;i++) tmp_cmd[i]=0;
  if (insize>17 ){ // 讀取藍牙訊息
      tmp_cmd[0]=(char)I2CBT.read();
      if(tmp_cmd[0]=='A'){
        //Serial.println(insize);
        //Serial.println(tmp_cmd[0]);
        for(int i = 1;i<=insize-1;i++) {tmp_cmd[i]=(char)I2CBT.read();}//Serial.print(tmp_cmd[i]);}
        //Serial.println(tmp_cmd); Serial.println(strlen(tmp_cmd));

        for(int i = 0;i<=16;i++) cmd[i]=0;
        if(insize==17){
          for(int i = 0;i<=16;i++) cmd[i]=tmp_cmd[i];
          
          if(cmd[1]=='G' && Q.is_launch()==false) return 'G';
          
          get_bt_command();
          //Serial.println(cmd);
        }
        else if(insize==18){
          for(int i = 0;i<=16;i++) cmd[i]=tmp_cmd[i];

          if(cmd[1]=='G' && Q.is_launch()==false) return 'G';

          get_bt_command();
          //Serial.println(cmd); 
        }
        else if(insize==19){
          for(int i = 0;i<=16;i++) cmd[i]=tmp_cmd[i];
          
          if(cmd[1]=='G' && Q.is_launch()==false) return 'G';          
          
          get_bt_command();
          //Serial.println(cmd);
        }
        else if(insize==34){

          for(int i = 0;i<=16;i++) cmd[i]=tmp_cmd[i];
          
          if(cmd[1]=='G' && Q.is_launch()==false) return 'G';          
          
          get_bt_command();
          //Serial.println(cmd);
          
          for(int i = 0;i<=16;i++) cmd[i]=tmp_cmd[i+17];
          
          if(cmd[1]=='G' && Q.is_launch()==false) return 'G';          
          
          get_bt_command();
          //Serial.println(cmd);
        }
        else if(insize==51){
          for(int i = 0;i<=16;i++) cmd[i]=tmp_cmd[i];
          
          if(cmd[1]=='G' && Q.is_launch()==false) return 'G';          
          
          get_bt_command();
          //Serial.println(cmd);
          for(int i = 0;i<=16;i++) cmd[i]=tmp_cmd[i+17];
          
          if(cmd[1]=='G' && Q.is_launch()==false) return 'G';          
          
          get_bt_command();
          //Serial.println(cmd);          
          for(int i = 0;i<=16;i++) cmd[i]=tmp_cmd[i+34];

          if(cmd[1]=='G' && Q.is_launch()==false) return 'G';          
          
          get_bt_command();
          //Serial.println(cmd);
        }
        else return 'F'; //failure
        
      }
  }
  else return 'Q'; // fail to get BT command
}



void get_bt_command(){  
  
  if(cmd[1]=='K' && Q.steady_avail()==false) Q.keep_height();
  else if(cmd[1]=='L' ) Q.Landing();
  else if(cmd[1]=='X') Q.SHUTDOWN();
  else if(cmd[1]=='G' && Q.is_launch()==0) Q.Launching();
  else{
     int rotate=(cmd[13]-'0')*10 + (cmd[14]-'0'); 
     if(rotate>45){ // CW

         int tmp=(rotate-45)*ROTATE/45;
         Q.Adjust_Servo_Speed(tmp, -tmp, -tmp, tmp);      
         Serial.print("CW: "); Q.print_servo_speed();   
         delay(100);
         Q.Adjust_Servo_Speed(-tmp, tmp, tmp, -tmp);     
     }
     else if(rotate<45){ // CCW
         int tmp=(45-rotate)*ROTATE/45;
         Q.Adjust_Servo_Speed(-tmp, tmp, tmp, -tmp);     
         Serial.print("CCW: "); Q.print_servo_speed();
         delay(100);
         Q.Adjust_Servo_Speed(tmp, -tmp, -tmp, tmp);
     }
     
     int power=(cmd[3]-'0')*100 + (cmd[4]-'0')*10 + (cmd[5]-'0');
     power=map(power,0,160,0,130);
     if(power!=previous_power){
         Q.power_mod(power-previous_power);
         previous_power=power;
     }
      
     double roll = (cmd[7]-'0')*10 + (cmd[8]-'0') - 30 ; //left & right
     roll/=2;
     double pitch = (cmd[10]-'0')*10 + (cmd[11]-'0') -20 ;
     pitch/=2;
     if(Q.steady_avail()==true){
         Q.print_debug_info();
         int p_PR = Q.check_previous_PR(pitch,roll);
         if(p_PR==0){
             Q.Adjust_Pitch(pitch);  
             //Q.Adjust_Roll(roll);	 
             Q.record_now_PR(pitch,roll);
         }
         else if(p_PR==1) { 
           //Q.Adjust_Roll(roll);
           Q.record_now_PR(pitch,roll); 
         }
         else if(p_PR==2) { 
           Q.Adjust_Pitch(pitch); 
           Q.record_now_PR(pitch,roll); 
         }
         else if(p_PR==3) ; //　沒變

     }
     
  } //else

}


