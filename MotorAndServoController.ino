/*........................
  BTS7960 Motor Driver Test
  Written By : Mohannad Rawashdeh
  Code for :
  https://www.instructables.com/member/Mohannad+Rawashdeh/
*/
#include <Servo.h>

unsigned long lastMoveDownTime = 0;
unsigned long lastMoveUpTime = 0;
unsigned long timeToWaitUp = 1500;
unsigned long timeToWaitDown = 4500;

int fullUpServoPosition = 0;
int fullDownServoPosition = 180;
boolean isSpinning = false;
boolean isUp = false;
boolean isDown = false;

Servo myservo;  // create servo object to control a servo

int potpin = 5;  // analog pin used to connect the potentiometer
int potValueServo;    // variable to read the value from the pot for servo control



int RPWM = 5;
int LPWM = 6;
// timer 0
int L_EN = 7;
int R_EN = 8;
int potValueForSpinner = 0;
int pwmOutput = 0;


void setup() {
  // put your setup code here, to run once:

  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  
  for (int i = 5; i < 9; i++) {
    pinMode(i, OUTPUT);
  }
  for (int i = 5; i < 9; i++) {
    digitalWrite(i, LOW);
  }
  delay(1000);
  Serial.begin(9600);
}



void loop() {

  Serial.println("EN High");
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);

  // --- Start spinning block ----
  potValueForSpinner = analogRead(A0); // Read potentiometer value
  pwmOutput = map(potValueForSpinner, 0, 1023, 0 , 255); // Map the potentiometer value from 0 to 255
  // put your main code here, to run repeatedly:
  int potValueForSpinner = analogRead(A0); // Read potentiometer value
  Serial.println("potValueForSpinner");
  Serial.println(potValueForSpinner);
  
  analogWrite(RPWM, pwmOutput); // Send PWM signal to L298N Enable pin
// ----- End Spinning block ------


  if(!isDown && millis()- lastMoveUpTime > timeToWaitUp){

    // We've been up for long enough move down

    myservo.write(fullDownServoPosition);
    delay(200); // let it move down
    lastMoveDownTime = millis();
    isDown=true;
    isUp=false;
  }

  if(!isUp && millis()- lastMoveDownTime > timeToWaitDown){

    // We've been Down for long enough move up

    myservo.write(fullUpServoPosition);
    delay(200); // let it move down
    lastMoveUpTime = millis();
    isUp=true;
    isDown=false;
    
  }

  // Set time for Time up
  potValueServo = analogRead(potpin);            // reads the value of the potentiometer (value between 0 and 1023)
  
  Serial.println("Servo Pot pin");
  
  Serial.println(potValueServo);
  potValueServo = map(potValueServo, 0, 1023, 1000, 10000);     // scale it to use it with the servo (value between 1 second to 10 seconds)
  timeToWaitUp = potValueServo;

  Serial.println("Time to Wait up");
  
  Serial.println(timeToWaitUp);
}
