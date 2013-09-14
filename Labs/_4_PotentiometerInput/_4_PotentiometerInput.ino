/*
  Lab 4- This lab adds a potentiometer input.
  
  What will happen?
  Twisting the pot will control the brightness of the 2nd LED.
  
  Note: LED 2 must be on a PWM output.
  
  Learn:
  - Pots as voltage divider (how we're using it)
  - Pull up/pull down resistors
  - Analog inputs
  - map() function
    
  Schematic:
  - 3 LEDs
    (+)leads to pins 2,3,5
    (-)leads to GND, each via 470 ohm resistor
  - Push button
    -1 lead to +5v
    -Other lead via 10K ohm pull-down resistor to GND, also connect this
     lead to pin 8
     
  Add:
  + Potentiometer - outer pins wired to 5v,GND. Wiper (middle wire) wired to A0
 */

//Digital
#define LED_1 2
#define LED_2 3
#define LED_3 5
#define BUTTON 8

//Analog
#define POT 0

int prevButtonState = LOW;

void setup() {
  
  pinMode(BUTTON, INPUT);
  
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  
  //Analog inputs don't need to be defined
    
  Serial.begin(9600);
}

void loop() {
  
  /* BUTTON */
  int buttonState = digitalRead(BUTTON);
  //Button click = state change and now up
  if(buttonState != prevButtonState) {
    if(buttonState == LOW) {
      Serial.println("Button Clicked");
           
      cycleLEDs();
     
    }
    
    prevButtonState = buttonState;
  }
  
  
  /* POT */
  Serial.println(analogRead(POT));
  //Can variable write because pin 3 is PWM.
  //PWM output is 0-255. Analog in is 0-1023.
  int scaledVal = map(analogRead(POT), 0, 1023, 0, 255);
  analogWrite(LED_2, scaledVal);
  
  
  delay(100);
} 


void cycleLEDs() {
  flashLED(LED_1, 500);
  flashLED(LED_2, 500);
  flashLED(LED_3, 500);  
}

void flashLED(int led, int flashTime) {
  digitalWrite(led, HIGH);
  delay(flashTime);
  digitalWrite(led, LOW);
}
