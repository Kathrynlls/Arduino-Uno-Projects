#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define AnalogOut A0
#define RLed 7
#define GLed 6
#define BLed 5
#define Buzzer 4
#define ButtonPin 3

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust I2C address if needed
SoftwareSerial esp(11, 10); //ESP8266 TX/RX FOR WIFI

//ThingSpeak API Key
String apiKey = "073FPFALONMBVFLZ";

//Wi-FI credentials
const char* ssid = "OPPO Reno12 F";
const char* password = "r3zjz4u3";

void setup() {
  esp.begin(115200);
  pinMode(AnalogOut, INPUT);
  pinMode(RLed, OUTPUT);
  pinMode(GLed, OUTPUT);
  pinMode(BLed, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(ButtonPin, INPUT); // Use internal pull-up

  setColor(0, 0, 0);         // LEDs off
  digitalWrite(Buzzer, LOW); // Buzzer off

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hold Btn 5s to");
  lcd.setCursor(0, 1);
  lcd.print("Start Detection");

  Serial.begin(9600);
  connectWiFi();
}

void loop() {
  int analogVal = analogRead(AnalogOut);
  Serial.print("Analog value : ");
  Serial.println(analogVal);

  // Check if button is held
  if (digitalRead(ButtonPin) == HIGH) {
    int countdown = 5;
    unsigned long startHold = millis();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hold for 5 sec");

    while (digitalRead(ButtonPin) == HIGH) {
      if (millis() - startHold >= 1000) {
        startHold += 1000;
        lcd.setCursor(0, 1);
        lcd.print("Loading: ");
        lcd.print(countdown);
        countdown--;

        if (countdown < 0) break;
      }
    }

    // If full 5 seconds held
    if (countdown < 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sampling...");

      long sum = 0;
      int samples = 0;
      unsigned long sampleStart = millis();
      unsigned long lastKeepAlive = millis();

      // Sample for 5 seconds
      while (millis() - sampleStart < 5000) {
        int val = analogRead(AnalogOut);
        sum += val;
        samples++;
        delay(50); // sampling every 50ms
      }

      int avg = sum / samples;
      float bac = estimateBAC(avg);  // Estimate BAC from average

      // Display analog average and BAC
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("BAC: ");
      lcd.print(bac, 3);
      lcd.print("%");

      // Classification and action
      if (avg <= 419) {
        setColor(0, 255, 0);            // Green
        digitalWrite(Buzzer, LOW);
        lcd.setCursor(0, 1);
        lcd.print("Status: Safe");
      } else if (avg <= 480) {
        setColor(0, 0, 255);            // Blue
        lcd.setCursor(0, 1);
        lcd.print("Status: Legal");

        for (int i = 0; i < 10; i++) {  // Dotted tone
          digitalWrite(Buzzer, HIGH);
          delay(100);
          digitalWrite(Buzzer, LOW);
          delay(200);
        }
      } else {
        setColor(255, 0, 0);            // Red
        digitalWrite(Buzzer, HIGH);
        lcd.setCursor(0, 1);
        lcd.print("Status: DRUNK!");
        delay(2000); // Buzzer on for 2s
        digitalWrite(Buzzer, LOW);
      }

      uploadToThingSpeak(avg);
      delay(15000); // Wait before resetting
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hold Btn 5s to");
      lcd.setCursor(0, 1);
      lcd.print("Start Detection");
      setColor(0, 0, 0);
      digitalWrite(Buzzer, LOW);
    } else {
      // Not held long enough
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hold Canceled");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hold Btn 5s to");
      lcd.setCursor(0, 1);
      lcd.print("Start Detection");
    }
  }

  delay(100);
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(RLed, redValue);
  analogWrite(GLed, greenValue);
  analogWrite(BLed, blueValue);
}

float estimateBAC(int analogValue) {
  // Custom linear scale: 280 → 0.001%, 480 → ~0.05%
  if (analogValue < 280) return 0.0;
  float slope = 0.00025;  // (0.05 - 0.001) / (480 - 280)
  return (analogValue - 280) * slope + 0.001;
}

void connectWiFi(){
  sendCommand("AT", 2000);
  sendCommand("AT+CWMODE=1", 2000);
  sendCommand("AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"", 8000);
}

void uploadToThingSpeak(int avg){
  float bac = estimateBAC(avg); // Compute BAC from average

  String cmd = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80";
  sendCommand(cmd, 3000);

  String getStr = "GET /update?api_key=" + apiKey + "&field1=" + String(avg) + "&field2=" + String(bac, 3) + " HTTP/1.1\r\nHost: api.thingspeak.com\r\nConnection: close\r\n\r\n";
  sendCommand("AT+CIPSEND=" + String(getStr.length()), 2000);
  esp.print(getStr);
  delay(2000); // Wait for transmission
  esp.println("AT+CIPCLOSE");
}

void sendCommand(String command, int timeout){
  esp.println(command);
  long int time = millis();
  while ((time + timeout) > millis()){
    while (esp.available()){
      Serial.write(esp.read());
    }
  }
}