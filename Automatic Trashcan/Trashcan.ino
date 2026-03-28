#include <Servo.h>

// Naming 2 Servo Motor
Servo Lid;
Servo Compressor;

// Ultrasonic Pin Initialization
const int trigPin = 9;
const int echoPin = 10;

// Variables for ultrasonic
long duration;
int distance;

// Timing variables
unsigned long lastValidTime = 0; // for update interval
unsigned long lastInvalidTime = 0;
bool CompressorOn = false; // to track compressor servo position
bool LidOn = false; // to track lid servo position

void setup() {
  // Servo pins
  Lid.attach(6);
  Compressor.attach(5);

  Lid.write(150); // initial degree Lid Servo (closed position)
  Compressor.write(60); // initial degree Compressor Servo (idle)

  // Ultrasonic PinMode
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600); // serial monitor for distance readings
}

void loop() {
  // Update distance every 1.5 seconds
  if (millis() - lastInvalidTime >= 1500) {
    distance = getDistance();
    lastInvalidTime = millis();
  }

  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  /* If distance is consistently between 0-7 cm for 3 seconds, 
  Lid will close and Compressor will activate. */
  if (distance > 0 && distance <= 7) {
    if (millis() - lastValidTime >= 3000) {
      Lid.write(60); // Close the Lid
      delay(1000); // Allow time for the lid to settle
      LidOn = true; // Mark Lid as closed
      CompressorOn = true; // Allow compressor to activate
    }
  } else {
    lastValidTime = millis(); // Reset Timer if distance condition is not met
  }

  // Compressor Servo will only rotate after the lid closes
  if (CompressorOn && LidOn) {
    // If distance is still valid, activate the Compressor
    if (distance > 0 && distance <= 7) {
      Compressor.write(180); // Rotate to 180°
      delay(500); // Hold for 0.5 seconds
      Compressor.write(60); // Return to 60°
      delay(1000); // Wait for the compressor to settle

      // Check distance again after compressor operation
      distance = getDistance();
      if (distance > 0 && distance <= 7) {
        // If distance is still <= 7, Lid stays closed
        Lid.write(60);
      } else {
        // If distance is greater than 7, open the Lid back to 150°
        Lid.write(150);
      }

      // Reset flags for next cycle
      CompressorOn = false;
      LidOn = false;
    }
    delay(100); // Short delay to avoid overwhelming the loop
  }
}

// Function to measure distance using ultrasonic sensor
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  int dist = duration * 0.034 / 2;

  return dist;
}
