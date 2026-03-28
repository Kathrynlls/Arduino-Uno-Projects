#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

//Buttons Pin
const int startButtonPin = 5;
const int modeButtonPin = 6;
const int oddButtonPin = 8;
const int evenButtonPin = 7;

//Buzzer Pin
const int buzzerPin = 11;

//Lives LED Pin
const int Lives3 = 13;
const int Lives2 = 4;
const int Lives1 = 3;

//Odd/Even LED Pin
const int evenLED = A3;
const int oddLED = 2;

//Ultrasonic Pin
const int trigPin = 9;  // Ultrasonic trigger
const int echoPin = 10; // Ultrasonic echo

// Game Variables
int mode = 0; // 0 = Easy ; 1 = Medium ; 2 = Hard
int level = 1;
int score = 0;
int lives = 3;
int streak = 0;
int setsCompleted = 0;
int binarylength = 4;
bool gameActive = false;
bool gameEnded = false;
int currentBinary;
String playerStatus = "____";

// Timer Variables
unsigned long previousMillis = 0;
unsigned long interval = 1000;  // Update interval for time display
unsigned long displayInterval = 100; // Shorter interval for display updates
int timeLeft = 15;

//Buzzer Variables
// Define note frequencies for 8-bit-style music (in Hz)
const int NOTES[] = {
  262, 294, 330, 349, 392, 440, 494, 523, 587, 659, 698, 784 // C4, D4, E4, F4, G4, A4, B4, C5, D5, E5, F5, G5
};

// Define the melody using indices from NOTES[] array
const int melody[] = {
  0, 2, 4, 7,   // C4, E4, G4, C5 (Intro scale up)
  7, 4, 2, 0,   // C5, G4, E4, C4 (Intro scale down)
  4, 5, 3, 1,   // G4, A4, F4, D4 (Main melody 1)
  4, 0, 1, 2,   // G4, C4, D4, E4 (Main melody 2)
  
  2, 3, 4, 7,   // E4, F4, G4, C5 (Build-up)
  8, 9, 10, 11, // D5, E5, F5, G5 (Higher build-up)
  7, 9, 8, 4,   // C5, E5, D5, G4 (Descend to main theme)
  4, 0, 2, 1,   // G4, C4, E4, D4 (Alternate melody)
  
  1, 2, 3, 4,   // D4, E4, F4, G4 (Fast scale up)
  4, 5, 4, 3,   // G4, A4, G4, F4 (Fast scale down)
  3, 2, 1, 0    // F4, E4, D4, C4 (End of section)
};

// Define the duration of each note (1 = whole note, 2 = half note, etc.)
const int noteDurations[] = {
  8, 8, 8, 8,   // Fast notes for intro
  8, 8, 8, 8,   // Fast notes for intro
  4, 4, 4, 4,   // Slower section
  8, 8, 8, 8,   // Fast alternating section
  
  8, 8, 8, 8,   // Build-up
  4, 4, 4, 4,   // Higher build-up
  8, 8, 8, 8,   // Descending theme
  8, 8, 8, 8,   // Alternate melody
  
  8, 8, 8, 8,   // Fast scale up
  8, 8, 8, 8,   // Fast scale down
  4, 4, 4, 4    // Final cadence
};

// Define the tempo for the tune
const int tempo = 120;  // Adjust this to speed up or slow down the music

// Buzzer Variables
int buzzerNoteIndex = 0;
unsigned long buzzerPreviousMillis = 0;
int buzzerNoteDuration = 0;
bool isPlayingBuzzer = false;

// Variables for ultrasonic sensor
long duration;
int distance;
unsigned long distanceStableStartTime = 0; // Timer for stable distance
bool isDistanceConsistent = false;         // Flag for distance consistency
bool startupScreenShown = false;          // Flag to prevent duplicate startup screen display

void setup() {
    // Initialize Serial Monitor for distance readings
    Serial.begin(9600);

    //LCD Setup
    lcd.init();
    lcd.backlight();

    //Input Setup
    pinMode(modeButtonPin, INPUT);
    pinMode(oddButtonPin, INPUT);
    pinMode(evenButtonPin, INPUT);

    //Output Setup
    pinMode(buzzerPin, OUTPUT);
    pinMode(Lives3, OUTPUT);
    pinMode(Lives2, OUTPUT);
    pinMode(Lives1, OUTPUT);
    pinMode(evenLED, OUTPUT);
    pinMode(oddLED, OUTPUT);

    // Setup ultrasonic sensor pins
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    //Start up animation
    lcd.clear();
    lcd.setCursor(6, 1);
    lcd.print(" Parity ");
    lcd.setCursor(6, 2);
    lcd.print(" Panic! ");

    delay(2000); // 2 second hold

    binaryErase(" Parity ", " Panic! ", 7); // Erase Parity Panic text in animation form

    for (int i = 0; i < 3; i++) {
        lcd.noDisplay();
        delay(250);
        lcd.display();
        delay(250);
    }
    lcd.clear();

    //Show Startup display
    displayStartupScreen();
}

void loop() {
  distance = measureDistance(); //get cm distance in ultrasonic
  unsigned long currentMillis = millis(); 

  // Print the distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  // Check if the distance is consistently less than or equal to 100 cm
  if (distance <= 100) {
        // If the distance is less than or equal to 100 cm, reset the timer and allow the game
        distanceStableStartTime = currentMillis; // Reset the timer
        isDistanceConsistent = true;             // Set the flag to allow the game

        // **Update the screen based on the game state**
        if (gameActive) {
            displayGameScreen(); // Function to display the active game interface
            startupScreenShown = false; // Reset the startup screen flag
        } else if (!startupScreenShown) {
            displayStartupScreen(); // Show the startup screen only once
            startupScreenShown = true; // Set the flag to prevent duplicate display
        }
    } else {
        // If the distance is greater than 100 cm, check if it has been consistently greater for 2.5 seconds
        if (currentMillis - distanceStableStartTime >= 2500) {
            isDistanceConsistent = false; // Set the flag to stop the game
            startupScreenShown = false;   // Reset the flag so the startup screen can be shown again
        }
    }
  
  if (isDistanceConsistent) { //Check if player is detected
    
    // Check if the mode button is pressed and the game is not currently active
    if (digitalRead(modeButtonPin) == HIGH && !gameActive) {
        tone(buzzerPin, 500); //short tone for audio feedback
        delay(100);
        noTone(buzzerPin); //stop tone after 100 miliseconds
        mode = (mode + 1) % 3; // Cycle through the game modes (0, 1, 2)
        displayStartupScreen(); // Display the startup screen for the new mode
        delay(200); // Debounce delay to prevent accidental multiple presses
    }

    // Check if the start button is pressed and the game is not currently active
    if (digitalRead(startButtonPin) == HIGH && !gameActive) {
        tone(buzzerPin, 500); //short tone for audio feedback
        delay(100);
        noTone(buzzerPin); //stop tone after 100 miliseconds
        lcd.clear(); 
        startGame(); // Call startGame to initialize the game
        gameActive = true;  //Update Flag that the game started
        return; // Exit the function to avoid further updates
    }

    // Check if the even button is pressed and the game is currently active
    if (digitalRead(evenButtonPin) == HIGH && gameActive) {
        digitalWrite(evenLED, HIGH); // Turn on the "Even" LED
        tone(buzzerPin, 500); // Play a short tone for feedback
        delay(100);
        noTone(buzzerPin); //stop tone after 100 miliseconds
        playerStatus = "Even"; // Set player status to "Even" shown on the lcd
        checkAnswer(true); // True = Even
        delay(100);
        digitalWrite(evenLED, LOW); // Turn off the "Even" LED after a delay
    }
    
    // Check if the odd button is pressed and the game is currently active
    if (digitalRead(oddButtonPin) == HIGH && gameActive) {
        digitalWrite(oddLED, HIGH); // Turn on the "Odd" LED
        tone(buzzerPin, 1000); //play short tone for feedback
        delay(100);
        noTone(buzzerPin); //stop tone after 100 miliseconds
        playerStatus = "Odd "; // Set player status to "Odd" shown on the lcd
        checkAnswer(false); // False = Odd
        delay(100);
        digitalWrite(oddLED, LOW); // Turn off the "Odd" LED
    }

    unsigned long currentMillis = millis(); // Get the current time in milliseconds

    if (gameActive) { // Check if the game is active
        if (currentMillis - previousMillis >= interval) { // Check if the interval has passed
            previousMillis = currentMillis; // Update the previous time
            if (timeLeft > 0) { // If there is still time left
                timeLeft--; // Decrease the time left
            } else {
                handleTimeUp(); // Handle the situation when time is up
            }
        }

        // Update the game display frequently
        if (currentMillis - previousMillis >= displayInterval) {
            displayGameScreen(); // Refresh the game screen on the LCD
        }
        play8BitMusic(); // Play background music during the game
    } else if (gameEnded) {
        noTone(buzzerPin); // Stop any tone if the game has ended
    }
  } else {
    lcd.clear(); // Clear the LCD display if the player is not detected
    noTone(buzzerPin); // Ensure the buzzer is turned off
  } 
}

void binaryErase(String text1, String text2, int binaryLength) {
    int textLength = text1.length();

    for (int i = textLength - 1; i >= 0; i--) {
        for (int j = 0; j < binaryLength; j++) {
            lcd.setCursor(6 + i, 1); // For parity printing 0 and 1 animation
            lcd.print(random(0, 2)); // Print random 0 or 1
            lcd.setCursor(6 + i, 2); // For Panic
            lcd.print(random(0, 2)); // Print random 0 or 1

            delay(50); // Delay of print
        }
    }
    delay(500); // Pause before lcd clear
}

//Startup LCD Display
void displayStartupScreen() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("----Parity Panic----");
    lcd.setCursor(4, 1);
    lcd.print("Press Start");
    lcd.setCursor(6, 2);
    lcd.print("To Play");

    // Changing of Mode 
    lcd.setCursor(2, 3);
    if (mode == 0) {
        lcd.print("-> Mode:Easy <-");
    } else if (mode == 1) {
        lcd.print("-> Mode:Medium <-");
    } else if (mode == 2) {
        lcd.print("-> Mode:Hard <-");
    }
}

//funtion for Randomly generating binary 
void generateBinary() {
    currentBinary = random(0, (1 << binarylength));
}

//Show Binary with leading zero in LCD
void BinaryWithLeadingZero(int number, int length) {
    lcd.setCursor(8, 2);
    for (int i = length - 1; i >= 0; i--) {
        int bit = (number >> i) & 1;
        lcd.print(bit);
    }
}

//Times Up Function (when player reach 00 countdown when answering)
void handleTimeUp() {
    digitalWrite(Lives1, LOW);
    digitalWrite(Lives2, LOW);
    digitalWrite(Lives3, LOW);

    //Print Time's up game over
    lcd.clear();
    lcd.setCursor(5, 1);
    lcd.print("Time's up!");
    lcd.setCursor(5, 2);
    lcd.print("Score: ");
    lcd.print(score);
    timeUpTone();
    noTone(buzzerPin);
    delay(3000);  // Display the message for 3 seconds

    gameEnded = true; //Flag that the game ended
    resetGame();  // Reset game or handle next steps
}

// Function to set the countdown time based on the mode
void setTimeForMode() {
    if (mode == 0) {
        timeLeft = 15;  // Easy mode: 15 seconds
    } else if (mode == 1) {
        timeLeft = 13;   // Medium mode: 13 seconds
    } else if (mode == 2) {
        timeLeft = 10;   // Hard mode: 10 seconds
    }
}

//Initialize Game Variables to original state
void startGame() {
    gameActive = true; //Set game to active
    level = 1; //initial level 1
    score = 0; // reset score to 0
    lives = 3; // set lives back to 3
    streak = 0; //reset streak
    setsCompleted = 0; //Reset sets completed
    binarylength = 4; //start with 4-bit binary number

    //Turn on all LEDs representing the lives
    digitalWrite(Lives3, HIGH);
    digitalWrite(Lives2, HIGH);
    digitalWrite(Lives1, HIGH);

    setTimeForMode(); // Set the timer based on the selected mode
    generateBinary(); // Generate the initial binary number
    displayGameScreen(); // Display the game screen on the LCD
}


void displayGameScreen() {
    // Display the current level on the first line
    lcd.setCursor(0, 0);
    lcd.print("Lvl: ");
    lcd.print(level);

    // Display the current mode (Easy, Medium, Hard) on the first line
    lcd.setCursor(10, 0);
    lcd.print("Mode: ");
    if (mode == 0) lcd.print("Easy"); // Easy mode
    else if (mode == 1) lcd.print("Med"); // Medium mode
    else lcd.print("Hard"); // Hard mode

    // Display the current score on the second line
    lcd.setCursor(0, 1);
    lcd.print("Scr: ");
    lcd.print(score);

    // Display the remaining time in "MM" format on the second line
    char TimeLCD[3]; // Array to hold the formatted time string
    sprintf(TimeLCD, "%02d", timeLeft); // Format the time with leading zero if necessary

    lcd.setCursor(10, 1);
    lcd.print("Time: ");
    lcd.print(TimeLCD); // Print the formatted time

    // Display the binary number on the third line
    lcd.setCursor(0, 2);
    lcd.print("Binary: ");
    BinaryWithLeadingZero(currentBinary, binarylength); // Print the binary number with leading zeros

    // Display the player's current status (e.g., "Even" or "Odd") on the fourth line
    lcd.setCursor(0, 3);
    lcd.print("Status: ");
    lcd.print(playerStatus);
}

void resetGame() {
    noTone(buzzerPin); // Stop any sound from the buzzer
    gameActive = false; // Set the game state to inactive
    level = 1; // Reset the level to 1
    score = 0; // Reset the score to 0
    binarylength = 4; // Reset the binary number length to 4 bits
    playerStatus = "____"; // Reset the player status display

    setTimeForMode(); // Reconfigure the timer for the current mode
    displayStartupScreen(); // Show the startup screen on the LCD
}

void checkAnswer(bool playerAnswer) {
    // Compare player answer with actual binary
    if ((currentBinary % 2 == 0 && playerAnswer) || (currentBinary % 2 != 0 && !playerAnswer)) {
        streak++;

        // Base score depending on streak
        int baseScore = 0;
        if (streak == 1) {
            baseScore = 5;  // Base score for the first streak
        } else if (streak == 2) {
            baseScore = 10; // Base score for the second streak
            lcd.setCursor(15, 3);
            lcd.print("NICE");
        } else {
            baseScore = 15; // Base score for the third or more streaks
            lcd.setCursor(14, 3);
            lcd.print("Combo!");
        }

        // Modify score based on selected mode
        if (mode == 0) {
            // Easy mode, base score stays the same
            score += baseScore;
        } else if (mode == 1) {
            // Medium mode, 20% higher score
            score += baseScore * 1.2;
        } else if (mode == 2) {
            // Hard mode, 40% higher score
            score += baseScore * 1.4;
        }

    } else {
        lives--;
        streak = 0;
        lcd.setCursor(14, 3);
        lcd.print("      ");
        
        // Update LEDs based on lives count
        if (lives == 2) {
            digitalWrite(Lives3, LOW);  // Turn off Lives3 LED
            digitalWrite(Lives2, HIGH); // Keep Lives2 LED on
            digitalWrite(Lives1, HIGH); // Keep Lives1 LED on
        } else if (lives == 1) {
            digitalWrite(Lives3, LOW);  // Lives3 LED remains off
            digitalWrite(Lives2, LOW);  // Turn off Lives2 LED
            digitalWrite(Lives1, HIGH); // Keep Lives1 LED on
        } else if (lives == 0) {
            digitalWrite(Lives3, LOW);  // All LEDs off
            digitalWrite(Lives2, LOW);
            digitalWrite(Lives1, LOW);
            lcd.clear();

            //Game Over display
            lcd.setCursor(5, 1);
            lcd.print("GAME OVER!");
            lcd.setCursor(5, 2);
            lcd.print("Score: ");
            lcd.print(score);
            gameOverTone();
            noTone(buzzerPin);
            delay(3000);

            gameEnded = true;
            resetGame();
            return;
        }
    }

    setsCompleted++;
    if (setsCompleted >= 10) {
        level++; // Move to next level
        setsCompleted = 0; // Reset completed sets
        if (level > 3) {
            // Player has completed all levels
            digitalWrite(Lives1, LOW);
            digitalWrite(Lives2, LOW);
            digitalWrite(Lives3, LOW);

            lcd.clear();

            //Congratulation Display
            lcd.setCursor(2, 1);
            lcd.print("Congratulations!");
            lcd.setCursor(5, 2);
            lcd.print("Score: ");
            lcd.print(score);
            victoryTone();
            delay(3000);
            lcd.clear();
            gameEnded = true;
            noTone(buzzerPin);
            resetGame();
            
            return;
        } else {
            // Continue to next binary number
            binarylength += 2;  // Increase binary length for next level
            generateBinary();
            setTimeForMode();
            displayGameScreen();
        }
    } else {
        // Continue to the next binary number
        generateBinary();
        setTimeForMode();
        displayGameScreen();
    }
    //if lives is still not 0 continue game play
    if (lives > 0) {
        generateBinary();
        setTimeForMode();
        displayGameScreen();
    }
}


// Function to play the extended 8-bit style tune

void play8BitMusic() {
    unsigned long currentMillis = millis();
    
    if (isPlayingBuzzer && currentMillis - buzzerPreviousMillis >= buzzerNoteDuration) {
        // Stop the note after the duration
        noTone(buzzerPin);
        buzzerPreviousMillis = currentMillis;
        buzzerNoteIndex++;
        
        if (buzzerNoteIndex >= sizeof(melody) / sizeof(melody[0])) {
            buzzerNoteIndex = 0;  // Reset the melody
            isPlayingBuzzer = false;  // Stop music after playing the melody once
        } else {
            // Play the next note
            buzzerNoteDuration = 1000 / noteDurations[buzzerNoteIndex];  // Calculate note duration
            tone(buzzerPin, NOTES[melody[buzzerNoteIndex]]);
            buzzerPreviousMillis = currentMillis;  // Reset timer for the next note
        }
    } else if (!isPlayingBuzzer) {
        // If buzzer is not currently playing, start it
        buzzerNoteIndex = 0;
        buzzerNoteDuration = 1000 / noteDurations[buzzerNoteIndex];
        tone(buzzerPin, NOTES[melody[buzzerNoteIndex]]);
        buzzerPreviousMillis = currentMillis;
        isPlayingBuzzer = true;
    }
}

//Game Over Buzzer Tone
void gameOverTone() {
  int melody[] = {1000, 800, 600};  // Array of frequencies
  int duration = 300;  // Duration for each tone

  for (int i = 0; i < 3; i++) {
    tone(buzzerPin, melody[i]);
    delay(duration);
    noTone(buzzerPin);
    delay(100);  // Short pause between tones
  }
}

//Times Up Buzzer Tone
void timeUpTone() {
  for (int i = 0; i < 5; i++) {
    tone(buzzerPin, 1000);  // First frequency
    delay(250);  // Wait for 250 ms
    noTone(buzzerPin);
    delay(50);  // Short pause
    tone(buzzerPin, 800);  // Second frequency
    delay(250);  // Wait for 250 ms
    noTone(buzzerPin);
    delay(50);  // Short pause
  }
}

//Victor Buzzer Tone
void victoryTone() {
  int melody[] = { 1000, 1200, 1400, 1600 };  // Higher frequencies for celebration
  int duration = 300;  // Duration for each note

  for (int i = 0; i < 4; i++) {
    tone(buzzerPin, melody[i]);  // Play each note
    delay(duration);  // Duration for the note
    noTone(buzzerPin);  // Stop the tone
    delay(50);  // Short pause
  }
}

//Ultrasonic Measuring of distance function
int measureDistance() {
  // Send a pulse from the ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(4);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(trigPin, LOW);

  // Measure the pulse duration
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance in cm
  int dist = duration * 0.034 / 2;

  return dist;
}






