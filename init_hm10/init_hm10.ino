#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN 3
#define SS_PIN 2
#include "motorPID.h"
MFRC522 *mfrc522;
//motor





/*
 * HM-10 Initialization for Arduino Mega (Serial3)
 * Logic: Auto-Baud Detect -> Force Disconnect -> Restore Default -> Set Name -> Notify -> Reset
 */

// --- USER CONFIGURATION ---
#define CUSTOM_NAME "HM10_Car2"  // Max length is 12 characters [1]
// ---------------------------

long baudRates[] = { 9600, 19200, 38400, 57600, 115200, 4800, 2400, 1200, 230400 };
bool moduleReady = false;

void setup() {
  // Serial.begin(9600);
  SPI.begin();
  mfrc522 = new MFRC522(SS_PIN, RST_PIN);
  mfrc522->PCD_Init();

  // Serial.println(F("Read UID on a MIFARE PICC:"));
  //
  // Serial.begin(115200); // Debug Monitor (USB)
  while (!Serial)
    ;
  // Serial.println("Initializing HM-10...");

  // 1. Automatic Baud Rate Detection
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

  if (!moduleReady) {
    // Serial.println("Failed to detect HM-10. Check 3.3V VCC and wiring.");
    return;
  }

  // // 3. Restore Factory Defaults
  // // Serial.println("Restoring factory defaults...");
  // sendATCommand("AT+RENEW");  // Restores all setup values
  // delay(500);

  // // 4. Set Custom Name via Macro
  // // Serial.print("Setting name to: ");
  // // Serial.println(CUSTOM_NAME);
  // String nameCmd = "AT+NAME" + String(CUSTOM_NAME);
  // sendATCommand(nameCmd.c_str());  // Max length is 12

  // // 5. Enable Connection Notifications
  // // Serial.println("Enabling notifications...");
  // sendATCommand("AT+NOTI1");  // Notify when link is established/lost

  // // 6. Get the Bluetooth MAC Address
  // // Serial.println("Querying Bluetooth Address");
  // sendATCommand("AT+ADDR?");

  // // 7. Restart the module to apply changes
  // // Serial.println("Restarting module...");
  // sendATCommand("AT+RESET");  // Restart the module
  // delay(1000);
  Serial3.begin(9600);  // Now the module would use baudrate 9600

  // Serial.println("Initialization Complete.");

  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);

  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  pinMode(analogPin1, INPUT);
  pinMode(analogPin2, INPUT);
  pinMode(analogPin3, INPUT);
  pinMode(analogPin4, INPUT);
  pinMode(analogPin5, INPUT);  // 目前預設該接腳作為輸入
  // Serial.begin(9600);
}
char cmd='x';

void loop() {
  // 1. Module to PC: Forward HM-10 responses to the Serial Monitor

  if (Serial3.available()) {
    // control(Serial3.read());
    // Serial3.print(control('0'));
    cmd = Serial3.read();
    if (cmd!='b') {
    Serial3.print(cmd);
    delay(1);
      
    }
  }
  if(cmd=='r'){
    MotorWriting(0,0);
    read();
  }
  else if (cmd=='k'){
    int l3 = analogRead(analogPin5);
    int l2 = analogRead(analogPin4);
    int m = analogRead(analogPin3);
    int r2 = analogRead(analogPin2);
    int r3 = analogRead(analogPin1);
    MotorWriting(0, 0);
    Serial3.print('|');
    Serial3.print(l3);
    Serial3.print('|');

    Serial3.print(l2);
    Serial3.print('|');

    Serial3.print(m);
    Serial3.print('|');

    Serial3.print(r2);
    Serial3.print('|');

    Serial3.print(r3);
    delay(2000);
  }
  if(cmd!='x'){
    // control(Tracking(control('0'), (read)));
    cmd = Tracking(cmd, (read));
    if(cmd=='s') read();

  }
  else{
    MotorWriting(0,0); 
  }


  //   // 2. PC to Module: Read user input and truncate line endings
  //   if (Serial.available()) {
  //     static String inputBuffer = "";
  //     while (Serial.available()) {
  //       char c = Serial.read();

  //       // Check if the character is a line ending
  //       if (c == '\r' || c == '\n') {
  //         if (inputBuffer.length() > 0) {
  //           // Send the clean string to the HM-10
  //           Serial3.print(inputBuffer);

  //           // Debug: Show what was actually sent
  //           Serial.print("\n[Sent to HM-10: ");
  //           Serial.print(inputBuffer);
  //           Serial.println("]");

  //           inputBuffer = ""; // Clear buffer for next command
  //         }
  //       } else {
  //         inputBuffer += c; // Add character to buffer
  //       }
  //     }
  //   }
}

/**
 * Helper to send AT commands (Uppercase, no \r or \n) [6]
 */
void sendATCommand(const char *command) {
  Serial3.print(command);
  waitForResponse("", 1000);
}

/**
 * Helper to check response for specific substrings
 */
bool waitForResponse(const char *expected, unsigned long timeout) {
  unsigned long start = millis();
  Serial3.setTimeout(timeout);
  String response = Serial3.readString();
  if (response.length() > 0) {
    Serial.print("HM10 Response: ");
    Serial.println(response);
  }
  return (response.indexOf(expected) != -1);
}

void motorcontrol(int a) {}
void PICC_DumpDetails(Print &output, MFRC522::Uid *uid);

void read() {
  if (!mfrc522->PICC_IsNewCardPresent()) {
    return;
  }
  if (!mfrc522->PICC_ReadCardSerial()) {
    return;
  }
  Serial.println(F("**Card Detected:**"));
  PICC_DumpDetails(Serial3, &(mfrc522->uid));
  motorcontrol(0);
  mfrc522->PICC_HaltA();
  mfrc522->PCD_StopCrypto1();
  return;
}



void PICC_DumpDetails(Print &output, MFRC522::Uid *uid  ///< Pointer to Uid struct returned from a successful PICC_Select().
) {
  // UID
  // output.print(F("Card UID:"));
  for (byte i = 0; i < uid->size; i++) {
    if (uid->uidByte[i] < 0x10)
      output.print(F("0"));
    else
      output.print(F(""));
    output.print(uid->uidByte[i], HEX);
  }
  output.println();

  // SAK
  // output.print(F("Card SAK: "));
  // if(uid->sak < 0x10)
  // 	output.print(F("0"));
  // output.println(uid->sak, HEX);

  // // (suggested) PICC type
  // MFRC522::PICC_Type piccType = mfrc522->PICC_GetType(uid->sak);
  // output.print(F("PICC type: "));
  // output.println(mfrc522->PICC_GetTypeName(piccType));
}  // End PICC_DumpDetailsToSerial()

char control(char _cmd){
  
  static char cmd = 'x';
  if(_cmd=='0') return cmd;
  cmd=_cmd;
  return cmd;
}