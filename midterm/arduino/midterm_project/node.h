/***************************************************************************/
// File			  [node.h]
// Author		  [Erik Kuo, Joshua Lin]
// Synopsis		[Code for managing car movement when encounter a node]
// Functions  [/* add on your own! */]
// Modify		  [2020/03/027 Erik Kuo]
/***************************************************************************/

/*===========================import variable===========================*/
int extern _Tp;
/*===========================import variable===========================*/
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
// TODO: add some function to control your car when encounter a node
// here are something you can try: left_turn, right_turn... etc.
void back(void(*f)()){
      int threshold=100;
  int threshold_m=40;
  int l3 = analogRead(analogPin5);
  int l2 = analogRead(analogPin4);
  int m = analogRead(analogPin3);
  int r2 = analogRead(analogPin2);
  int r3 = analogRead(analogPin1);

  MotorWriting(220, -250);
  delay(300);
  f();
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
void right(void(*f)()){
  int threshold=80;
  int threshold_m=40;

  while(true){
      int l3 = analogRead(analogPin5);
      int l2 = analogRead(analogPin4);
      int m = analogRead(analogPin3);
      int r2 = analogRead(analogPin2);
      int r3 = analogRead(analogPin1);
      f();
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
void left(void(*f)()){
  int threshold=80;
  int threshold_m=40;

  while(true){
      int l3 = analogRead(analogPin5);
      int l2 = analogRead(analogPin4);
      int m = analogRead(analogPin3);
      int r2 = analogRead(analogPin2);
      int r3 = analogRead(analogPin1);
      f();
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
      f();
      if((l2 >=threshold) || (m >=threshold_m) || (r2 >=threshold))
        break;
      MotorWriting(0, 200);
    }

}

void enterblack(int& count, int& cmd, void(*f)()){
    if(cmd!='q'){
      if(cmd=='w') ;
      if(cmd=='a') left(f);
      if(cmd=='d') right(f);
      if(cmd=='s') back(f);
      cmd='q';
    }
    else{
      switch(count) {
        case 0: {
          right(f); count++;break;
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
          left(f); count++; break;
        }
        case 5: {
          back(f); count=0; break;
        }
      }
      }
      
    
}