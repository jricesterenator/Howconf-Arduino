/*
  Lab 1 - LED Flash
  
  This lab helps verify that the Arduino is working will get your feet wet
  doing something basic. This will also teach the basics of activating an LED
  and using a current limiting resistor.
  
  What will happen?
  The LED will flash on and off every second.
  
  Learn:
  - Basic Arduino program flow
  - Turn LED on/off
  - Current limiting resistor
  
  Schematic:
  + LED, 
    (+)lead to pin 2. 
    (-)lead to GND via 470 ohm resistor
  
 */

#define LED_1 2

void setup() {
 pinMode(LED_1, OUTPUT);  
}

void loop() {
 
  digitalWrite(LED_1, HIGH);
  delay(1000);
  digitalWrite(LED_1, LOW);
  delay(1000);
  
} 
