#include <SoftwareSerial.h>

// Define pins
#define LDR_PIN A0
#define LED1 2
#define LED2 3
#define LED3 4
#define LED4 5
#define LED5 6

// ESP8266 TX to Arduino pin 10, RX to pin 11
SoftwareSerial esp(10, 11); 

// ThingSpeak API key
String apiKey = "GVGISNVR0G07166P";

// Your Wi-Fi credentials
const char* ssid = "PLDTHOMEFIBReba48";
const char* password = "PLDTWIFIbt7fr";

void setup() {
  Serial.begin(9600);
  esp.begin(115200);

  pinMode(LDR_PIN, INPUT);
  for (int i = 2; i <= 6; i++) pinMode(i, OUTPUT);

  connectWiFi();
}

void loop() {
  int lightVal = analogRead(LDR_PIN);
  Serial.print("Light Level: ");
  Serial.println(lightVal);

  int ledStatus = 0; // 0 - OFF, 1 - ON

  // LED Control Logic
  if (lightVal < 500) { // If it's dark, turn on LEDs
    for (int i = 2; i <= 6; i++) {
      digitalWrite(i, HIGH);
    }
    ledStatus = 1; // LEDs are ON
  } else { // If it's bright, turn off LEDs
    for (int i = 2; i <= 6; i++) {
      digitalWrite(i, LOW);
    }
    ledStatus = 0; // LEDs are OFF
  }

  // Upload both LDR value and LED status to ThingSpeak
  uploadToThingSpeak(lightVal, ledStatus);
  delay(20000); // ThingSpeak requires 15s delay between uploads
}

void connectWiFi() {
  sendCommand("AT", 2000);
  sendCommand("AT+CWMODE=1", 2000);
  sendCommand("AT+CWJAP=\"" + String(ssid) + "\",\"" + String(password) + "\"", 6000);
}

void uploadToThingSpeak(int lightVal, int ledStatus) {
  String cmd = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80";
  sendCommand(cmd, 2000);

  String getStr = "GET /update?api_key=" + apiKey + "&field1=" + String(lightVal) + "&field2=" + String(ledStatus) + "\r\n";
  sendCommand("AT+CIPSEND=" + String(getStr.length() + 2), 2000);
  esp.print(getStr);
}

void sendCommand(String command, int timeout) {
  esp.println(command);
  long int time = millis();
  while ((time + timeout) > millis()) {
    while (esp.available()) {
      Serial.write(esp.read());
    }
  }
}
