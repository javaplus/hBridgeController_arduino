/**
 * 
 * Expects a serial input of the format PWM #, plus Speeed value ended with a 'E' for an end marker.
 * 
 * Valide PWM values are 5 or 6.
 * Valid Speeds are 0 (stop) to 255 (full power)
 * 
 * Example:
 * 
 * "6100E" - would send the speed of 100 to PWM output pin 6
 * "60E" - would send a 0 speed(stop) to output pin 6.
 * 
 * 
 */

int RPWM = 5;
int LPWM = 6;
int L_EN = 7;
int R_EN = 8;

int speed = 0;
int last_speed = 0;

int CurrentPWMChannel = 5;
int LastPWMChannel = 5;
int ledPin = 12;

const int SMOOTHING_FACTOR = 20;

const byte numChars = 32;
char inputBuffer[numChars];

boolean newData = false;


void setup() {
  // put your setup code here, to run once:
  pinMode(10, INPUT); // Set our input pins as such
  for (int i = 5; i < 9; i++) {
    pinMode(i, OUTPUT);
  }
  for (int i = 5; i < 9; i++) {
    digitalWrite(i, LOW);
  }
  // for flashing LED
  pinMode(ledPin, OUTPUT);

  digitalWrite(R_EN, HIGH); // enable forward
  digitalWrite(L_EN, HIGH); // enable reverse
  delay(1000);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  recvWithEndMarker();
  Serial.println(speed);
  Serial.print("PWM"); 
  Serial.println(CurrentPWMChannel);
  //flash(speed);
  last_speed = goForward(CurrentPWMChannel, speed, last_speed);

  LastPWMChannel = CurrentPWMChannel;
  //analogWrite(CurrentPWMChannel, speed);
  delay(50);
}



void recvWithEndMarker() {
  int ndx = 0;
  char endMarker = 'E';
  char rc;

  // if (Serial.available() > 0) {
  while (Serial.available() >0) {
    rc = Serial.read();

    if (rc != endMarker) {
      Serial.println("Not an end marker");
      Serial.print("ndx=");
      Serial.println(ndx);
      Serial.print("rc=");
      Serial.println(rc);
      inputBuffer[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        Serial.println("Hit max chars!!!!!!!!!!!!!!!!!!!");
        ndx = numChars - 1;
      }
    }
    else {
      Serial.println("Hit END Marker");
      inputBuffer[ndx] = '\0'; // terminate the string
      ndx = 0;
      //newData = true;
      CurrentPWMChannel = inputBuffer[0] - '0';
      Serial.print("nowPWM");
      Serial.println(CurrentPWMChannel);
      speed = atoi(&inputBuffer[1]);
      Serial.print("nowSpeed");
      Serial.println(speed);
    }
  }
}

int goForward(int direction, int forward_speed, int last_speed) {

  int theSpeed = smoothSpeed(direction, forward_speed, last_speed);
  analogWrite(direction, theSpeed);
  return theSpeed;
}

int smoothSpeed(int direction, int wanted_speed, int current_speed){
  int speedDiff = wanted_speed - current_speed;
  if(speedDiff > SMOOTHING_FACTOR){ // speedingUp
    //Serial.print("smoothing:");
    //Serial.println(current_speed + SMOOTHING_FACTOR);
    return current_speed + SMOOTHING_FACTOR;
  }else if(speedDiff < -SMOOTHING_FACTOR){ // slowing down
    //Serial.print("smoothing down:");
    //Serial.println(current_speed - SMOOTHING_FACTOR);
    return current_speed - SMOOTHING_FACTOR;
  }
  return wanted_speed;
  
}

void flash(int n)
{
  for (int i = 0; i < n; i++)
  {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}

/**
    current_speed = last recorded speed what we sent as PWM value
    requested_speed = what we are asking to set the speed to
    new_direction = what direction they are asking to go in
    last_direction
*/
int increaseSpeed(int current_speed, int requested_speed, int new_direction, int last_direction) {
  // direction 1 is forward, 0 is stopped, and -1 is backward
  // check current direction
  if (new_direction == last_direction) { // same direction just increase speed
    //Serial.print("just changing speed");
    //Serial.println(requested_speed);

    return requested_speed;
  } else {
    if (requested_speed == 0) { // we want to stop
      //Serial.println("Stopping");

      comeToStop(current_speed, last_direction);

      //delay(1000);
      return 0;
      //Serial.println("came to stop");
    } else {

      //Serial.println("swiching directions");
      //delay(1000);
      // we are switching directions... so slow down then go the other direction
      comeToStop(current_speed, last_direction);
      // now get up to speed the other direction
      delay(25);
      /*for(int i=0; i < (current_speed/2); i++){
            Serial.print("looping to speed up: ");
            Serial.println(i);
            analogWrite(new_direction, i);
            delay(50);
        }*/
      return (0);

    }

  }
}

int comeToStop(int current_speed, int last_direction) {
  // loop and slowly stop
  for (int i = (current_speed / 4); i >= 0; i--) {
    //Serial.print("looping to stop: ");
    //Serial.println(i);
    analogWrite(last_direction, i);
    delay(50);
  }
}

