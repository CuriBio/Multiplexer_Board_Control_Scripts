
int WELL_NUMBER = 24;
int STARTING_PIN = 2;

String rows[] = {"A", "B", "C", "D"};
String cols[] = {"6", "5", "4", "3", "2", "1"};
String pol[] = {"+", "-"};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int well = 0; well < WELL_NUMBER * 2; well++) {
    pinMode(STARTING_PIN + well, INPUT_PULLUP);
  }
}

void loop() {
  if (Serial.available() > 0) {
    Serial.read();
    String connections[WELL_NUMBER*2];
    String ideals[WELL_NUMBER*2];

    for (int test_pin = 0; test_pin < WELL_NUMBER; test_pin++) {
      connections[test_pin] = rows[test_pin / 6] + cols[test_pin % 6] + pol[0];
      ideals[test_pin] = rows[test_pin / 6] + cols[test_pin % 6] + pol[0] + rows[test_pin / 6] + cols[test_pin % 6] + pol[1];
      pinMode(STARTING_PIN + 2*test_pin, OUTPUT);
      digitalWrite(STARTING_PIN + 2*test_pin, 0);

      for (int well = 0; well < WELL_NUMBER; well++) { // iterate through negative pins
        if (!digitalRead(STARTING_PIN + 2 * well) && well != test_pin) { 
          connections[test_pin].concat(rows[well / 6] + cols[well % 6] + pol[0]);          
        }
      }

      for (int well = 0; well < WELL_NUMBER; well++) { // iterate through negative pins
        if (!digitalRead(STARTING_PIN + 2 * well + 1)) { 
          connections[test_pin].concat(rows[well / 6] + cols[well % 6] + pol[1]);          
        }
      }
      pinMode(STARTING_PIN + 2*test_pin, INPUT_PULLUP);
    }

    for (int test_pin = 0; test_pin < WELL_NUMBER; test_pin++) {
      connections[test_pin + WELL_NUMBER] = rows[test_pin / 6] + cols[test_pin % 6] + pol[1];
      ideals[test_pin + WELL_NUMBER] = rows[test_pin / 6] + cols[test_pin % 6] + pol[1] + rows[test_pin / 6] + cols[test_pin % 6] + pol[0];
      pinMode(STARTING_PIN + 2*test_pin + 1, OUTPUT);
      digitalWrite(STARTING_PIN + 2*test_pin + 1, 0);

      for (int well = 0; well < WELL_NUMBER; well++) { // iterate through negative pins
        if (!digitalRead(STARTING_PIN + 2 * well + 1) && well != test_pin) { 
          connections[test_pin + WELL_NUMBER].concat(rows[well / 6] + cols[well % 6] + pol[1]);          
        }
      }

      for (int well = 0; well < WELL_NUMBER; well++) { // iterate through negative pins
        if (!digitalRead(STARTING_PIN + 2 * well)) { 
          connections[test_pin + WELL_NUMBER].concat(rows[well / 6] + cols[well % 6] + pol[0]);          
        }
      }
      pinMode(STARTING_PIN + 2*test_pin + 1, INPUT_PULLUP);

    }

    bool same = true; 
    Serial.println("Begin Test");
    Serial.println();
    for (int i = 0; i < 2 * WELL_NUMBER; i++) {
      // Serial.println(ideals[i]);
      // Serial.println(connections[i]);

      if (!ideals[i].equals(connections[i])){
        same = false; 
        Serial.print(connections[i]);
        if (connections[i].length() == 3) {
          Serial.print( " is open");
        } else {
          Serial.print(" are shorted");
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
