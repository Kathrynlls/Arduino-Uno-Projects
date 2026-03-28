const int digitPins[] = {13, 10, 9, 7};
const int SegmentPins[] = {12, 8, 5, 3, 2, 11, 6};

// Button Pins 
int TwoPointButton = A0;
int ThreePointButton = A1;
int OnePointButton = A4;

// Player Pin (Slide switch to select HomePlayer)
int HomePlayer = A2;  // HomePlayer pin for the slide switch (now using A2)
int GuestPlayer = A3;  // GuestPlayer LED
int HomeLed = A5;      // HomePlayer LED

// Initialize Player Score
int HomeScore = 0;
int GuestScore = 0;

// Initialize Display
int currentDigit = 0;

// Debouncing Variable
unsigned long lastButtonCheck = 0;
const unsigned long debounceDelay = 200;

void setup() {
  // Initialize segment pins
  for (int i = 0; i < 7; i++) {
    pinMode(SegmentPins[i], OUTPUT);
  }
  
  // Initialize digit pins
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
    digitalWrite(digitPins[i], HIGH); // Turn off digits initially
  }

  // Initialize button pins
  pinMode(OnePointButton, INPUT);
  pinMode(TwoPointButton, INPUT);
  pinMode(ThreePointButton, INPUT);
  
  // Player input pin for HomePlayer
  pinMode(HomePlayer, INPUT_PULLUP);   // HomePlayer pin should be pulled up as there no external resistor
  pinMode(GuestPlayer, OUTPUT);        // LedPIN as Output
  pinMode(HomeLed, OUTPUT);            // LedPIN as Output

  // Start Serial Monitor for debugging
  Serial.begin(9600);
}

void loop() {
  // Read A2 (slide switch) and control A3 and A5 based on its state
  int homePlayerState = digitalRead(HomePlayer); // Read the state of A2
  if (homePlayerState == HIGH) {
    digitalWrite(GuestPlayer, LOW);   // If A2 is HIGH, turn off the GuestPlayer LED 
    digitalWrite(HomeLed, HIGH);      // Turn on Home LED
  } else {
    digitalWrite(GuestPlayer, HIGH);  // If A2 is LOW, turn on the GuestPlayer LED 
    digitalWrite(HomeLed, LOW);       // Turn off Home LED
  }

  checkButton(); // Handle button presses for scoring
  Display();     // Display updated score
}

void checkButton() {
  // Button Debounce
  if (millis() - lastButtonCheck > debounceDelay) {
    if (digitalRead(OnePointButton) == HIGH) {
      updateScore(1); //1 point
      lastButtonCheck = millis();
      Serial.println("One Point Button Pressed");
    }
    if (digitalRead(TwoPointButton) == HIGH) {
      updateScore(2); //2 Point
      lastButtonCheck = millis();
      Serial.println("Two Point Button Pressed");
    }
    if (digitalRead(ThreePointButton) == HIGH) {
      updateScore(3); //3 Point
      lastButtonCheck = millis();
      Serial.println("Three Point Button Pressed");
    }
  }
}

void updateScore(int points) {
  // Debugging: Print button states
  int homePlayerState = digitalRead(HomePlayer);  // Get the state of the slide switch
  Serial.print("HomePlayer (Slide Switch): ");
  Serial.println(homePlayerState);

  // If the HomePlayer slide switch is LOW, score is updated for HomePlayer
  if (homePlayerState == LOW) {  // HomePlayer pressed (active)
    HomeScore = min(HomeScore + points, 99); // Update Home score
    Serial.print("Home Score: ");
    Serial.println(HomeScore);
  }
  // If the HomePlayer slide switch is HIGH, score is updated for GuestPlayer
  else {
    GuestScore = min(GuestScore + points, 99); // Update Guest Score
    Serial.print("Guest Score: ");
    Serial.println(GuestScore);
  }
}

void Display() {
  // Prepare the scores for display (tens and ones places for both home and guest)
  int displayValues[] = {HomeScore / 10, HomeScore % 10, GuestScore / 10, GuestScore % 10};
  
  digitalWrite(digitPins[currentDigit], HIGH); //Refresh for next digit
  currentDigit = (currentDigit + 1) % 4; // There are 4 digits, cycle from 0 to 3

  // Display the updated digit number
  DigitNumber(displayValues[currentDigit]);
  digitalWrite(digitPins[currentDigit], LOW); // Turn on the next digit

  // Increased delay for better multiplexing visibility (each digit stays on for enough time)
  delay(5); 
}

// Function to light up segments for a given number
void DigitNumber(int displayNumber) {
  int sevenSegmentStates[10][7] = {
    {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW},  // 0
    {LOW, HIGH, HIGH, LOW, LOW, LOW, LOW},      // 1
    {HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH},  // 2
    {HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH},  // 3
    {LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH},   // 4
    {HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH},  // 5
    {HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH}, // 6
    {HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW},    // 7
    {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH},// 8
    {HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH}, // 9
  };

  // Ensure we only handle digits between 0 and 9
  if (displayNumber >= 0 && displayNumber < 10) {
    // Update segments to match the current digit
    for (int i = 0; i < 7; i++) {
      digitalWrite(SegmentPins[i], sevenSegmentStates[displayNumber][i]);
    }
  } else {
    // If the digit is out of bounds, turn off all segments (shouldn't happen)
    for (int i = 0; i < 7; i++) {
      digitalWrite(SegmentPins[i], LOW);
    }
  }
}
