// the setup function runs once when you press reset or power the board
int PIN_OUT = 12;
void setup() {
  // initialize digital pin PIN_OUT as an output.
  pinMode(PIN_OUT, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(PIN_OUT, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(PIN_OUT, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
