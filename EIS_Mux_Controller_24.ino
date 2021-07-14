
int outputs = 24; // MUST match number of connected channels, otherwise some krazy shite starts happening
int startPin = 22; // Corresponds to channel 1 on PCB
int channel = startPin; // this is incremented to switch between channels
int triggerpin = 47; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(triggerpin, INPUT);
  
  for (int i = startPin; i < outputs+startPin; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH); 
    delay(100);
    digitalWrite(i, LOW);
  }

  delay(2000); 
}

// BE SURE TO LET MUX CIRCUIT FINISH SETUP BEFORE STARTING GAMRY SEQUENCE

void loop() {
    // wait for input from EIS before activating channel
    if (digitalRead(triggerpin) == HIGH) {
      digitalWrite(channel, HIGH); //open relay at channel
      while (digitalRead(triggerpin) == HIGH) { // sequence will make the trigger pin remain high during data acquisition
          delay(1); // time for EIS
      }
      
    digitalWrite(channel, LOW); //close relay at channel
      channel++; // increment channel
    }
    
    if (channel == startPin + outputs) {
      channel = startPin; // reset after going through all channels
    }
  }

      
