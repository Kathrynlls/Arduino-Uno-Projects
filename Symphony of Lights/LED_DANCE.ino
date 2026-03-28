int led0 = 2; 
int led1 = 3; 
int led2 = 4; 
int led3 = 5; 
int led4 = 6; 
int led5 = 7; 
int led6 = 8;
int led7 = 9; 
int led8 = 10; 
int led9 = 11;
int buttonBuzz = 13;
int buttonThe1975 = 12;
int buttonUptown = 1;
int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

void setup(){
  pinMode(led0, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
  pinMode(led9, OUTPUT);
  pinMode(buttonBuzz, INPUT);
  pinMode(buttonThe1975, INPUT);
  pinMode(buttonUptown, INPUT);
}

void loop(){
  buttonState1 = digitalRead(buttonBuzz);
  buttonState2 = digitalRead(buttonThe1975);
  buttonState3 = digitalRead(buttonUptown);
  
  if (buttonState1 == HIGH) {
    Buzz();
  }
  else{
  }
  
  if (buttonState2 == HIGH) {
    The1975();
  }
  else{
  }
  
  if (buttonState3 == HIGH) {
    Uptown();
  }
  else{
  }
  
  
}

void Buzz(){
  int Beatings = 600;
  
  //Center to outward Pattern
  for (int i = 0; i < 1 ; i++){
    digitalWrite(6, HIGH); digitalWrite(7, HIGH);
    delay(Beatings);
    digitalWrite(6, LOW); digitalWrite(7, LOW);
    digitalWrite(5, HIGH); digitalWrite(8, HIGH);
    delay(Beatings);
    digitalWrite(5, LOW); digitalWrite(8, LOW);
    digitalWrite(4, HIGH); digitalWrite(9, HIGH);
    delay(Beatings);
    digitalWrite(4, LOW); digitalWrite(9, LOW);
    digitalWrite(3, HIGH); digitalWrite(10, HIGH);
    delay(Beatings);
    digitalWrite(3, LOW); digitalWrite(10, LOW);
    digitalWrite(2, HIGH); digitalWrite(11, HIGH);
    delay(Beatings);
    digitalWrite(2, LOW); digitalWrite(11, LOW);
  } 
  
  //All lights on beat pattern
  for (int i = 0; i < 4; i++){
    allOn();
    delay(Beatings/2);
    allOff();
    delay(Beatings/2);
  } 
  
  //Alternating Odd Even Leds
  for (int i = 0; i < 2; i++){
    digitalWrite(2,HIGH); digitalWrite(4,HIGH); digitalWrite(6,HIGH); 
    digitalWrite(8,HIGH); digitalWrite(10,HIGH); 
    delay(Beatings);
    digitalWrite(2,LOW); digitalWrite(4,LOW); digitalWrite(6,LOW); 
    digitalWrite(8,LOW); digitalWrite(10,LOW); 
    digitalWrite(3,HIGH); digitalWrite(5,HIGH); digitalWrite(7,HIGH); 
    digitalWrite(9,HIGH); digitalWrite(11,HIGH); 
    delay(Beatings);
    digitalWrite(3,LOW); digitalWrite(5,LOW); digitalWrite(7,LOW); 
    digitalWrite(9,LOW); digitalWrite(11,LOW); 
  }
  
  //Chasing effect from right to left
  for (int i = 0; i < 10; i++) {
    digitalWrite(i + 2, HIGH);
    delay(Beatings / 6); 
    digitalWrite(i + 2, LOW);
  }
  
  //Chasing effect from left to right
  for (int i = 9; i >= 0; i--) {
    digitalWrite(i + 2, HIGH);
    delay(Beatings/ 6); 
    digitalWrite(i + 2, LOW);
  }
  
  
  //Pulsing Pairs
  for (int i = 0; i < 2.5; i++) {
    digitalWrite(led0, HIGH); digitalWrite(led9, HIGH); delay(Beatings / 3);
    digitalWrite(led0, LOW);  digitalWrite(led9, LOW);
    digitalWrite(led1, HIGH); digitalWrite(led8, HIGH); delay(Beatings / 3);
    digitalWrite(led1, LOW);  digitalWrite(led8, LOW);
    digitalWrite(led2, HIGH); digitalWrite(led7, HIGH); delay(Beatings / 3);
    digitalWrite(led2, LOW);  digitalWrite(led7, LOW);
    digitalWrite(led3, HIGH); digitalWrite(led6, HIGH); delay(Beatings / 3);
    digitalWrite(led3, LOW);  digitalWrite(led6, LOW);
    digitalWrite(led4, HIGH); digitalWrite(led5, HIGH); delay(Beatings / 3);
    digitalWrite(led4, LOW);  digitalWrite(led5, LOW);
  }
  
  //Chasing effect from right to left
  for (int i = 0; i < 10; i++) {
    digitalWrite(i + 2, HIGH);
    delay(Beatings / 6); 
    digitalWrite(i + 2, LOW);
  }
  
  //Chasing effect from left to right
  for (int i = 9; i >= 0; i--) {
    digitalWrite(i + 2, HIGH);
    delay(Beatings/ 6); 
    digitalWrite(i + 2, LOW);
  }
  
  //All lights on beat pattern
  for (int i = 0; i < 3; i++) {
    allOn(); 
    delay(Beatings);
    allOff();
    delay(Beatings);
  }
  
  //Alternating Odd Even Leds
  for (int i = 0; i < 3; i++){
    digitalWrite(2,HIGH); digitalWrite(4,HIGH); digitalWrite(6,HIGH); 
    digitalWrite(8,HIGH); digitalWrite(10,HIGH); 
    delay(Beatings);
    digitalWrite(2,LOW); digitalWrite(4,LOW); digitalWrite(6,LOW); 
    digitalWrite(8,LOW); digitalWrite(10,LOW); 
    digitalWrite(3,HIGH); digitalWrite(5,HIGH); digitalWrite(7,HIGH); 
    digitalWrite(9,HIGH); digitalWrite(11,HIGH); 
    delay(Beatings);
    digitalWrite(3,LOW); digitalWrite(5,LOW); digitalWrite(7,LOW); 
    digitalWrite(9,LOW); digitalWrite(11,LOW); 
  }
  
  for (int i = 0; i < 1.5; i++) {
    allOn(); 
    delay(Beatings / 2); 
    allOff();
    delay(Beatings / 2); 
    allOn(); 
    delay(Beatings / 2);
    allOff();
    delay(Beatings / 2); 
  }
  
}

void The1975(){
  for (int i = 0; i < 1; i++) {
    allOn(); 
    delay(305);
    allOff();
    delay(305);
  }
  
  //Chasing effect from right to left
  for (int i = 0; i < 10; i++) {
    digitalWrite(i + 2, HIGH);
    delay(25); 
    digitalWrite(i + 2, LOW);
    delay(25); 
  }
  
  //Chasing effect from left to right
  for (int i = 9; i >= 0; i--) {
    digitalWrite(i + 2, HIGH);
    delay(30); 
    digitalWrite(i + 2, LOW);
    delay(30);
  }
  
  for (int i = 0; i < 3; i++) {
    allOn(); 
    delay(100);
    allOff();
    delay(100);
  }
  
  for(int j = 0; j < 4; j++) {
    for(int i = 0; i < 10; i += 2) {
      digitalWrite(i + 2, HIGH);
    }
    delay(304);
    for(int i = 0; i < 10; i += 2) {
      digitalWrite(i + 2, LOW);
    }
    for(int i = 1; i < 10; i += 2) {
      digitalWrite(i + 2, HIGH);
    }
    delay(304);
    for(int i = 1; i < 10; i += 2) {
      digitalWrite(i + 2, LOW);
    }
  }
  
  //Chasing effect from right to left
  for (int i = 0; i < 10; i++) {
    digitalWrite(i + 2, HIGH);
    delay(30); 
    digitalWrite(i + 2, LOW);
    delay(30); 
  }
  
  //Chasing effect from left to right
  for (int i = 9; i >= 0; i--) {
    digitalWrite(i + 2, HIGH);
    delay(30); 
    digitalWrite(i + 2, LOW);
    delay(30);
  }
  
  for (int i = 0; i < 2; i++) {
    allOn(); 
    delay(250);
    allOff();
    delay(250);
  }
  
  for(int j = 0; j < 3; j++) {
    for(int i = 0; i < 10; i += 2) {
      digitalWrite(i + 2, HIGH);
    }
    delay(304);
    for(int i = 0; i < 10; i += 2) {
      digitalWrite(i + 2, LOW);
    }
    for(int i = 1; i < 10; i += 2) {
      digitalWrite(i + 2, HIGH);
    }
    delay(304);
    for(int i = 1; i < 10; i += 2) {
      digitalWrite(i + 2, LOW);
    }
  }
  
  for(int i = 0; i < 9; i++) {
    digitalWrite(6 + i, HIGH);
    digitalWrite(7 - i, HIGH);
    delay(50);
    digitalWrite(6 + i, LOW);
    digitalWrite(7 - i, LOW);
  }
  
  for(int i = 0; i < 9; i++) {
    digitalWrite(i + 2, HIGH);
    digitalWrite(i + 3, HIGH);
    delay(100); 
    digitalWrite(i + 2, LOW);
  }
  for(int i = 8; i > 0; i--) {
    digitalWrite(i + 2, HIGH);
    digitalWrite(i + 1, HIGH);
    delay(100);
    digitalWrite(i + 3, LOW);
  }
  
  for(int i = 0; i < 9; i++) {
    digitalWrite(6 + i, HIGH);
    digitalWrite(7 - i, HIGH);
    delay(50);
    digitalWrite(6 + i, LOW);
    digitalWrite(7 - i, LOW);
  }
  
  for(int i = 0; i < 5; i++){
    int randomLED = random(2, 12);
    digitalWrite(randomLED, HIGH);
    delay(100);
    digitalWrite(randomLED, LOW);
    delay(200);
  }
  
  //Chasing effect from right to left
  for (int i = 0; i < 10; i++) {
    digitalWrite(i + 2, HIGH);
    delay(30); 
    digitalWrite(i + 2, LOW);
    delay(30); 
  }
  
  //Chasing effect from left to right
  for (int i = 9; i >= 0; i--) {
    digitalWrite(i + 2, HIGH);
    delay(30); 
    digitalWrite(i + 2, LOW);
    delay(30);
  }
  
  for(int i = 0; i < 1.5; i++) {
    for(int j = 0; j < 10; j+=2) {
      digitalWrite(j + 2, HIGH);  // Turn on even LEDs
    }
    delay(305);
    for(int j = 0; j < 10; j+=2) {
      digitalWrite(j + 2, LOW);   // Turn off even LEDs
    }
    delay(305);
    for(int j = 1; j < 10; j+=2) {
      digitalWrite(j + 2, HIGH);  // Turn on odd LEDs
    }
    delay(305);
    for(int j = 1; j < 10; j+=2) {
      digitalWrite(j + 2, LOW);   // Turn off odd LEDs
    }
    delay(200);
  }
  
  for (int i = 0; i < 1; i++) {
    allOn(); 
    delay(250);
    allOff();
    delay(250);
  }
  
   //Chasing effect from right to left
  for (int i = 0; i < 10; i++) {
    digitalWrite(i + 2, HIGH);
    delay(25); 
    digitalWrite(i + 2, LOW);
    delay(25); 
  }
  
  //Chasing effect from left to right
  for (int i = 9; i >= 0; i--) {
    digitalWrite(i + 2, HIGH);
    delay(30); 
    digitalWrite(i + 2, LOW);
    delay(30);
  }
  
  for (int i = 0; i < 1; i++) {
    allOn(); 
    delay(100);
    allOff();
    delay(100);
  }
  
}

void Uptown(){
  int beatDuration = 520;
  
  for (int i = 0; i < 5.5; i++) {
    allOn(); 
    delay(beatDuration);
    allOff();
    delay(beatDuration);
  }
  
  for (int i = 0; i < 4; i++) {
    allOn(); 
    delay(beatDuration/2);
    allOff();
    delay(beatDuration/2);
  }
  
   //Chasing effect from right to left
  for (int i = 0; i < 10; i++) {
    digitalWrite(i + 2, HIGH);
    delay(beatDuration / 5.5); 
    digitalWrite(i + 2, LOW);
  }
  
  //Chasing effect from left to right
  for (int i = 9; i >= 0; i--) {
    digitalWrite(i + 2, HIGH);
    delay(beatDuration / 5.5); 
    digitalWrite(i + 2, LOW);
  }
  
   //Chasing effect from right to left
  for (int i = 0; i < 10; i++) {
    digitalWrite(i + 2, HIGH);
    delay(beatDuration / 5.5); 
    digitalWrite(i + 2, LOW);
  }
  
  //Chasing effect from left to right
  for (int i = 9; i >= 0; i--) {
    digitalWrite(i + 2, HIGH);
    delay(beatDuration / 5.5); 
    digitalWrite(i + 2, LOW);
  }
  
   //Chasing effect from right to left
  for (int i = 0; i < 10; i++) {
    digitalWrite(i + 2, HIGH);
    delay(beatDuration / 5.5); 
    digitalWrite(i + 2, LOW);
  }
  
  //Chasing effect from left to right
  for (int i = 9; i >= 0; i--) {
    digitalWrite(i + 2, HIGH);
    delay(beatDuration / 5.5); 
    digitalWrite(i + 2, LOW);
  }
  
  //Center to outward Pattern
  for (int i = 0; i < 1 ; i++){
    digitalWrite(6, HIGH); digitalWrite(7, HIGH);
    delay(beatDuration);
    digitalWrite(6, LOW); digitalWrite(7, LOW);
    digitalWrite(5, HIGH); digitalWrite(8, HIGH);
    delay(beatDuration);
    digitalWrite(5, LOW); digitalWrite(8, LOW);
    digitalWrite(4, HIGH); digitalWrite(9, HIGH);
    delay(beatDuration);
    digitalWrite(4, LOW); digitalWrite(9, LOW);
    digitalWrite(3, HIGH); digitalWrite(10, HIGH);
    delay(beatDuration);
    digitalWrite(3, LOW); digitalWrite(10, LOW);
    digitalWrite(2, HIGH); digitalWrite(11, HIGH);
    delay(beatDuration);
    digitalWrite(2, LOW); digitalWrite(11, LOW);
  } 
  
   for (int i = 0; i < 2; i++) {
    allOn(); 
    delay(beatDuration);
    allOff();
    delay(beatDuration);
  }
  
  
}

void allOn(){
  for(int i = 2; i <= 11; i++){
    digitalWrite(i, HIGH); //Turn on all LEDs from PIN 2 TO 11
  }
}

void allOff(){
  for(int i = 2; i <= 11; i++){
    digitalWrite(i, LOW); //Turn on all LEDs from PIN 2 TO 11
  }
}
