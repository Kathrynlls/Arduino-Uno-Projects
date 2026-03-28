/* Key	 Code
 CH-	 BA45FF00
 CH	   B946FF00
 CH+	 B847FF00
 <<	   BB44FF00
 >>	   BF40FF00
 >||	 BC43FF00
 –	   F807FF00
 +	   EA15FF00
 EQ	   F609FF00
 100+	 E619FF00
 200+	 F20DFF00
 0	   E916FF00
 1	   F30CFF00
 2	   E718FF00
 3	   A15EFF00
 4	   F708FF00
 5	   E31CFF00
 6	   A55AFF00
 7	   BD42FF00
 8	   AD52FF00
 9	   B54AFF00 */

#include <IRremote.hpp>
#include <Stepper.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

const int LED = 3;
const int BUZZER = 4;
const int stepsPerRevolution = 2048;

int ServoPos = 0;
bool ServoSweepDirection = true;  //track direction
Servo RemotedServo;
Stepper RemotedStepper = Stepper(stepsPerRevolution, 8, 9, 10, 11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Bool Statement
bool LEDHIGH = false;
bool BUZZERHIGH = false;
bool StepperON = false;
bool ServoON = false;
bool LCDON = false;

//Button Code
#define Button0 0xE916FF00  // Code for Button 0
#define Button1 0xF30CFF00  // Code for Button 1
#define Button2 0xE718FF00  // Code for Button 2
#define Button3 0xA15EFF00  // Code for Button 3
#define Button4 0xF708FF00  // Code for Button 4

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(2, ENABLE_LED_FEEDBACK);
  Serial.println("Ir Receiver Ready");

  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  digitalWrite(LED, LOW);       //Set Led Initially to off
  digitalWrite(BUZZER, LOW);    //Set Led Initially to off
  RemotedStepper.setSpeed(10);  //Set the RPM of the Stepper
  RemotedServo.attach(5);
  lcd.init();
  lcd.backlight();
}

void updateLCD() {
  lcd.setCursor(0, 0);
  lcd.print("LED: ");
  lcd.print(LEDHIGH ? "ON " : "OFF");
  lcd.print(" Buz: ");
  lcd.print(BUZZERHIGH ? "ON" : "OFF");

  lcd.setCursor(0, 1);
  lcd.print("Stp: ");
  lcd.print(StepperON ? "ON " : "OFF");
  lcd.print(" Srv: ");
  lcd.print(ServoON ? "ON" : "OFF");
}

void loop() {

  if (IrReceiver.decode()) {
    uint32_t decodedValue = IrReceiver.decodedIRData.decodedRawData;
    Serial.print("Decoded Data: ");
    Serial.println(decodedValue, HEX);

    if (decodedValue == Button1 && !LEDHIGH) {
      digitalWrite(LED, HIGH);
      LEDHIGH = true;
    } else if (decodedValue == Button1 && LEDHIGH) {
      digitalWrite(LED, LOW);
      LEDHIGH = false;
    }

    if (decodedValue == Button2 && !BUZZERHIGH) {
      digitalWrite(BUZZER, HIGH);
      BUZZERHIGH = true;
    } else if (decodedValue == Button2 && BUZZERHIGH) {
      digitalWrite(BUZZER, LOW);
      BUZZERHIGH = false;
    }

    if (decodedValue == Button3) {
      StepperON = !StepperON;
    }

    if (decodedValue == Button4) {
      ServoON = !ServoON;
    }

    if (decodedValue == Button0) {
      LCDON = !LCDON;
      if (LCDON) {
        lcd.backlight();
      } else {
        lcd.clear();
        lcd.noBacklight();
      }
    }

    IrReceiver.resume();
  }

  if (LCDON) {
    updateLCD();
  }

  if (StepperON) {
    RemotedStepper.step(stepsPerRevolution / 100);
  }

  if (ServoON) {
    if (ServoSweepDirection) {
      ServoPos += 1;
      if (ServoPos >= 180) {
        ServoSweepDirection = false;
      }
    } else {
      ServoPos -= 1;
      if (ServoPos <= 0) {
        ServoSweepDirection = true;
      }
    }
    RemotedServo.write(ServoPos);
    delay(15);
  }
}