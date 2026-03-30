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
#define analogPin2 A4
#define analogPin3 A5
#define analogPin4 A6
#define analogPin5 A7

void back();
void right();
void setup() {
	
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(analogPin1 , INPUT);
  pinMode(analogPin2 , INPUT);
  pinMode(analogPin3 , INPUT);
  pinMode(analogPin4 , INPUT);
  pinMode(analogPin5 , INPUT); // 目前預設該接腳作為輸入
  Serial.begin(9600);

}

void loop() {
  // analogWrite(PWMA, 128);
  // analogWrite(PWMB, 128);

  Tracking();
//   int sensorValue1 = analogRead(analogPin1);
// int sensorValue2 = analogRead(analogPin2);
// int sensorValue3 = analogRead(analogPin3);
// int sensorValue4 = analogRead(analogPin4);
// int sensorValue5 = analogRead(analogPin5);
//  Serial.println(sensorValue1);


//  Serial.println(sensorValue2);
// // Serial.println(F("**Sensor2 Detected:**"));

// Serial.println(sensorValue3);
// // Serial.println(F("**Sensor3 Detected:**"));

// Serial.println(sensorValue4);
// // Serial.println(F("**Sensor4 Detected:**"));

// Serial.println(sensorValue5);
// Serial.println(F("**Sensor5 Detected:**"));
// delay(500);
// // 

}

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
void back(){
      int threshold=100;
  int threshold_m=40;
  int l3 = analogRead(analogPin5);
  int l2 = analogRead(analogPin4);
  int m = analogRead(analogPin3);
  int r2 = analogRead(analogPin2);
  int r3 = analogRead(analogPin1);

  MotorWriting(220, -250);
  delay(300);
  while(true){
      int l3 = analogRead(analogPin5);
      int l2 = analogRead(analogPin4);
      int m = analogRead(analogPin3);
      int r2 = analogRead(analogPin2);
      int r3 = analogRead(analogPin1);
      if((l2 >=threshold) || (m >=threshold_m) || (r2 >=threshold))
        break;
      MotorWriting(178, -200);
  }

  
}
void straight(){
  
}
void right(){
  int threshold=80;
  int threshold_m=40;

  while(true){
      int l3 = analogRead(analogPin5);
      int l2 = analogRead(analogPin4);
      int m = analogRead(analogPin3);
      int r2 = analogRead(analogPin2);
      int r3 = analogRead(analogPin1);
      if(!((l3 >=threshold) && (l2 >=threshold) && (m >=threshold_m) && (r2 >=threshold) && (r3 >=threshold)))
        break;
      MotorWriting(178, 200);
    }
  MotorWriting(200,0); 
  delay(700);
  while(true){
      int l3 = analogRead(analogPin5);
      int l2 = analogRead(analogPin4);
      int m = analogRead(analogPin3);
      int r2 = analogRead(analogPin2);
      int r3 = analogRead(analogPin1);
      if((l2 >=threshold) || (m >=threshold_m) || (r2 >=threshold))
        break;
      MotorWriting(178,0); 
    }
  

}
void left(){
  int threshold=80;
  int threshold_m=40;

  while(true){
      int l3 = analogRead(analogPin5);
      int l2 = analogRead(analogPin4);
      int m = analogRead(analogPin3);
      int r2 = analogRead(analogPin2);
      int r3 = analogRead(analogPin1);
      if(!((l3 >=threshold) && (l2 >=threshold) && (m >=threshold_m) && (r2 >=threshold) && (r3 >=threshold)))
        break;
      MotorWriting(178, 200);
    }
  
  MotorWriting(0,255); 
  delay(400);
  while(true){
      int l3 = analogRead(analogPin5);
      int l2 = analogRead(analogPin4);
      int m = analogRead(analogPin3);
      int r2 = analogRead(analogPin2);
      int r3 = analogRead(analogPin1);
      if((l2 >=threshold) || (m >=threshold_m) || (r2 >=threshold))
        break;
      MotorWriting(0, 200);
    }

}
void Tracking() {
  int threshold=100;
  int threshold_m=40;
  int l3 = analogRead(analogPin5);
  int l2 = analogRead(analogPin4);
  int m = analogRead(analogPin3);
  int r2 = analogRead(analogPin2);
  int r3 = analogRead(analogPin1);
  double w1= 5;
  double w2= 1;
  double w3= 1;
  double w4= 4;
  double Kp= 100;
  static double stady[2] = {250,220};
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
  if(vL>231) vL = 231;
  if(vR<-255) vR = -255;
  if(vL<-231) vL = -231;
  MotorWriting(vL, vR);
  static int count=0;
  if((l3 >=threshold) && (l2 >=threshold) && (m >=threshold_m) && (r2 >=threshold) && (r3 >=threshold)) {
    switch(count) {
      case 0: {
        
        right(); count++;break;
      }
      case 1: {
        back(); count++; break;
      }
      case 2: {
        MotorWriting(178, 200);
        delay(500);
        count++;
        break;
      }
      case 3: {
        back(); count++; break;
      }
      case 4:{
        left(); count++; break;
      }
      case 5: {
        back(); count=0; break;
      }
    }
  }
  lastError=error;
}