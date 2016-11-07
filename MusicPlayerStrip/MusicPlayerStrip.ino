/*
SparkFun Inventor's Kit 

Authors: Mangirish Wagle
         Gourav Shenoy

Arduino code file for Musical Fingerboard.

*/

const int SOFT_POT_PIN = 0;      // Analog input pin
const int FLEX_PIN = 1;          // Flex sensor input pin
const int PHOTO_RESIS_PIN = 2;   // Flex sensor input pin

const int buzzerPin = 8;       // Buzzer output pin

// RGB LED Pins
const int RED_PIN = 9;
const int GREEN_PIN = 10;
const int BLUE_PIN = 11;

const int button_c = 1;
const int button_a = 2;
const int button_g = 3;

const int light_thresh = 30;

char button = 'c';
long flex_init_val = 0;

void setup()
{
  // Set up the pushbutton pins to be an input:
  pinMode(button_c, INPUT);
  pinMode(button_a, INPUT);
  pinMode(button_g, INPUT);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.begin(9600);

  flex_init_val = analogRead(FLEX_PIN);
}

/**
 * Funtion that returns the variance of the sound freq. based on flex analog read.
 */
int get_variance()
{

  long diff=0, variance = 0, current_flex_val;
  int quantum = 10;
  int threshold_diff = 10;

  current_flex_val = analogRead(FLEX_PIN);
  diff = current_flex_val - flex_init_val;
  variance = (diff/threshold_diff) * quantum;

  return variance;  
}

/**
 * Function that shuts off the RGB LED.
 */
int reset_led() {

  // No light on LED
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
}

int glow_led(int pin) {

  if(pin == RED_PIN) {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW);
  }
  if(pin == GREEN_PIN) {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
  }
  if(pin == BLUE_PIN) {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);
  }
}

void loop()
{
  int button_a_state, button_c_state, button_g_state;
  
  // Read the voltage from the softpot (0-1023)
  int soft_pot_sensor_value, flex_sensor_value, photo_resist_sensor_value;

  // Reading soft potentiometer value.
  soft_pot_sensor_value = analogRead(SOFT_POT_PIN);

  // Reading flex sensor value.
  flex_sensor_value = analogRead(FLEX_PIN);

  // Reading photo resister sensor value.
  photo_resist_sensor_value = analogRead(PHOTO_RESIS_PIN);

  button_a_state = digitalRead(button_a);
  button_c_state = digitalRead(button_c);
  button_g_state = digitalRead(button_g);

  if(button_a_state == LOW) {
    button = 'a';
  } else if(button_g_state == LOW) {
    button = 'g';
  } else if(button_c_state == LOW) {
    button = 'c';
  }

  // Calling without pin param to keep it off.
  reset_led();
  
  if(soft_pot_sensor_value>100 && soft_pot_sensor_value<=284) {
    tone(buzzerPin, frequency('d') + get_variance(), 10);
    if(photo_resist_sensor_value <= light_thresh){
      glow_led(RED_PIN);
    }
  } else if (soft_pot_sensor_value>284 && soft_pot_sensor_value<=468) {
    tone(buzzerPin, frequency('r') + get_variance(), 10);
    if(photo_resist_sensor_value <= light_thresh){
      glow_led(GREEN_PIN);
    }
  }
  else if (soft_pot_sensor_value>468 && soft_pot_sensor_value<=652) {
    tone(buzzerPin, frequency('m') + get_variance(), 10);
    if(photo_resist_sensor_value <= light_thresh){
      glow_led(BLUE_PIN);
    }
  }
  else if (soft_pot_sensor_value>652 && soft_pot_sensor_value<=836) {
    tone(buzzerPin, frequency('f') + get_variance(), 10);
    if(photo_resist_sensor_value <= light_thresh){
      glow_led(RED_PIN);
    }
  }
  else if (soft_pot_sensor_value>836 && soft_pot_sensor_value<=1023) {
    tone(buzzerPin, frequency('s') + get_variance(), 10);
    if(photo_resist_sensor_value <= light_thresh){
      glow_led(GREEN_PIN);
    }
  }

  // Serial.println(photo_resist_sensor_value);
}

/**
 * Function that returns the frequency of a note based on the currect selected scale.
 */
int frequency(char note) 
{
  // This function takes a note character (a-g), and returns the
  // corresponding frequency in Hz for the tone() function.

  int i;
  const int numNotes = 8;  // number of notes we're storing

  // The following arrays hold the note characters and their
  // corresponding frequencies. The last "C" note is uppercase
  // to separate it from the first lowercase "c". If you want to
  // add more notes, you'll need to use unique characters.

  // For the "char" (character) type, we put single characters
  // in single quotes.

  char names[] = { 'd', 'r', 'm', 'f', 's', 'l', 'c', 'D' };
  int frequencies_c[] = {262, 294, 330, 349, 392, 440, 494, 523};
  int frequencies_g[] = {392, 440, 494, 523, 587, 659, 740, 784};
  int frequencies_a[] = {440, 494, 554, 587, 659, 740, 831, 880};

  // Now we'll search through the letters in the array, and if
  // we find it, we'll return the frequency for that note.

  for (i = 0; i < numNotes; i++)  // Step through the notes
  {
    if (names[i] == note)         // Is this the one?
    {
      if(button == 'a') {
        return(frequencies_a[i]);     // Yes! Return the frequency A
      } else if (button == 'g') {
        return(frequencies_g[i]);     // Yes! Return the frequency G
      } else if (button == 'c') {
        return(frequencies_c[i]);     // Yes! Return the frequency C
      }
      
    }
  }
  return(0);  // We looked through everything and didn't find it,
              // but we still need to return a value, so return 0.
}
