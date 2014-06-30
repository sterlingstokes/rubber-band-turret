#include <Servo.h> 
 
Servo bigServo;  // create object for big servo
Servo littleServo; // create object for little servo

// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 2;    // the number of the pushbutton pin
const int firePin = 13;      // the number of the fire LED
const int standbyPin = 12;   // the number of the turret LED
const int bigServoPin = 9;  // big servo pin 
const int littleServoPin = 10; // little servo pin
const int potPin = A0;    // select the input pin for the potentiometer

// Variables will change:
int ledState = HIGH;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
int val = 0;    // variable to store the pot val
int previousVal = 0; // store the previos val to check for turret LED

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(firePin, OUTPUT);
  pinMode(standbyPin, OUTPUT);

  // set initial LED state
  bigServo.attach(bigServoPin);  // attaches the servo on pin 9 to the servo object
  littleServo.attach(littleServoPin);  // attaches the servo on pin 10 to the servo object
  
  bigServo.write(0);
  littleServo.write(0);
  
  digitalWrite(standbyPin, HIGH);
}

void fire()
{
  digitalWrite(standbyPin, LOW);
  digitalWrite(firePin, HIGH);
  
  littleServo.write(45);
  delay(100);
  littleServo.write(0);
  
  digitalWrite(firePin, LOW);
  digitalWrite(standbyPin, HIGH);
}

void loop() {
  val = analogRead(potPin);            // reads the value of the potentiometer (value between 0 and 1023) 
  val = map(val, 0, 1023, 0, 179);     // scale it to use it with the servo (value between 0 and 180)
  bigServo.write(val); // sets the servo position according to the scaled value
  delay(15); // waits for the servo to get there
  
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button 
  // (i.e. the input went from LOW to HIGH),  and you've waited 
  // long enough since the last press to ignore any noise:  

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        fire();
      }
    }
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;
}