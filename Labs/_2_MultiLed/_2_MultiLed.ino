/*
  Lab 2 - Multi LED Flash
  
  This lab reinforces LED activation by using multiple LEDs. It also gets 
  you writing common code in functions.
  
  What will happen?
  We'll cycle through the LEDs and flash each one for a second.
  
  Part 1. Base the code on lab one, no functions.
  Part 2. Extract common logic into a function.
    
  Learn:
  - More LEDs
  - Each LED needs its own current limiting resistor
  - Common code in functions
     
  Schematic:
  - LED, 
    (+)lead to pin 2. 
    (-)lead to GND via 470 ohm resistor
    
  Add:
  + 2 LEDs
    (+)leads to pins 3,5
    (-)leads to GND, each via 470 ohm resistor
    
 */

#define LED_1 2
#define LED_2 3
#define LED_3 5

void setup() {
 pinMode(LED_1, OUTPUT);
 pinMode(LED_2, OUTPUT);
 pinMode(LED_3, OUTPUT);
}

void loop() {
  flashLED(LED_1, 500);
  flashLED(LED_2, 500);
  flashLED(LED_3, 500);  
} 

void flashLED(int led, int flashTime) {
  digitalWrite(led, HIGH);
  delay(flashTime);
  digitalWrite(led, LOW);
}
