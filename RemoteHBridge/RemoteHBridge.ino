/*........................
  BTS7960 Motor Driver Test
  Written By : Mohannad Rawashdeh
  Code for :
  https://www.instructables.com/member/Mohannad+Rawashdeh/
*/
int RPWM = 5;
int LPWM = 6;
// timer 0
int L_EN = 7;
int R_EN = 8;

// 1130 High up, 1500 mid, 1850 all down
int pwm_value; // Here's where we'll keep our channel values
int speed = 0;
int MAX_REMOTE_VALUE = 1750;
int LOW_REMOTE_VALUE = 1280;
int MID_REMOTE_VALUE = 1500;


void setup() {

  // put your setup code here, to run once:
  pinMode(10, INPUT); // Set our input pins as such
  for (int i = 5; i < 9; i++) {
    pinMode(i, OUTPUT);
  }
  for (int i = 5; i < 9; i++) {
    digitalWrite(i, LOW);
  }
  Serial.println("EN High");
  digitalWrite(R_EN, HIGH);
  digitalWrite(L_EN, HIGH);
  delay(1000);
  Serial.begin(9600);
}



void loop() {
  // put your main code here, to run repeatedly:

 pwm_value = pulseIn(10, HIGH, 25000); // Read the pulse width of 
 Serial.print("pwm value:");
 Serial.println(pwm_value);
 if(pwm_value > MID_REMOTE_VALUE + 20){
  speed = pwm_value - MID_REMOTE_VALUE; 
  if(speed > 255)
    speed = 255;
  Serial.print("Speed Down:");
  Serial.println(speed); 
  analogWrite(LPWM, speed);
 }

 if(pwm_value < MID_REMOTE_VALUE -20){
  speed = MID_REMOTE_VALUE - pwm_value;
  if(speed > 255)
    speed = 255;
  Serial.print("Speed Up:");
  Serial.println(speed);  
  analogWrite(RPWM, speed);
 }

 delay(50);
 
  
}
