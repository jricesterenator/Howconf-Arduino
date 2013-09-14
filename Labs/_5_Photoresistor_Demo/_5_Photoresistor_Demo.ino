/*
  Demo 5.1. This demos using a photoresistor.
  
  What will happen?
  Waving over the light sensor will cause the 3rd LED to dim proportionally.
  
  Learn:
  - Photoresistor
    
  Schematic:
  - LED
    (+)lead to pin 5
    (-)lead to GND, via 470 ohm resistor
  
  Add:
  + Photoresistor (wired just like button)
    -1 lead to +5v
    -Other lead via 1K pull-down resistor (or any other high value resistor) 
     to GND; also connect this lead to A1.
 */
 
/*
  These values help make the photoresistor dimmer work right.
  Set them by serial-outputting the light values and looking at them.
  -HIGH is ambient (or expected max) light. 
  -LOW is the value when you put your finger on the sensor and it's as 
   dark as expected.
*/
#define AMBIENT_LIGHT_LOW 150
#define AMBIENT_LIGHT_HIGH 350


//Digital
#define LED_3 5

//Analog
#define LIGHT 1

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
    
  
  delay(10);
} 

/* This makes the LED dimming along with the pot work better */
int getScaledLightVal(int lightVal) {
  if(lightVal > AMBIENT_LIGHT_HIGH)
    lightVal = AMBIENT_LIGHT_HIGH;
  else if(lightVal < AMBIENT_LIGHT_LOW)
    lightVal = AMBIENT_LIGHT_LOW;
  
  return map(lightVal, AMBIENT_LIGHT_LOW, AMBIENT_LIGHT_HIGH, 0, 255);
}

