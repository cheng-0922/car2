/***************************************************************************/
// File       [final_project.ino]
// Author     [Erik Kuo]
// Synopsis   [Code for managing main process]
// Functions  [setup, loop, Search_Mode, Hault_Mode, SetState]
// Modify     [2020/03/27 Erik Kuo]
/***************************************************************************/

#define DEBUG  // debug flag

// for RFID
#include <MFRC522.h>
#include <SPI.h>

/*===========================define pin & create module object================================*/
// BlueTooth
// BT connect to Serial1 (Hardware Serial)
// Mega               HC05
// Pin  (Function)    Pin
// 18    TX       ->  RX
// 19    RX       <-  TX
// TB6612, 請按照自己車上的接線寫入腳位(左右不一定要跟註解寫的一樣)
// TODO: 請將腳位寫入下方
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
#define RST_PIN 3
#define SS_PIN 2
// BT
#define CUSTOM_NAME "HM10_Car2"   // Max length is 12 characters [1]

/*===========================define pin & create module object===========================*/

/*============setup============*/
void setup() {
    // bluetooth initialization
    Serial3.begin(9600);  
    // Serial window
    Serial.begin(9600);
    // RFID initial
    SPI.begin();
    mfrc522.PCD_Init();
    // TB6612 pin
    pinMode(PWMA, OUTPUT);
    pinMode(PWMB, OUTPUT);

    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);

    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    // tracking pin
    pinMode(analogPin1 , INPUT);
    pinMode(analogPin2 , INPUT);
    pinMode(analogPin3 , INPUT);
    pinMode(analogPin4 , INPUT);
    pinMode(analogPin5 , INPUT);
    mfrc522 = new MFRC522(SS_PIN, RST_PIN);


    for (int i = 0; i < 9; i++) {
    // Serial.print("Testing baud rate: ");
    // Serial.println(baudRates[i]);

    Serial3.begin(baudRates[i]);
    Serial3.setTimeout(100);
    delay(100);

    // 2. Force Disconnection
    // Sending "AT" while connected forces the module to disconnect [2].
    Serial3.print("AT");

    if (waitForResponse("OK", 800)) {
      // Serial.println("HM-10 detected and ready.");
      moduleReady = true;
      break;
    } else {
      Serial3.end();
      delay(100);
    }
  }
#ifdef DEBUG
    Serial.println("Start!");
#endif
}
/*============setup============*/

/*=====Import header files=====*/
#include "RFID.h"
#include "bluetooth.h"
#include "node.h"
#include "track.h"
/*=====Import header files=====*/

/*===========================initialize variables===========================*/
int l2 = 0, l1 = 0, m0 = 0, r1 = 0, r2 = 0;  // 紅外線模組的讀值(0->white,1->black)
int _Tp = 90;                                // set your own value for motor power
bool state = false;     // set state to false to halt the car, set state to true to activate the car
BT_CMD _cmd = NOTHING;  // enum for bluetooth message, reference in bluetooth.h line 2
/*===========================initialize variables===========================*/

/*===========================declare function prototypes===========================*/
void Search();    // search graph
void SetState();  // switch the state
/*===========================declare function prototypes===========================*/

/*===========================define function===========================*/
void loop() {
    if (!state)
        MotorWriting(0, 0);
    else
        Search();
    SetState();
}

void SetState() {
    // TODO:
    // 1. Get command from bluetooth
    // 2. Change state if need
    _cmd=ask_BT();

}

void Search() {
    // TODO: let your car search graph(maze) according to bluetooth command from computer(python
    // code)
    int threshold=100;
    int threshold_m=40;
    int l3 = analogRead(analogPin5);
    int l2 = analogRead(analogPin4);
    int m = analogRead(analogPin3);
    int r2 = analogRead(analogPin2);
    int r3 = analogRead(analogPin1);
    if((l3 >=threshold) && (l2 >=threshold) && (m >=threshold_m) && (r2 >=threshold) && (r3 >=threshold)) {
        enterblack(count, cmd, send_byte());
    }
    else{
        tracking(l3,l2,m,r2,r3);
    }
}
/*===========================define function===========================*/


