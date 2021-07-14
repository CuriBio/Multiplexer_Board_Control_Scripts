
int outputs = 4; // MUST match number of connected channels, otherwise some krazy shite starts happening
int startPin = 22; // Corresponds to channel 1 on PCB
int phaseDur = 2; //ms
int openingTime = 1; //ms
int stimTime = 2*phaseDur; 
int triggerpin = 47; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(triggerpin, OUTPUT);
  
  for (int i = startPin; i < outputs+startPin; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH); 
    delay(100);
    digitalWrite(i, LOW);
  }

  delay(2000); 
}

void loop() {
  // Start cycling through relays at start of train
  // start loop with starting relay open
      delay(100);
//      
      for (int i = startPin; i < outputs+startPin; i++) {
        digitalWrite(i, HIGH); //open relay at i
        delay(openingTime); // time for  relay to open
        digitalWrite(triggerpin, HIGH); // trigger stim  
        delay(stimTime); // time for stimulus waveform 
        digitalWrite(i, LOW); //close relay at i
        digitalWrite(triggerpin, LOW); 
        delay(openingTime); // time for relay to close
      }
      
}
