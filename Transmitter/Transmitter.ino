/*
* Arduino Wireless Communication Tutorial
*     Example 1 - Transmitter Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Radio set-uup
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

// Payload set-up
int num = 0;
char cstr[16];

// Pin mapping set-up
const int red = 2;
const int blue = 3;
const int yellow = 4;
const int green = 5;
const int buttonPin = 9;

// Helper variables
bool swung = false;
bool hit = false;
int swingWindow = 1000; // 1 second
char text[32];
char recv[32];
const char start[] = "Start!";
bool waitingForHitBack = false;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  
  radio.begin();
  radio.openWritingPipe(address);      // For writing
  radio.openReadingPipe(0, address);   // For Reading
  radio.setPALevel(RF24_PA_MIN);

  radio.startListening(); // Read mode
  Serial.println("Push button to throw pitch at other player, or await pitch yourself.");
}

void loop() {
  
  
  while(true){

      if (digitalRead(buttonPin) == HIGH) { // Throw pitch to other player
        radio.stopListening(); // Write mode
        Serial.println("Throwing pitch!");
        radio.write(&start, sizeof(start));
        Serial.println("Pitch thrown");
        waitingForHitBack = true;
        break;
      }

    radio.startListening(); // Read mode
    Serial.println("Listening for pitch...");
    if (radio.available()) {
      radio.read(&recv, sizeof(recv));
      Serial.print("Received text: ");
      Serial.println(recv);
      if (strcmp(recv,"Start!") == 0){
        strncpy(recv, "", 32);
        break;
      }
    }
    
  }

  if (waitingForHitBack){
    radio.startListening(); // Read mode
    while (true){
      if (radio.available()) {
        radio.read(&recv, sizeof(recv));
        Serial.print("Received text: ");
        Serial.println(recv);
        if (strcmp(recv,"The player hit the ball!") == 0){
          strncpy(recv, "", 32);
          waitingForHitBack = false;
          break; // go to pitch below
        }
        else if (strcmp(recv,"The player missed the ball!") == 0){
          strncpy(recv, "", 32);
          winner();
          waitingForHitBack = false;
          return;
        }
      }
    }
  }
    
  radio.stopListening(); // Write mode
  Serial.println("Received pitch");
  pitch();// start "pitching" the ball to the player
  
  if (hit) { // If the player hit the ball....
    strncpy(text, "The player hit the ball!", 32);
  }
  else { // If the player missed the ball....
    strncpy(text, "The player missed the ball!", 32);
  }

  radio.stopListening(); // Write mode
  radio.write(&text, sizeof(text)); // Let the other arduino know how the player did
  //delay(1000); 
}


void pitch() {
  swung = false;
  hit = false;
  
  digitalWrite(red, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);
  digitalWrite(red, LOW);   // turn the LED off by making the voltage LOW
  delay(1);

  digitalWrite(blue, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);
  digitalWrite(blue, LOW);   // turn the LED off by making the voltage LOW
  delay(1);

  digitalWrite(yellow, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);
  digitalWrite(yellow, LOW);   // turn the LED off by making the voltage LOW
  delay(1);

  if (digitalRead(buttonPin) == HIGH) { // check if the pushbutton is pressed. If it is, the player swung too early
    swung = true; // This will prevent the user from swinging again (and getting away with just holding down the button)
  }
  
  digitalWrite(green, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(swingWindow);
  if (!swung && digitalRead(buttonPin) == HIGH) { // If not swung before and currently swinging...
    swung = true;
    hit = true; // ... Then the player hit the ball!
  }
  digitalWrite(green, LOW);   // turn the LED off by making the voltage LOW
  delay(1);
}



void winner() { // Strobe lights to show victory
  digitalWrite(red, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(blue, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(yellow, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(green, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);
  digitalWrite(red, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(blue, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(yellow, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(green, LOW);   // turn the LED off by making the voltage LOW
  delay(500);
  digitalWrite(red, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(blue, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(yellow, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(green, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);
  digitalWrite(red, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(blue, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(yellow, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(green, LOW);   // turn the LED off by making the voltage LOW
  delay(500);
  digitalWrite(red, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(blue, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(yellow, HIGH);  // turn the LED on (HIGH is the voltage level)
  digitalWrite(green, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);
  digitalWrite(red, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(blue, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(yellow, LOW);   // turn the LED off by making the voltage LOW
  digitalWrite(green, LOW);   // turn the LED off by making the voltage LOW
  delay(500);
}
