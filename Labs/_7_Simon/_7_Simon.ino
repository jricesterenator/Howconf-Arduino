/*
  Lab 7. This lab puts everything together in a Simon + Bop It like game.
  
  Note: if it's complains about missing NOTE_C4, etc constants, load the 
  toneMelody example sketch and compile it first. Something goes wrong
  loading the tone library from here sometimes.
  
  Schematic:
  - 3 LEDs
    (+)leads to pins 2,3,5
    (-)leads to GND, each via 470 ohm resistor
 
  - Push button
    -1 lead to +5v
    -Other lead via 10K ohm pull-down resistor to GND, also connect this
     lead to pin 8
 
  - Potentiometer - outer pins wired to 5v,GND. Wiper (middle wire) wired to A0
  
  - IR LED as emitter
    (-)lead via 220 ohm resistor to GND
    (+)lead to +5v
    
  - IR (must be wrapped with heatshrink and electrical tape or IT WONT WORK!)
    Also, place an inch away from emitter.
    (+)lead to A1  (and to yellow (-)lead)
    (-)lead to +5v
    
  - RED LED
    (+)lead to GND
    (-)lead to A1 (and to IR LED sensor (+)lead)

  ADD:
  + Buzzer [optional]
    (+)lead to pin 9
    (-)lead to GND
    
 */
#include "pitches.h"

/*
  The light level for counting as a WAVE action. For help figuring this
  out. Enable DEBUG_LIGHT, and look in the serial console for a good
  light level when you wave your hand over it.
 */
#define LIGHT_TRIGGER_LEVEL 200
#define DEBUG_LIGHT true

//The amount of pot twist for TWIST action.
#define POT_TRIGGER_LEVEL 150
#define DEBUG_POT false


//Digital
#define LED_1 2  //Push LED, Red
#define LED_2 3  //Twist LED, Yellow
#define LED_3 5  //Wave LED, Green

#define BUTTON 8
#define SPEAKER 9

//Analog
#define POT 0
#define LIGHT 1

/* 
Game Constants
=========================================
*/

// The number of levels in the game
#define NUM_LEVELS 5

// The amount of time to wait before resetting the game after a win/loss (in ms)
#define GAME_RESET_DELAY 3000

// The amount of time to show each LED when teaching the sequence
#define TEACH_SHOW_TIME 500


#define STATE_NEW 0
#define STATE_TEACH 1
#define STATE_USER 2
#define STATE_LOSE 3
#define STATE_WIN 4

#define PUSH 0
#define TWIST 1
#define WAVE 2  


String activities[] = {"Push It!", "Twist it!", "Wave it!"};
int leds[3];

int game_state;
int level;
int * levels;

int prevButtonState = LOW;
int game_prev_pot = 0;
int game_prev_button = LOW;

/*
/Game Constants
=========================================
*/

void setup() {
  
  pinMode(BUTTON, INPUT);
  
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);


  // Read in the value from an empty analog port to seed. The random noise
  // on this port is a good seed.
  randomSeed(analogRead(5));
  
  Serial.begin(9600);
  
  game_init(NUM_LEVELS, LED_1, LED_2, LED_3);
  
}

void loop() {
  
  game_loop(); 
  
  delay(50);
}

int getUserInput() {

  /* Check button */
  if(digitalRead(BUTTON) != game_prev_button) {
    return PUSH;
  }

  /* Check pot */
  int potVal = analogRead(POT);
  if(DEBUG_POT) {
    Serial.print("POT=");
    Serial.println(potVal);
  }
  if(abs(potVal - game_prev_pot) > POT_TRIGGER_LEVEL) {
    return TWIST;
  }

  /* Check light */
  int lightVal = readLight();
  if(DEBUG_LIGHT) {
    Serial.print("LIGHT=");
    Serial.println(lightVal);
  }
  if(lightVal > LIGHT_TRIGGER_LEVEL) {
    return WAVE;
  }
  
  return -1;  
}

int readLight() {
  /*
    Because the analog inputs are multiplexed, the values on other analog
    ports can mess with this. In this case, the pot messed with the light
    reading. This helps work around that.
   */
  analogRead(LIGHT); //Causes Arduino to switch to the analog port
  delay(50); //Wait a bit for things to settle
  return analogRead(LIGHT); //Read a good value
}

boolean detectClick() {
  int buttonState = digitalRead(BUTTON);
  //Button click = state change and now up
  if(buttonState != prevButtonState) {
    prevButtonState = buttonState;
    if(buttonState == LOW) {
      return true;
    }
  }
  return false;
}

void showAllLEDs() {
  digitalWrite(LED_1, HIGH);
  digitalWrite(LED_2, HIGH);
  digitalWrite(LED_3, HIGH);
}

void clearLEDs() {
  digitalWrite(LED_1, LOW);
  digitalWrite(LED_2, LOW);
  digitalWrite(LED_3, LOW);
}

void cycleLEDs() {
  flashLED(LED_1, 100);
  flashLED(LED_2, 100);
  flashLED(LED_3, 100);  
}

void flashLED(int led, int flashTime) {
  digitalWrite(led, HIGH);
  delay(flashTime);
  digitalWrite(led, LOW);
}


/*
  Initialize some game parameters
*/
void game_init(int num_levels, int push_led, int twist_led, int wave_led) {
  leds[0] = push_led;
  leds[1] = twist_led;
  leds[2] = wave_led;
  levels = (int*)malloc(num_levels * sizeof(int));

  reset_game();
}

void reset_game() {
  level = 0;
  game_state = STATE_NEW;
}

/*
    All the game logic is in here.
*/
void game_loop() {

  if(game_state == STATE_WIN) {

    Serial.println("YOU WIN!");

    //Show the GREEN LED to indicate WIN
    clearLEDs();
    digitalWrite(LED_3, HIGH);
    
    playWinSong();


    /* Wait a sec, then restart the game */
    Serial.print("Resetting game...");
    delay(GAME_RESET_DELAY);
    Serial.println("Reset.");

    reset_game();
    return;
    
    
  } else if(game_state == STATE_LOSE) {
    Serial.println("YOU LOSE!");

    //Show the RED LED to indicate LOSS
    clearLEDs();
    digitalWrite(LED_1, HIGH);
    
    playLoseSong();

    /* Wait a sec, then restart the game */
    Serial.print("Resetting game...");
    delay(GAME_RESET_DELAY);
    Serial.println("Reset.");


    reset_game();
    return;


  } else if(game_state == STATE_NEW) {

    reset_game();

    /*
       This causes the LEDs to flash while we're waiting for the user
       to click the button to start the game. I've put the click detection
       in between the flashes so we're more likely to catch the button click.
       Otherwise, the user might click during a delay().
    */
    int flash_rate = 75;
    delay(flash_rate);
    showAllLEDs();

    //When the user clicks, we start to teach him a sequence.
    if(detectClick()) {
      game_state = STATE_TEACH;   
      return;
    }
    
    delay(flash_rate);
    clearLEDs();
      
    return;
    
    
  } else if(game_state == STATE_TEACH) {

    //Cycle the LEDs a few times for effect. Indicates we're about to teach
    clearLEDs();
    for(int i=0; i<4; ++i) {
      cycleLEDs();
    }
    clearLEDs();
    delay(1000);
    
    
    /* --- Add/show new activity --- */

    //Win detection
    if(level >= NUM_LEVELS) {
      game_state = STATE_WIN;
      return;
    } 
     
    //Add a random activity to the sequence
    //http://arduino.cc/en/Reference/random
    int rand = random(0, 3);
    levels[level] = rand;

    Serial.print("New Action: ");
    Serial.print(level);
    Serial.print(" - ");
    Serial.println(activities[rand]);


    //Show the pattern to the user
    for(int l=0; l<=level; ++l) {
      
      clearLEDs();

      //Show & log each sequence action
      int actionVal = levels[l];
      digitalWrite(leds[actionVal], HIGH);

      Serial.print(l);
      Serial.print("-");
      Serial.println(activities[actionVal]);

      
      delay(TEACH_SHOW_TIME); //Amount of time to show
      clearLEDs();
      
      if(l == level)
        delay(250); //delay between TEACH and USER
      else
        delay(500); //Amount of blank time between each activity
 
    } 
    
    ++level;
    Serial.println("----------------------------");
    Serial.println("What did you learn?");

    //Now it's time for user input
    game_state = STATE_USER;    


  } else if(game_state == STATE_USER) { //Get user input

    //Flash LEDs to indicate it's time for user input    
    showAllLEDs();
    delay(75);
    clearLEDs();

    /*
        For each activity in the sequence, we wait for user input. If the
        input (i.e. button press, pot press, light wave) matches what
        we're expecting, we go to the next sequence item. If it doesn't match,
        it's GAME OVER.
     */

    for(int l=0; l<level; ++l) {
      game_prev_pot = analogRead(POT); //Reset pot state so we have a good baseline for next time
      
      int expected = levels[l];
      Serial.print("EXPECTED: ");
      Serial.println(activities[expected]);


      int CONFIRM_LED_TIME = 350; //Amount of time to show confirmation LED
      int showLEDTime = 0; //Used to track when confirm LED was lit
      
      int input = -1;
      while(input < 0) {

        //Clear confirmation LED after it's been up long enough
        if(millis() - showLEDTime > CONFIRM_LED_TIME) {
          showLEDTime = 0;
          clearLEDs();
        }

        input = getUserInput();
        if(input >= 0) {
          Serial.print("YOU DID: ");
          Serial.println(activities[input]);

          //Show the LED corresponding to the action the user performed
          clearLEDs();
          digitalWrite(leds[input], HIGH);
          showLEDTime = millis(); //Record activation time so we can clear it

          //Correct/Incorrect detection
          if(input != expected) {
            Serial.println("INCORRECT");
            game_state = STATE_LOSE;
            return;
            
          } else {
            Serial.println("CORRECT");
            delay(CONFIRM_LED_TIME);
            break;
          }

        } else {
            //If no input, take a breath to decrease the poll rate.
            delay(100);
        }
      }
    }
    //If you made it this far, time for next level
    game_state = STATE_TEACH;
  }

}


/*
    The song to play when you win.
*/
void playWinSong() {

  int melody[] = {NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};

  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] =  { 4, 8, 8, 4,4,4,4,4 };

  playSong(melody, noteDurations, 8);
}

/*
    The song to play when you lose.
*/
void playLoseSong() {

  int melody[] = {NOTE_C3, NOTE_C3, NOTE_C3, NOTE_A2};

  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] =  { 5, 5, 5, 1 };

  playSong(melody, noteDurations, 4);
}

/*
    Lifted from the Arduino Tone tutorial.
    http://arduino.cc/en/Tutorial/Tone
*/
void playSong(int melody[], int noteDurations[], int numNotes) {
    // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < numNotes; thisNote++) {

    // to calculate the note duration, take one second
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/ noteDurations[thisNote];
    tone(SPEAKER, melody[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(SPEAKER);
  }
  noTone(SPEAKER);

}
