// COMMON CATHODE 7 SEGMENT

const int digitPins[] = {7, 9, 10, 13};
const int sevensegmentPins[] = {12, 8, 5, 3, 2, 11, 6};
const byte Increment = A0;
const byte Decrement = A1;

int hours = 9; //initialize hours
int minutes = 50; // initialize minutes
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 60000; // 1 minute

// Button initialization
unsigned long buttonPressTime[2] = {0, 0};
bool buttonHeld[2] = {false, false};
const unsigned long holdDuration = 1000; // When button is held for 1 second, hours will increment/decrement

void setup() {
  for (int i = 0; i < 7; i++) {
    pinMode(sevensegmentPins[i], OUTPUT);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(digitPins[i], OUTPUT);
  }
  pinMode(Increment, INPUT_PULLUP);
  pinMode(Decrement, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastUpdate >= updateInterval) {
    lastUpdate = currentMillis;
    IncrementMinute();
  }
  Display(hours * 100 + minutes);
  checkButtons();
}

// TIME INTEGRATION
void IncrementMinute() {
  minutes++;
  if (minutes > 59) {
    minutes = 0;
    hours++;
    if (hours > 12) {
      hours = 1;
    }
  }
}

void DecrementMinute() {
  if (minutes == 0) {
    minutes = 59;
    if (hours == 1) {
      hours = 12;
    } else {
      hours--;
    }
  } else {
    minutes--;
  }
}

void IncrementHour() {
  hours = (hours % 12) + 1;
}

void DecrementHour() {
  hours = (hours == 1) ? 12 : hours - 1;
}

// CHECK BUTTONS
void checkButtons() {
  int buttons[] = {Increment, Decrement};

  for (int i = 0; i < 2; i++) {
    if (!digitalRead(buttons[i])) {
      if (buttonPressTime[i] == 0) {
        buttonPressTime[i] = millis(); // to start count of hold duration
      }
      if ((millis() - buttonPressTime[i]) > holdDuration) {
        buttonHeld[i] = true; // Button is held
      }
    } else {
      if (buttonHeld[i]) {
        (i == 0) ? IncrementHour() : DecrementHour();
        buttonHeld[i] = false;
      } else if (buttonPressTime[i] != 0) {
        (i == 0) ? IncrementMinute() : DecrementMinute();
      }
      buttonPressTime[i] = 0;
    }
  }
}

// FOR SEVEN SEGMENT
void Display(int toDisplay) {
  for (int digit = 0; digit < 4; digit++) {
    digitalWrite(digitPins[digit], LOW); // Turn on the current digit
    DigitNumber(toDisplay % 10); // Display the digit
    toDisplay /= 10;
    delayMicroseconds(2000);  

    DigitNumber(-1); // Turn off the segments
    digitalWrite(digitPins[digit], HIGH); 
  }
}

void DigitNumber(int displayNumber) {
  int sevensegmentStates[10][7] = {
    {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW},  // 0
    {LOW, HIGH, HIGH, LOW, LOW, LOW, LOW},  // 1
    {HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH},  // 2
    {HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH},  // 3
    {LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH},  // 4
    {HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH},  // 5
    {HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH},  // 6
    {HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW},  // 7
    {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH},  // 8
    {HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH},  // 9
  };

  if (displayNumber >= 0 && displayNumber < 10) {
    for (int i = 0; i < 7; i++) {
      digitalWrite(sevensegmentPins[i], sevensegmentStates[displayNumber][i]);
    }
  } else {
    for (int i = 0; i < 7; i++) {
      digitalWrite(sevensegmentPins[i], LOW); // turn off segments
    }
  }
}
