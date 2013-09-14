/*
  Demo 5.2. This demos using an LED as a photoresistor.
  
  What will happen?
  Waving over the LED will cause the 3rd LED to dim proportionally.
  
  Learn:
  - Photoresistor
    
  Schematic:
  - 1 LED
    (+)lead to pins 
    (-)lead to GND, via 470 ohm resistor
  
  Add:
  + IR LED (shrouded)
    - (-)lead to +5V
    - (+)lead to A1
  + RED LED
    - (-)lead to A1
    - (+)lead to GND
    
 */

//Digital
#define LED_3 5

//Analog
#define LIGHT 1

/*
  These values help make the photoresistor dimmer work right.
  Set them by serial-outputting the light values and looking at them.
  -HIGH is ambient (or expected max) light. 
  -LOW is the value when you put your finger on the sensor and it's as 
   dark as expected.
*/
#define AMBIENT_LIGHT_LOW 100
#define AMBIENT_LIGHT_HIGH 1000

int prevButtonState = LOW;

void setup() {
 
  pinMode(LED_3, OUTPUT);
  
  //Analog inputs don't need to be defined
 
  digitalWrite(LED_3, LOW);

  Serial.begin(9600);
  
}

void loop() {

  /* LIGHT */
  int lightVal = analogRead(LIGHT);
  Serial.println(lightVal);
  int scaledLightVal = getScaledLightVal(lightVal);
  analogWrite(LED_3, scaledLightVal);
    
  
  delay(100);
} 

/* This makes the LED dimming along with the pot work better */
int getScaledLightVal(int lightVal) {
  if(lightVal > AMBIENT_LIGHT_HIGH)
    lightVal = AMBIENT_LIGHT_HIGH;
  else if(lightVal < AMBIENT_LIGHT_LOW)
    lightVal = AMBIENT_LIGHT_LOW;
  
  return map(lightVal, AMBIENT_LIGHT_LOW, AMBIENT_LIGHT_HIGH, 0, 255);
}

