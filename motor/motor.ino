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
 
  MotorWriting(178, -200);
  delay(700);
 
}
void right(){
  delay(200);
  MotorWriting(178,50); 
  delay(900);
}
void left(){
  delay(200);
  MotorWriting(45,200); 
  delay(900);
  

}
void Tracking() {
  int threshold=100;
  int threshold_m=40;
  int l3 = analogRead(analogPin5);
  int l2 = analogRead(analogPin4);
  int m = analogRead(analogPin3);
  int r2 = analogRead(analogPin2);
  int r3 = analogRead(analogPin1);
  static int count = 0;
  if ((l3 >=threshold) && (l2 <threshold) && (m <threshold_m) && (r2 <threshold) && (r3 <threshold)) {
    MotorWriting(100, 200); // very big left turn
  } else if ((l3 >=threshold) && (l2 >=threshold) && (m <threshold_m) && (r2 <threshold) && (r3 <threshold)) {
    MotorWriting(120, 200); // big left turn
  } else if ((l3 <threshold) && (l2 >=threshold) && (m <threshold_m) && (r2 <threshold) && (r3 <threshold)) {
    MotorWriting(140, 200); // left turn
  } else if ((l3 <threshold) && (l2 >=threshold) && (m >=threshold_m) && (r2 <threshold) && (r3 <threshold)) {
    MotorWriting(155, 200); // small left turn
  } else if ((l3 <threshold) && (l2 <threshold) && (m >=threshold_m) && (r2 <threshold) && (r3 <threshold)) {
    MotorWriting(178, 200); // GO straight
    // if((l3 <threshold) && (l2 <threshold) && (m <threshold_m) && (r2 <threshold) && (r3 <threshold))
    // {
    //   MotorWriting(0, 0);
    //     delay(1000);
    //     MotorWriting(-178, 200);
    //     delay(700); 
    // }
  } else if ((l3 <threshold) && (l2 <threshold) && (m >=threshold_m) && (r2 >=threshold) && (r3 <threshold)) {
    MotorWriting(178, 180); // small right turn
  } else if ((l3 <threshold) && (l2 <threshold) && (m <threshold_m) && (r2 >=threshold) && (r3 <threshold)) {
    MotorWriting(178, 160); // right turn
  } else if ((l3 <threshold) && (l2 ==LOW) && (m <threshold_m) && (r2 >=threshold) && (r3 >=threshold)) {
    MotorWriting(178, 140); // big right turn
  } else if ((l3 <threshold) && (l2 <threshold) && (m <threshold_m) && (r2 <threshold) && (r3 >=threshold)) {
    MotorWriting(178, 100); // very big right turn
  } else if ((l3 <threshold) && (l2 <threshold) && (m <threshold_m) && (r2 <threshold) && (r3 <threshold)) {
    MotorWriting(100, 100);
  } else if((l3 >=threshold) && (l2 >=threshold) && (m >=threshold_m) && (r2 >=threshold) && (r3 >=threshold)) {
    switch(count) {
      case 0: {
        right(); count++;break;
      }
      case 1: {
        back(); count++; break;
      }
      case 2: {
        delay(500); count++; break;
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
    
      // // while((l3 >=threshold) && (l2 >=threshold) && (m >=threshold_m) && (r2 >=threshold) && (r3 >=threshold)){
      //     MotorWriting(90, 100);
      //     delay(200);
      // // }
      // // if((l3 <threshold) && (l2 <threshold) && (m <threshold_m) && (r2 <threshold) && (r3 <threshold))
      // // {
        
      //     MotorWriting(0, 0);
      //     delay(1000);
      //     MotorWriting(178, -200);
      //     delay(700); 

      //     while((l3 >=threshold) && (l2 >=threshold) && (m >=threshold_m) && (r2 >=threshold) && (r3 >=threshold)) {
      //       MotorWriting(178, 200); // GO straight
      //        delay(20);
      //        if ((l3 <threshold)&& (m >=threshold_m) && (r3 <threshold)) {
      //         break;}

      //     }
      // }
      // else
      // {
      //   MotorWriting(-178, 200); // right turn
      //   delay(350); 
      //   while((l3 >=threshold) && (l2 >=threshold) && (m >=threshold_m) && (r2 >=threshold) && (r3 >=threshold)) {


      //       MotorWriting(178, 200); // GO straight
      //       delay(20);

      //     }
        
      // }
}
}
  // else {
  // MotorWriting(-60, -60);
  // }



// void MotorCheck()
// {
//   analogWrite(PWMA, 128);
//   analogWrite(PWMB, 128);
//   // test  front
//   digitalWrite(AIN1, HIGH);
// 	digitalWrite(AIN2, LOW);
// 	digitalWrite(BIN1, HIGH);
// 	digitalWrite(BIN2, LOW);
//   delay(threshold0);

//   // test  back
//  	digitalWrite(AIN1, LOW);
// 	digitalWrite(AIN2, HIGH);
// 	  digitalWrite(BIN1, LOW);
// 	digitalWrite(BIN2, HIGH);
//   delay(threshold0);

//   //left both
// 	digitalWrite(AIN1, LOW);
// 	digitalWrite(AIN2, HIGH);
// 	digitalWrite(BIN1, HIGH);
// 	digitalWrite(BIN2, LOW);
//   delay(threshold0);

//   // right both
//  	digitalWrite(AIN1, HIGH);
// 	digitalWrite(AIN2, LOW);
// 	digitalWrite(BIN1, LOW);
// 	digitalWrite(BIN2, HIGH);
//   delay(threshold0);
//   // left side
//  	digitalWrite(AIN1, LOW);
// 	digitalWrite(AIN2, LOW);
// 	digitalWrite(BIN1, HIGH);
// 	digitalWrite(BIN2, LOW);
//   delay(threshold0);
// 	// right side
// 	digitalWrite(AIN1, HIGH);
// 	digitalWrite(AIN2, LOW);
// 	digitalWrite(BIN1, LOW);
// 	digitalWrite(BIN2, LOW);
// 	delay(threshold0);

//   // stop
// 	digitalWrite(AIN1, LOW);
// 	digitalWrite(AIN2, LOW);
// 	digitalWrite(BIN1, LOW);
// 	digitalWrite(BIN2, LOW);
// 	delay(threshold0);
// }
