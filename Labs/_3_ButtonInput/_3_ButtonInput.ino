/*
  Lab 3 - The lab adds a push button input. Pushing the button will trigger the
  3 LEDs to cycle.
  
  Part 1. Enable Serial debugging and print button value with delay (tada!) 
  Part 2. Click detection and serial out "Button Clicked"
  Part 3. Make button click trigger LEDs
  
  Learn:
  - Buttons
  - Click detection
  - Pull up/pull down resistors
  - Serial debugging
  - Debouncing (2 kinds. Delay and sampling)
    
  Schematic:
  - 3 LEDs
    (+)leads to pins 2,3,5
    (-)leads to GND, each via 470 ohm resistor
    
  Add:
  + Push button
    -1 lead to +5v
    -Other lead via 10K ohm pull-down resistor to GND, also connect this
     lead to pin 8
 */

#define LED_1 2
#define LED_2 3
#define LED_3 5
#define BUTTON 8

int prevButtonState = LOW;

void setup() {
  
  pinMode(BUTTON, INPUT);
  
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  
  int buttonState = digitalRead(BUTTON);  
  
  //Button click = state change and now up
  if(buttonState != prevButtonState) {
    if(buttonState == LOW) {
      Serial.println("Button Clicked");
           
      cycleLEDs();
     
    }
    
    prevButtonState = buttonState;
  }
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
