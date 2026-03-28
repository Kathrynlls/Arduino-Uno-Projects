#include <Servo.h>
#include <IRremote.hpp>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

int ENA = 6;
int IN1 = 8;
int LED_LOW = 7;
int LED_MEDIUM = 9;
int LED_HIGH = 5;
int BUZZER = 3;
int ServoPin = 10;

// LCD I2C Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Keypad setup
const byte ROWS = 4; 
const byte COLS = 3;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {A3, A2, A1, A0};
byte colPins[COLS] = {11,12,13};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//IR Remote Codes
#define Unli50 0xBA45FF00 
#define Unli75 0xB946FF00  
#define Unli100 0xB847FF00  
#define Stop 0xBC43FF00 
#define Timer0 0xE916FF00  
#define Timer1 0xF30CFF00
#define Timer2 0xE718FF00
#define Timer3 0xA15EFF00
#define Timer4 0xF708FF00
#define Timer5 0xE31CFF00
#define Timer6 0xA55AFF00

Servo tiltServo;
int speedState = 50;
bool isMixing = false;
unsigned long mixTime = 0;
unsigned long startTime = 0;
bool unlimitedMode = false;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(2, ENABLE_LED_FEEDBACK);
  Serial.println("Ir Receiver Ready");

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);

  pinMode(LED_LOW, OUTPUT);
  pinMode(LED_MEDIUM, OUTPUT);
  pinMode(LED_HIGH, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  tiltServo.attach(ServoPin);
  tiltServo.write(15);

  lcd.init();
  lcd.backlight();
  resetToIdle();
}

void loop(){
  char customKey = customKeypad.getKey();
  if (customKey != NO_KEY){
    if (customKey == '0'){
      startTime = millis(); // Record the start time
      mixTime = 30000; // Set mixing time to 30 seconds 
      isMixing = true;
      unlimitedMode = false;

      int prevSpeedState = 0; // Track previous speed to update LCD only when needed

      while (millis() - startTime < mixTime) {
          unsigned long elapsedTime = millis() - startTime;

          if (elapsedTime < 10000) { // 1-20 seconds, 100% speed
              speedState = 100;
              analogWrite(ENA, 255);
          } else if (elapsedTime < 20000) { // 20-40 seconds, 75% speed
              speedState = 75;
              analogWrite(ENA, 192);
          } else { // 40-60 seconds, 50% speed
              speedState = 50;
              analogWrite(ENA, 127);
          }

          digitalWrite(IN1, HIGH);

          if (speedState == 50) {
            setColor(0, 255, 0); // Set color to GREEN for speedState 50
          } else if (speedState == 75) {
            setColor(255, 255, 0); // Set color to YELLOW for speedState 75
          } else if (speedState == 100) {
            setColor(255, 0, 0); // Set color to RED for speedState 100
          } else {
            setColor(0, 0, 0); // Turn off the LED for any other speedState
          }

          tiltServo.write(40);

          // Update LCD only if speed changed
          if (speedState != prevSpeedState) {
              updateLCD();
              prevSpeedState = speedState; // Update previous speed state
          }

          if (millis() - startTime >= mixTime) break;
      }
        stopMixing(); 
    } else if (customKey == '1'){
      startTime = millis(); // Record the start time
      mixTime = 60000; // Set mixing time to 60 seconds (1 minute)
      isMixing = true;
      unlimitedMode = false;

      int prevSpeedState = 0; // Track previous speed to update LCD only when needed

      while (millis() - startTime < mixTime) {
          unsigned long elapsedTime = millis() - startTime;

          if (elapsedTime < 20000) { // 1-20 seconds, 100% speed
              speedState = 100;
              analogWrite(ENA, 255);
          } else if (elapsedTime < 40000) { // 20-40 seconds, 75% speed
              speedState = 75;
              analogWrite(ENA, 192);
          } else { // 40-60 seconds, 50% speed
              speedState = 50;
              analogWrite(ENA, 127);
          }

          digitalWrite(IN1, HIGH);
          if (speedState == 50) {
            setColor(0, 255, 0); // Set color to GREEN for speedState 50
          } else if (speedState == 75) {
            setColor(255, 255, 0); // Set color to YELLOW for speedState 75
          } else if (speedState == 100) {
            setColor(255, 0, 0); // Set color to RED for speedState 100
          } else {
            setColor(0, 0, 0); // Turn off the LED for any other speedState
          }
          tiltServo.write(40);
          // Update LCD only if speed changed
          if (speedState != prevSpeedState) {
              updateLCD();
              prevSpeedState = speedState; // Update previous speed state
          }

          if (millis() - startTime >= mixTime) break;
      }
        stopMixing(); // Stop mixing after 60 seconds
    } else if (customKey == '2'){
      startTime = millis(); // Record the start time
      mixTime = 120000; // Set mixing time to 120 seconds (2 minute)
      isMixing = true;
      unlimitedMode = false;

      int prevSpeedState = 0; // Track previous speed to update LCD only when needed

      while (millis() - startTime < mixTime) {
          unsigned long elapsedTime = millis() - startTime;

          if (elapsedTime < 40000) { // 1-20 seconds, 100% speed
              speedState = 100;
              analogWrite(ENA, 255);
          } else if (elapsedTime < 80000) { // 20-40 seconds, 75% speed
              speedState = 75;
              analogWrite(ENA, 192);
          } else { // 40-60 seconds, 50% speed
              speedState = 50;
              analogWrite(ENA, 127);
          }

          digitalWrite(IN1, HIGH);
          if (speedState == 50) {
            setColor(0, 255, 0); // Set color to GREEN for speedState 50
          } else if (speedState == 75) {
            setColor(255, 255, 0); // Set color to YELLOW for speedState 75
          } else if (speedState == 100) {
            setColor(255, 0, 0); // Set color to RED for speedState 100
          } else {
            setColor(0, 0, 0); // Turn off the LED for any other speedState
          }
          tiltServo.write(40);

          // Update LCD only if speed changed
          if (speedState != prevSpeedState) {
              updateLCD();
              prevSpeedState = speedState; // Update previous speed state
          }

          if (millis() - startTime >= mixTime) break;
      }
        stopMixing(); // Stop mixing after 60 seconds

    } else if (customKey == '3'){
      startTime = millis(); // Record the start time
      mixTime = 180000; // Set mixing time to 120 seconds (2 minute)
      isMixing = true;
      unlimitedMode = false;

      int prevSpeedState = 0; // Track previous speed to update LCD only when needed

      while (millis() - startTime < mixTime) {
          unsigned long elapsedTime = millis() - startTime;

          if (elapsedTime < 40000) { // 1-20 seconds, 100% speed
              speedState = 100;
              analogWrite(ENA, 255);
          } else if (elapsedTime < 120000) { // 20-40 seconds, 75% speed
              speedState = 75;
              analogWrite(ENA, 192);
          } else { // 40-60 seconds, 50% speed
              speedState = 50;
              analogWrite(ENA, 127);
          }

          digitalWrite(IN1, HIGH);
          if (speedState == 50) {
            setColor(0, 255, 0); // Set color to GREEN for speedState 50
          } else if (speedState == 75) {
            setColor(255, 255, 0); // Set color to YELLOW for speedState 75
          } else if (speedState == 100) {
            setColor(255, 0, 0); // Set color to RED for speedState 100
          } else {
            setColor(0, 0, 0); // Turn off the LED for any other speedState
          }
          tiltServo.write(40);

          // Update LCD only if speed changed
          if (speedState != prevSpeedState) {
              updateLCD();
              prevSpeedState = speedState; // Update previous speed state
          }

          if (millis() - startTime >= mixTime) break;
      }
        stopMixing(); // Stop mixing after 60 seconds

    } else if (customKey == '4'){
      startTime = millis(); // Record the start time
      mixTime = 240000; // Set mixing time to 120 seconds (2 minute)
      isMixing = true;
      unlimitedMode = false;

      int prevSpeedState = 0; // Track previous speed to update LCD only when needed

      while (millis() - startTime < mixTime) {
          unsigned long elapsedTime = millis() - startTime;

          if (elapsedTime < 60000) { // 1-20 seconds, 100% speed
              speedState = 100;
              analogWrite(ENA, 255);
          } else if (elapsedTime < 160000) { // 20-40 seconds, 75% speed
              speedState = 75;
              analogWrite(ENA, 192);
          } else { // 40-60 seconds, 50% speed
              speedState = 50;
              analogWrite(ENA, 127);
          }

          digitalWrite(IN1, HIGH);
          if (speedState == 50) {
            setColor(0, 255, 0); // Set color to GREEN for speedState 50
          } else if (speedState == 75) {
            setColor(255, 255, 0); // Set color to YELLOW for speedState 75
          } else if (speedState == 100) {
            setColor(255, 0, 0); // Set color to RED for speedState 100
          } else {
            setColor(0, 0, 0); // Turn off the LED for any other speedState
          }
          tiltServo.write(40);

          // Update LCD only if speed changed
          if (speedState != prevSpeedState) {
              updateLCD();
              prevSpeedState = speedState; // Update previous speed state
          }

          if (millis() - startTime >= mixTime) break;
      }
        stopMixing(); 

    } else if (customKey == '5'){
      startTime = millis(); // Record the start time
      mixTime = 300000; // Set mixing time to 300 seconds (5 minute)
      isMixing = true;
      unlimitedMode = false;

      int prevSpeedState = 0; // Track previous speed to update LCD only when needed

      while (millis() - startTime < mixTime) {
          unsigned long elapsedTime = millis() - startTime;

          if (elapsedTime < 80000) { // 1-20 seconds, 100% speed
              speedState = 100;
              analogWrite(ENA, 255);
          } else if (elapsedTime < 200000) { // 20-40 seconds, 75% speed
              speedState = 75;
              analogWrite(ENA, 192);
          } else { // 40-60 seconds, 50% speed
              speedState = 50;
              analogWrite(ENA, 127);
          }

          digitalWrite(IN1, HIGH);
          if (speedState == 50) {
            setColor(0, 255, 0); // Set color to GREEN for speedState 50
          } else if (speedState == 75) {
            setColor(255, 255, 0); // Set color to YELLOW for speedState 75
          } else if (speedState == 100) {
            setColor(255, 0, 0); // Set color to RED for speedState 100
          } else {
            setColor(0, 0, 0); // Turn off the LED for any other speedState
          }
          tiltServo.write(40);

          // Update LCD only if speed changed
          if (speedState != prevSpeedState) {
              updateLCD();
              prevSpeedState = speedState; // Update previous speed state
          }

          if (millis() - startTime >= mixTime) break;
      }
        stopMixing(); 

    } else if (customKey == '6'){
      startTime = millis(); // Record the start time
      mixTime = 360000; // Set mixing time to 360 seconds (6 minute)
      isMixing = true;
      unlimitedMode = false;

      int prevSpeedState = 0; // Track previous speed to update LCD only when needed

      while (millis() - startTime < mixTime) {
          unsigned long elapsedTime = millis() - startTime;

          if (elapsedTime < 100000) { // 1-20 seconds, 100% speed
              speedState = 100;
              analogWrite(ENA, 255);
          } else if (elapsedTime < 240000) { // 20-40 seconds, 75% speed
              speedState = 75;
              analogWrite(ENA, 192);
          } else { // 40-60 seconds, 50% speed
              speedState = 50;
              analogWrite(ENA, 127);
          }

          digitalWrite(IN1, HIGH);
          if (speedState == 50) {
            setColor(0, 255, 0); // Set color to GREEN for speedState 50
          } else if (speedState == 75) {
            setColor(255, 255, 0); // Set color to YELLOW for speedState 75
          } else if (speedState == 100) {
            setColor(255, 0, 0); // Set color to RED for speedState 100
          } else {
            setColor(0, 0, 0); // Turn off the LED for any other speedState
          }
          tiltServo.write(40);

          // Update LCD only if speed changed
          if (speedState != prevSpeedState) {
              updateLCD();
              prevSpeedState = speedState; // Update previous speed state
          }

          if (millis() - startTime >= mixTime) break;
      }
        stopMixing();

    } else if (customKey == '7'){
      digitalWrite(IN1, HIGH);
      analogWrite(ENA, 127);
      tiltServo.write(40);
      speedState = 50;
      startMixing();

    } else if (customKey == '8'){
      digitalWrite(IN1, HIGH);
      analogWrite(ENA, 192);
      tiltServo.write(40);
      speedState = 75;
      startMixing();

    } else if (customKey == '9') {
      digitalWrite(IN1, HIGH);
      tiltServo.write(40);
      analogWrite(ENA, 192);
      delay(200);
      analogWrite(ENA, 255);
      speedState = 100;
      startMixing();

    } else if (customKey == '*'){
      digitalWrite(IN1, LOW);
      analogWrite(ENA, 0);
      stopMixing();

    }
  }
  if (IrReceiver.decode()) {
    uint32_t decodedValue = IrReceiver.decodedIRData.decodedRawData;
    Serial.print("Decoded Data: ");
    Serial.println(decodedValue, HEX);

    if (decodedValue == Unli50){
      digitalWrite(IN1, HIGH);
      tiltServo.write(40);
      analogWrite(ENA, 127);
      speedState = 50;
      startMixing();
    }

    if (decodedValue == Unli75){
      digitalWrite(IN1, HIGH);
      tiltServo.write(40);
      analogWrite(ENA, 192);
      speedState = 75;
      startMixing();
    }

    if (decodedValue == Unli100){
      digitalWrite(IN1, HIGH);
      tiltServo.write(40);
      analogWrite(ENA, 192);
      delay(200);
      analogWrite(ENA, 255);
      speedState = 100;
      startMixing();
    }
    
    if (decodedValue == Stop){
      digitalWrite(IN1, LOW);
      analogWrite(ENA, 0);
      stopMixing();
    }

    if (decodedValue == Timer1) {
      startTime = millis(); // Record the start time
      mixTime = 60000; // Set mixing time to 60 seconds (1 minute)
      isMixing = true;
      unlimitedMode = false;

      int prevSpeedState = 0; // Track previous speed to update LCD only when needed

      while (millis() - startTime < mixTime) {
          unsigned long elapsedTime = millis() - startTime;

          if (elapsedTime < 20000) { // 1-20 seconds, 100% speed
              speedState = 100;
              analogWrite(ENA, 255);
          } else if (elapsedTime < 40000) { // 20-40 seconds, 75% speed
              speedState = 75;
              analogWrite(ENA, 192);
          } else { // 40-60 seconds, 50% speed
              speedState = 50;
              analogWrite(ENA, 127);
          }

          digitalWrite(IN1, HIGH);
          if (speedState == 50) {
            setColor(0, 255, 0); // Set color to GREEN for speedState 50
          } else if (speedState == 75) {
            setColor(255, 255, 0); // Set color to YELLOW for speedState 75
          } else if (speedState == 100) {
            setColor(255, 0, 0); // Set color to RED for speedState 100
          } else {
            setColor(0, 0, 0); // Turn off the LED for any other speedState
          }
          tiltServo.write(40);

          // Update LCD only if speed changed
          if (speedState != prevSpeedState) {
              updateLCD();
              prevSpeedState = speedState; // Update previous speed state
          }

          if (millis() - startTime >= mixTime) break;
      }
        stopMixing(); // Stop mixing after 60 seconds
      }

      if (decodedValue == Timer2) {
      startTime = millis(); // Record the start time
      mixTime = 120000; // Set mixing time to 120 seconds (2 minute)
      isMixing = true;
      unlimitedMode = false;

      int prevSpeedState = 0; // Track previous speed to update LCD only when needed

      while (millis() - startTime < mixTime) {
          unsigned long elapsedTime = millis() - startTime;

          if (elapsedTime < 40000) { // 1-20 seconds, 100% speed
              speedState = 100;
              analogWrite(ENA, 255);
          } else if (elapsedTime < 80000) { // 20-40 seconds, 75% speed
              speedState = 75;
              analogWrite(ENA, 192);
          } else { // 40-60 seconds, 50% speed
              speedState = 50;
              analogWrite(ENA, 127);
          }

          digitalWrite(IN1, HIGH);
          if (speedState == 50) {
            setColor(0, 255, 0); // Set color to GREEN for speedState 50
          } else if (speedState == 75) {
            setColor(255, 255, 0); // Set color to YELLOW for speedState 75
          } else if (speedState == 100) {
            setColor(255, 0, 0); // Set color to RED for speedState 100
          } else {
            setColor(0, 0, 0); // Turn off the LED for any other speedState
          }
          tiltServo.write(40);

          // Update LCD only if speed changed
          if (speedState != prevSpeedState) {
              updateLCD();
              prevSpeedState = speedState; // Update previous speed state
          }

          if (millis() - startTime >= mixTime) break;
      }
        stopMixing(); // Stop mixing after 60 seconds
      }

      if (decodedValue == Timer3) {
      startTime = millis(); // Record the start time
      mixTime = 180000; // Set mixing time to 120 seconds (2 minute)
      isMixing = true;
      unlimitedMode = false;

      int prevSpeedState = 0; // Track previous speed to update LCD only when needed

      while (millis() - startTime < mixTime) {
          unsigned long elapsedTime = millis() - startTime;

          if (elapsedTime < 40000) { // 1-20 seconds, 100% speed
              speedState = 100;
              analogWrite(ENA, 255);
          } else if (elapsedTime < 120000) { // 20-40 seconds, 75% speed
              speedState = 75;
              analogWrite(ENA, 192);
          } else { // 40-60 seconds, 50% speed
              speedState = 50;
              analogWrite(ENA, 127);
          }

          digitalWrite(IN1, HIGH);
          if (speedState == 50) {
            setColor(0, 255, 0); // Set color to GREEN for speedState 50
          } else if (speedState == 75) {
            setColor(255, 255, 0); // Set color to YELLOW for speedState 75
          } else if (speedState == 100) {
            setColor(255, 0, 0); // Set color to RED for speedState 100
          } else {
            setColor(0, 0, 0); // Turn off the LED for any other speedState
          }
          tiltServo.write(40);

          // Update LCD only if speed changed
          if (speedState != prevSpeedState) {
              updateLCD();
              prevSpeedState = speedState; // Update previous speed state
          }

          if (millis() - startTime >= mixTime) break;
      }
        stopMixing(); // Stop mixing after 60 seconds
      }

      if (decodedValue == Timer4) {
      startTime = millis(); // Record the start time
      mixTime = 240000; // Set mixing time to 120 seconds (2 minute)
      isMixing = true;
      unlimitedMode = false;

      int prevSpeedState = 0; // Track previous speed to update LCD only when needed

      while (millis() - startTime < mixTime) {
          unsigned long elapsedTime = millis() - startTime;

          if (elapsedTime < 60000) { // 1-20 seconds, 100% speed
              speedState = 100;
              analogWrite(ENA, 255);
          } else if (elapsedTime < 160000) { // 20-40 seconds, 75% speed
              speedState = 75;
              analogWrite(ENA, 192);
          } else { // 40-60 seconds, 50% speed
              speedState = 50;
              analogWrite(ENA, 127);
          }

          digitalWrite(IN1, HIGH);
          if (speedState == 50) {
            setColor(0, 255, 0); // Set color to GREEN for speedState 50
          } else if (speedState == 75) {
            setColor(255, 255, 0); // Set color to YELLOW for speedState 75
          } else if (speedState == 100) {
            setColor(255, 0, 0); // Set color to RED for speedState 100
          } else {
            setColor(0, 0, 0); // Turn off the LED for any other speedState
          }
          tiltServo.write(40);

          // Update LCD only if speed changed
          if (speedState != prevSpeedState) {
              updateLCD();
              prevSpeedState = speedState; // Update previous speed state
          }

          if (millis() - startTime >= mixTime) break;
      }
        stopMixing(); 
      }

      if (decodedValue == Timer5) {
      startTime = millis(); // Record the start time
      mixTime = 300000; // Set mixing time to 300 seconds (5 minute)
      isMixing = true;
      unlimitedMode = false;

      int prevSpeedState = 0; // Track previous speed to update LCD only when needed

      while (millis() - startTime < mixTime) {
          unsigned long elapsedTime = millis() - startTime;

          if (elapsedTime < 80000) { // 1-20 seconds, 100% speed
              speedState = 100;
              analogWrite(ENA, 255);
          } else if (elapsedTime < 200000) { // 20-40 seconds, 75% speed
              speedState = 75;
              analogWrite(ENA, 192);
          } else { // 40-60 seconds, 50% speed
              speedState = 50;
              analogWrite(ENA, 127);
          }

          digitalWrite(IN1, HIGH);
          if (speedState == 50) {
            setColor(0, 255, 0); // Set color to GREEN for speedState 50
          } else if (speedState == 75) {
            setColor(255, 255, 0); // Set color to YELLOW for speedState 75
          } else if (speedState == 100) {
            setColor(255, 0, 0); // Set color to RED for speedState 100
          } else {
            setColor(0, 0, 0); // Turn off the LED for any other speedState
          }
          tiltServo.write(40);

          // Update LCD only if speed changed
          if (speedState != prevSpeedState) {
              updateLCD();
              prevSpeedState = speedState; // Update previous speed state
          }

          if (millis() - startTime >= mixTime) break;
      }
        stopMixing(); 
      }

      if (decodedValue == Timer6) {
      startTime = millis(); // Record the start time
      mixTime = 360000; // Set mixing time to 360 seconds (6 minute)
      isMixing = true;
      unlimitedMode = false;

      int prevSpeedState = 0; // Track previous speed to update LCD only when needed

      while (millis() - startTime < mixTime) {
          unsigned long elapsedTime = millis() - startTime;

          if (elapsedTime < 100000) { // 1-20 seconds, 100% speed
              speedState = 100;
              analogWrite(ENA, 255);
          } else if (elapsedTime < 240000) { // 20-40 seconds, 75% speed
              speedState = 75;
              analogWrite(ENA, 192);
          } else { // 40-60 seconds, 50% speed
              speedState = 50;
              analogWrite(ENA, 127);
          }

          digitalWrite(IN1, HIGH);
          if (speedState == 50) {
            setColor(0, 255, 0); // Set color to GREEN for speedState 50
          } else if (speedState == 75) {
            setColor(255, 255, 0); // Set color to YELLOW for speedState 75
          } else if (speedState == 100) {
            setColor(255, 0, 0); // Set color to RED for speedState 100
          } else {
            setColor(0, 0, 0); // Turn off the LED for any other speedState
          }
          tiltServo.write(40);

          // Update LCD only if speed changed
          if (speedState != prevSpeedState) {
              updateLCD();
              prevSpeedState = speedState; // Update previous speed state
          }

          if (millis() - startTime >= mixTime) break;
      }
        stopMixing();
      }

      if (decodedValue == Timer0) {
      startTime = millis(); // Record the start time
      mixTime = 30000; // Set mixing time to 30 seconds 
      isMixing = true;
      unlimitedMode = false;

      int prevSpeedState = 0; // Track previous speed to update LCD only when needed

      while (millis() - startTime < mixTime) {
          unsigned long elapsedTime = millis() - startTime;

          if (elapsedTime < 10000) { // 1-20 seconds, 100% speed
              speedState = 100;
              analogWrite(ENA, 255);
          } else if (elapsedTime < 20000) { // 20-40 seconds, 75% speed
              speedState = 75;
              analogWrite(ENA, 192);
          } else { // 40-60 seconds, 50% speed
              speedState = 50;
              analogWrite(ENA, 127);
          }

          digitalWrite(IN1, HIGH);
          if (speedState == 50) {
            setColor(0, 255, 0); // Set color to GREEN for speedState 50
          } else if (speedState == 75) {
            setColor(255, 255, 0); // Set color to YELLOW for speedState 75
          } else if (speedState == 100) {
            setColor(255, 0, 0); // Set color to RED for speedState 100
          } else {
            setColor(0, 0, 0); // Turn off the LED for any other speedState
          }
          tiltServo.write(40);

          // Update LCD only if speed changed
          if (speedState != prevSpeedState) {
              updateLCD();
              prevSpeedState = speedState; // Update previous speed state
          }

          if (millis() - startTime >= mixTime) break;
      }
        stopMixing(); 
      }



    IrReceiver.resume();
  }
}

void startMixing(){
  isMixing = true;
  startTime = millis();
  unlimitedMode = (mixTime == 0);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MIXING STARTED");
  lcd.setCursor(0,1);
  lcd.print("SPEED: ");
  lcd.print(speedState);
  lcd.print("%");

    if (speedState == 50) {
    setColor(0, 255, 0); // Set color to GREEN for speedState 50
  } else if (speedState == 75) {
    setColor(255, 255, 0); // Set color to YELLOW for speedState 75
  } else if (speedState == 100) {
    setColor(255, 0, 0); // Set color to RED for speedState 100
  } else {
    setColor(0, 0, 0); // Turn off the LED for any other speedState
  }
}

void stopMixing() {
  analogWrite(ENA, 0);
  tiltServo.write(15);
  isMixing = false;
  IrReceiver.stop();  // Temporarily disable IR receiver
  tone(BUZZER, 1000, 500);
  delay(500);
  noTone(BUZZER);
  IrReceiver.begin(2, ENABLE_LED_FEEDBACK);  // Re-enable IR receiver
  setColor(0,0,0);
  resetToIdle();
}

void resetToIdle() {
  isMixing = false;
  speedState = 50;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Select Speed:");
  lcd.setCursor(13,0);
  lcd.print(speedState);
  lcd.print("%");
  lcd.setCursor(0,1);
  lcd.print("Mixer Ready");
}

void updateLCD() {
    lcd.setCursor(0, 0);
    lcd.print("MIXING TIMER MODE  "); // Extra spaces to overwrite old text
    lcd.setCursor(0, 1);
    lcd.print("SPEED: ");
    lcd.print(speedState);
    lcd.print("%  "); // Extra spaces to overwrite previous numbers
}

void setColor(int redValue, int greenValue,  int blueValue) {
  analogWrite(LED_HIGH, redValue);
  analogWrite(LED_LOW,  greenValue);
  analogWrite(LED_MEDIUM, blueValue);
}
