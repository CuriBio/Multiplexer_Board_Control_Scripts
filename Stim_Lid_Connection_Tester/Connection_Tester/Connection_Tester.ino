
int WELL_NUMBER = 16;
int STARTING_PIN = 22;

String rows[] = {"A", "B", "C", "D"};
String cols[] = {"1", "2", "3", "4", "5", "6"};
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
    String connections[48];
    String ideals[48];

    for (int test_pin = 0; test_pin < 16; test_pin++) {
      connections[test_pin] = rows[test_pin / 6] + cols[test_pin % 6] + pol[0];
      ideals[test_pin] = rows[test_pin / 6] + cols[test_pin % 6] + pol[0] + rows[test_pin / 6] + cols[test_pin % 6] + pol[1];
      pinMode(STARTING_PIN + 2*test_pin, OUTPUT);
      digitalWrite(STARTING_PIN + 2*test_pin, 0);

      for (int well = 0; well < 16; well++) { // iterate through negative pins
        if (!digitalRead(STARTING_PIN + 2 * well) && well != test_pin) { 
          connections[test_pin].concat(rows[well / 6] + cols[well % 6] + pol[0]);          
        }
      }

      for (int well = 0; well < 16; well++) { // iterate through negative pins
        if (!digitalRead(STARTING_PIN + 2 * well + 1)) { 
          connections[test_pin].concat(rows[well / 6] + cols[well % 6] + pol[1]);          
        }
      }
      pinMode(STARTING_PIN + 2*test_pin, INPUT_PULLUP);
    }

    for (int test_pin = 0; test_pin < 16; test_pin++) {
      connections[test_pin + WELL_NUMBER] = rows[test_pin / 6] + cols[test_pin % 6] + pol[1];
      ideals[test_pin + WELL_NUMBER] = rows[test_pin / 6] + cols[test_pin % 6] + pol[1] + rows[test_pin / 6] + cols[test_pin % 6] + pol[0];
      pinMode(STARTING_PIN + 2*test_pin + 1, OUTPUT);
      digitalWrite(STARTING_PIN + 2*test_pin + 1, 0);

      for (int well = 0; well < 16; well++) { // iterate through negative pins
        if (!digitalRead(STARTING_PIN + 2 * well + 1) && well != test_pin) { 
          connections[test_pin + WELL_NUMBER].concat(rows[well / 6] + cols[well % 6] + pol[1]);          
        }
      }

      for (int well = 0; well < 16; well++) { // iterate through negative pins
        if (!digitalRead(STARTING_PIN + 2 * well)) { 
          connections[test_pin + WELL_NUMBER].concat(rows[well / 6] + cols[well % 6] + pol[0]);          
        }
      }
      pinMode(STARTING_PIN + 2*test_pin + 1, INPUT_PULLUP);

    }

    bool same = true; 
    Serial.println("Open/Inter-channel short");
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
      Serial.println()
      Serial.println("Connections Good");
    } 
  }
}
