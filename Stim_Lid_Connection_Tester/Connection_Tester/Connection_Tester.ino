
int WELL_NUMBER = 24; // number of wells in the plate (half the number of channels to be tested)
int STARTING_PIN = 2; // pin on arduino mega to be mapped to A6+
int NUM_COLS = 6; // number of columns in 24 well plate

/* Pins on the arduino are mapped to a particular well id e.g. starting pin is A6+, starting pin + 1 is A5+,
 starting pin + 6 is B6+, starting pin + 24 is A6-, etc.) The serial monitor will display which pins are connected together 
 by their well ids*/ 
String rows[] = {"A", "B", "C", "D"}; // row numbers in 24 well plate
String cols[] = {"6", "5", "4", "3", "2", "1"}; // column numbers in 24 well plate
String pol[] = {"+", "-"}; // polarity of the channel

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int well = 0; well < WELL_NUMBER * 2; well++) {
    pinMode(STARTING_PIN + well, INPUT_PULLUP); // all pins to pull up 20k resistors
  }
}

void loop() {
  if (Serial.available() > 0) {
    Serial.read(); // serial byte must be sent to start process, can be anything
    String connections[WELL_NUMBER*2]; // entry at idx in connections[idx] starts with a well id and is followed by ids of other wells the first well is connected to
    String ideals[WELL_NUMBER*2]; // entry at idx in ideals[idx] starts with a well id followed the id the of the well that the first well should be connected to if the lid is working properly

    for (int test_pin = 0; test_pin < WELL_NUMBER; test_pin++) { // check what positive pins are connected to
      connections[test_pin] = rows[test_pin / NUM_COLS] + cols[test_pin % NUM_COLS] + pol[0];
      ideals[test_pin] = rows[test_pin / NUM_COLS] + cols[test_pin % NUM_COLS] + pol[0] + rows[test_pin / NUM_COLS] + cols[test_pin % NUM_COLS] + pol[1];
      pinMode(STARTING_PIN + 2*test_pin, OUTPUT);
      digitalWrite(STARTING_PIN + 2*test_pin, 0); // set test pin low

      for (int well = test_pin; well < WELL_NUMBER; well++) { // iterate through other positive pins, by column, then by row, to see what other pins were pulled low by the the test pin
        if (!digitalRead(STARTING_PIN + 2 * well) && well != test_pin) { 
          connections[test_pin].concat(rows[well / NUM_COLS] + cols[well % NUM_COLS] + pol[0]);          
        }
      }

      for (int well = test_pin; well < WELL_NUMBER; well++) { // iterate through negative pins
        if (!digitalRead(STARTING_PIN + 2 * well + 1)) { 
          connections[test_pin].concat(rows[well / NUM_COLS] + cols[well % NUM_COLS] + pol[1]);          
        }
      }
      pinMode(STARTING_PIN + 2*test_pin, INPUT_PULLUP);
    }

    for (int test_pin = 0; test_pin < WELL_NUMBER; test_pin++) { // check what negative pins are connected to
      connections[test_pin + WELL_NUMBER] = rows[test_pin / NUM_COLS] + cols[test_pin % NUM_COLS] + pol[1];
      ideals[test_pin + WELL_NUMBER] = rows[test_pin / NUM_COLS] + cols[test_pin % NUM_COLS] + pol[1] + rows[test_pin / NUM_COLS] + cols[test_pin % NUM_COLS] + pol[0];
      pinMode(STARTING_PIN + 2*test_pin + 1, OUTPUT);
      digitalWrite(STARTING_PIN + 2*test_pin + 1, 0);

      for (int well = test_pin; well < WELL_NUMBER; well++) { // iterate through other negative pins
        if (!digitalRead(STARTING_PIN + 2 * well + 1) && well != test_pin) { 
          connections[test_pin + WELL_NUMBER].concat(rows[well / NUM_COLS] + cols[well % NUM_COLS] + pol[1]);          
        }
      }

      for (int well = test_pin; well < WELL_NUMBER; well++) { // iterate through positive pins -- redundant, but makes presentation easier
        if (!digitalRead(STARTING_PIN + 2 * well)) { 
          connections[test_pin + WELL_NUMBER].concat(rows[well / NUM_COLS] + cols[well % NUM_COLS] + pol[0]);          
        }
      }
	  
      pinMode(STARTING_PIN + 2*test_pin + 1, INPUT_PULLUP);
    }

    bool same = true; // does connections[] match ideals[]?
    Serial.println("Begin Test");
    Serial.println();
    for (int i = 0; i < 2 * WELL_NUMBER; i++) {
      // Serial.println(ideals[i]);
      // Serial.println(connections[i]);

      if (!ideals[i].equals(connections[i])){
        same = false; 
        Serial.print(connections[i]);
        if (connections[i].length() == 3) { // connections[i] lists only one well id
          Serial.print( " is open");
        } else {
          Serial.print(" are shorted"); // connections[i] lists wrong well id pair or more than 2 well ids
        }
      } else {
        Serial.print(ideals[i] + " are connected");        
      }
      Serial.println();
            
    }
    if (same) {
      Serial.println();
      Serial.println("Connections Good");

    }
    Serial.println();
  }
}
