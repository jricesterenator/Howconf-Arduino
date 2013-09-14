/*
  Lab 6. This lab uses the LED-as-light-sensor idea for IR object detection.
  
  What will happen?
  When you wave your hand over the IR emitter/receiver, the value printed
  to the serial console will be proportional to the light reflected.
  
  Learn:
  - Basic IR object detection using 2 IR LEDs
  http://iopscience.iop.org/1742-6596/76/1/012054/pdf/1742-6596_76_1_012054.pdf
  
  Schematic:
  - 3 LEDs
    (+)leads to pins 2,3,5
    (-)leads to GND, each via 470 ohm resistor
  - Push button
    -1 lead to +5v
    -Other lead via 10K ohm pull-down resistor to GND, also connect this
     lead to pin 8
  - Potentiometer - outer pins wired to 5v,GND. Wiper (middle wire) wired to A0
  
  Add:
  + IR LED as emitter
    (-)lead via 220 ohm resistor to GND
    (+)lead to +5v
    
  + IR (must be wrapped with heatshrink and electrical tape or IT WONT WORK!)
    Also, place an inch away from emitter.
    (+)lead to A1  (and to yellow (-)lead)
    (-)lead to +5v
    
  + RED LED
    (+)lead to GND
    (-)lead to A1 (and to IR LED sensor (+)lead)
    
 */

//Digital
#define LED_1 2
#define LED_2 3
#define LED_3 5
#define BUTTON 8

//Analog
#define POT 0
#define LIGHT 1

int prevButtonState = LOW;

void setup() {
  
  pinMode(BUTTON, INPUT);
  
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  
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
  //Can variable write because pin 3 is PWM.
  //PWM output is 0-255. Analog in is 0-1023.
  int scaledVal = map(analogRead(POT), 0, 1023, 0, 255);
  analogWrite(LED_2, scaledVal);
    
  /* LIGHT */
  int lightVal = analogRead(LIGHT);
  Serial.println(lightVal);  
  
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
