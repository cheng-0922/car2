//motor
//left
int PWMA = 10;
int AIN2 = 6;
int AIN1 = 7;
//right
int BIN1 = 8;
int BIN2 = 9;
int PWMB = 11;
//IR
#define analogPin1 A3
#define analogPin2 A15
#define analogPin3 A5
#define analogPin4 A6
#define analogPin5 A7

void back();
void right();
// char currentCmd = 'x'; 

//   // 專門檢查藍牙的函數



void MotorWriting(double vL, double vR) {
  
  if (vR >= 0) {
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  
  } else if (vR < 0) {
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  vR = -vR; //因為analogWrite只吃正數，所以如果本來是負數，就要乘-1
  }
  if (vL >= 0) {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  
  } else if (vL < 0) {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  vL = -vL; //因為analogWrite只吃正數，所以如果本來是負數，就要乘-1
  }
  analogWrite(PWMA, vL);
  analogWrite(PWMB, vR);
}
// void checkBluetooth() {
//   if (Serial3.available()) {
//     currentCmd = Serial3.read();
//     // 這裡可以加入你原本的邏輯，例如遇到特殊情況強制停止
//     if (currentCmd == 'x') {
//       MotorWriting(0, 0); 
//     }
//   }
// }
void back(void(*f)()){
  int threshold=120;
  int threshold_m=100;
  int l3 = analogRead(analogPin5);
  int l2 = analogRead(analogPin4);
  int m = analogRead(analogPin3);
  int r2 = analogRead(analogPin2);
  int r3 = analogRead(analogPin1);

  MotorWriting(255, -255);
  delay(250);
  f();
  while(true){
      int l3 = analogRead(analogPin5);
      int l2 = analogRead(analogPin4);
      int m = analogRead(analogPin3);
      int r2 = analogRead(analogPin2);
      int r3 = analogRead(analogPin1);
      if((l3>= threshold) || (l2 >=threshold) || (m >=threshold_m) || (r2 >=threshold || (r3 >= threshold)))
        break;
      MotorWriting(255, -255);
      delay(1);
  }

  
}
int straight(){
  int threshold=100;
  int threshold_m=40;

  while(true){
      int l3 = analogRead(analogPin5);
      int l2 = analogRead(analogPin4);
      int m = analogRead(analogPin3);
      int r2 = analogRead(analogPin2);
      int r3 = analogRead(analogPin1);
      if(!((l3 >=threshold) && (l2 >=threshold) && (m >=threshold_m) && (r2 >=threshold) && (r3 >=threshold)))
        break;

    }

  delay(2);
  while(true){
      int l3 = analogRead(analogPin5);
      int l2 = analogRead(analogPin4);
      int m = analogRead(analogPin3);
      int r2 = analogRead(analogPin2);
      int r3 = analogRead(analogPin1);
      if((l2 >=threshold) || (m >=threshold_m) || (r2 >=threshold))
        break;
    }
  
}
void right(){
  int threshold=80;
  int threshold_m=40;
  // while(true){
  //     int l3 = analogRead(analogPin5);
  //     int l2 = analogRead(analogPin4);
  //     int m = analogRead(analogPin3);
  //     int r2 = analogRead(analogPin2);
  //     int r3 = analogRead(analogPin1);
  //     if(!((l3 >=threshold) && (l2 >=threshold) && (m >=threshold_m) && (r2 >=threshold) && (r3 >=threshold)))
  //       break;
  //   }
  MotorWriting(255, 70); 
  delay(400);
  while(true){
      int l3 = analogRead(analogPin5);
      int l2 = analogRead(analogPin4);
      int m = analogRead(analogPin3);
      int r2 = analogRead(analogPin2);
      int r3 = analogRead(analogPin1);
      if((l2 >=threshold) || (m >=threshold_m) || (r2 >=threshold))
        break;
      MotorWriting(255,70); 
    }
  

}
void left(){
  int threshold=80;
  int threshold_m=40;
  // while(true){
  //     int l3 = analogRead(analogPin5);
  //     int l2 = analogRead(analogPin4);
  //     int m = analogRead(analogPin3);
  //     int r2 = analogRead(analogPin2);
  //     int r3 = analogRead(analogPin1);
      
  //     if(!((l3 >=threshold) && (l2 >=threshold) && (m >=threshold_m) && (r2 >=threshold) && (r3 >=threshold)))
  //       break;
  //   }
  
  MotorWriting(100,255); 
  delay(400);
  while(true){
      int l3 = analogRead(analogPin5);
      int l2 = analogRead(analogPin4);
      int m = analogRead(analogPin3);
      int r2 = analogRead(analogPin2);
      int r3 = analogRead(analogPin1);
      if((l2 >=threshold) || (m >=threshold_m) || (r2 >=threshold))
        break;
      MotorWriting(100, 255);
    }

}
void back_to_line(int to_turn){
  int threshold=50;
  int threshold_m=50;
  // Serial3.print('o');
  while(true){
      int l3 = analogRead(analogPin5);
      int l2 = analogRead(analogPin4);
      int m = analogRead(analogPin3);
      int r2 = analogRead(analogPin2);
      int r3 = analogRead(analogPin1);
      if((l3 >=threshold) || (l2 >=threshold) || (m >=threshold_m) || (r2 >=threshold) || (r3 >=threshold)){
        break;
      } 
      MotorWriting(-150-to_turn, -150+to_turn);
      delay(100);
    }
}
char Tracking(char cmd, void(*f)()) {
  int threshold=100;
  int threshold_m=40;
  int l3 = analogRead(analogPin5);
  int l2 = analogRead(analogPin4);
  int m = analogRead(analogPin3);
  int r2 = analogRead(analogPin2);
  int r3 = analogRead(analogPin1);
  double w1= 5;
  double w2= 2;
  double w3= 1;
  double w4= 4;
  double Kp= 100;

  static double stady[2] = {250,250};
  // static double stady[2] = {200,178};

  double Tpr=stady[0];
  double Tpl=stady[1];
  double error = (l3*(-w1) + l2*(-w2) + r2*w3 + r3*w4)/(l3 + l2 + m + r2 + r3);
  double Kd=15;           // 參數，手動調整
  static double lastError=0;    // 前次偏移誤差
  double dError = error - lastError;
  double powerCorrection = Kp*error + Kd*dError;
  

  int vR = Tpr - powerCorrection; // ex. Tp = 150, 也與w2 & w3有關
  int vL = Tpl + powerCorrection;
  // if(abs( error )<1){
  //   stady[0]=vL; stady[1] = vR;
  // }
  
  if(vR>255) vR = 255;
  if(vL>255) vL = 255;
  if(vR<-255) vR = -255;
  if(vL<-255) vL = -255;
  if(cmd=='m'){
    Serial3.print(vL);
    Serial3.print('|');
    Serial3.print(vR);
  }
  
    // 在所有函數外面定義一個全域變數來存最新的指令
  
  MotorWriting(vL, vR);
  static int count=0;
  if((l3 <threshold-50) && (l2 <threshold-50) && (m <threshold_m+10) && (r2 <threshold-50) && (r3 <threshold-50)){
    delay(200);
      if((l3 <threshold-50) && (l2 <threshold-50) && (m <threshold_m+10) && (r2 <threshold-50) && (r3 <threshold-50)){
          back_to_line(0);
      }
  }
  if((l3 >=threshold) && (l2 >=threshold) && (m >=threshold_m) && (r2 >=threshold) && (r3 >=threshold)) {
    if(cmd!='q'){
      
      if(cmd=='a') {
        // Serial3.print("left turn");
        left();
        back_to_line(100);
        Serial3.print('n');
      }
      if(cmd=='d'){
        // Serial3.print("right turn");
        right();
        back_to_line(-100);
        Serial3.print('n');
      }
      if(cmd=='s') {
        // Serial3.print("right turn");
        back(f);
        Serial3.print('n');
        // f();
      }
      if(cmd=='w'){
        straight();
        Serial3.print('n');
      }
      return 'q';
    }
    
    if(cmd == 'z'){
      switch(count) {
        case 0: {
          right(); count++;break;
        }
        case 1: {
          back(f); count++; break;
        }
        case 2: {
          MotorWriting(178, 200);
          delay(500);
          count++;
          break;
        }
        case 3: {
          back(f); count++; break;
        }
        case 4:{
          left(); count++; break;
        }
        case 5: {
          back(f); count=0; break;
        }
      }
    }
    
  }

  

  lastError=error;
  return cmd;
}
