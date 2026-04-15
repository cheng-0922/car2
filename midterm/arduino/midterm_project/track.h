/***************************************************************************/
// File			  [track.h]
// Author		  [Erik Kuo]
// Synopsis		[Code used for tracking]
// Functions  [MotorWriting, MotorInverter, tracking]
// Modify		  [2020/03/27 Erik Kuo]
/***************************************************************************/

/*if you have no idea how to start*/
/*check out what you have learned from week 1 & 6*/
/*feel free to add your own function for convenience*/

/*===========================import variable===========================*/
int extern _Tp;
/*===========================import variable===========================*/
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
// Write the voltage to motor.
void MotorWriting(double vL, double vR) {
    // TODO: use TB6612 to control motor voltage & direction
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
}  // MotorWriting

// Handle negative motor_PWMR value.
// void MotorInverter(int motor, bool& dir) {
//     // Hint: the value of motor_PWMR must between 0~255, cannot write negative value.
// }  // MotorInverter

// P/PID control Tracking
void tracking(int l2, int l1, int m0, int r1, int r2) {
    int threshold=80;
    int threshold_m=40;
    // TODO: find your own parameters!
    double _w0=0;  //
    double _w1=1;  //
    double _w2=4;  //
    double _w4=5;
    double _Kp=100;  // p term parameter
    double _Kd=15;  // d term parameter (optional)
    double _Ki=0;  // i term parameter (optional) (Hint: 不要調太大)
    double error = l2 * _w2 + l1 * _w1 + m0 * _w0 + r1 * (-_w1) + r2 * (-_w2);
    double vR=250, vL=220;  // 馬達左右轉速原始值(從PID control 計算出來)。Between -255 to 255.
    double adj_R = 1, adj_L = 1;  // 馬達轉速修正係數。MotorWriting(_Tp,_Tp)如果歪掉就要用參數修正。
    
    
    // TODO: complete your P/PID tracking code
    static double lastError=0;    // 前次偏移誤差s
    double dError = error - lastError;
    double powerCorrection = _Kp*error + _Kd*dError;
    vR-=powerCorrection;
    vL+=powerCorrection;

    // end TODO
    if(vR>255) vR = 255;
    if(vL>231) vL = 231;
    if(vR<-255) vR = -255;
    if(vL<-231) vL = -231;
    MotorWriting(adj_L * vL, adj_R * vR);

    static int count=0;

    lastError=error;
}  // tracking

  

