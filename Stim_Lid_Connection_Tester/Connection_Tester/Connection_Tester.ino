int WELL_NUMBER = 32;
int STARTING_PIN = 22;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int well = 0; well < WELL_NUMBER; well++) {
    pinMode(STARTING_PIN + well, INPUT_PULLUP);
  }
}

void loop() {
  if (Serial.available() > 0) {
    Serial.read();
    for (int test_pin = 0; test_pin < 16; test_pin++) {
      Serial.println(STARTING_PIN + 2*test_pin);
      pinMode(STARTING_PIN + 2*test_pin, OUTPUT);
      digitalWrite(STARTING_PIN + 2*test_pin, 0);
      for (int well = 0; well < 16; well++) {
        // Serial.print(STARTING_PIN + 2*well + 1);
        Serial.print(digitalRead(STARTING_PIN + 2 * well + 1));
      }
      Serial.println();
      pinMode(STARTING_PIN + 2*test_pin, INPUT_PULLUP);
    }
  }
}
