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

int speed = 0;
int last_speed = 0; 

int MAX_REMOTE_VALUE = 1750;
int LOW_REMOTE_VALUE = 1250;
int MID_REMOTE_VALUE = 1500;
int CurrentPWMChannel = 5;
int LastPWMChannel = 5;

int pwm_value=MID_REMOTE_VALUE; // Here's where we'll keep our channel values
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
  digitalWrite(R_EN, HIGH); // enable forward
  digitalWrite(L_EN, HIGH); // enable reverse
  delay(1000);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  speed =0;
  pwm_value = pulseIn(10, HIGH, 25000); // Read the pulse width of
  Serial.print("pwm value:");
  Serial.println(pwm_value);
  if (pwm_value > MID_REMOTE_VALUE + 20) {
    speed = (pwm_value - MID_REMOTE_VALUE) + 25;
    if (speed > 255){
      speed = 250;
    }
    Serial.print("Speed Down:");
    Serial.println(speed);
    CurrentPWMChannel = RPWM;
 
  }

  if (pwm_value < MID_REMOTE_VALUE - 20) {
    speed = (MID_REMOTE_VALUE - pwm_value) + 25;
    if (speed > 255){
      speed = 250;
    }
    //Serial.print("Speed Up:");
    //Serial.println(speed);
    CurrentPWMChannel = LPWM;
  }


  speed = increaseSpeed(last_speed, speed, CurrentPWMChannel, LastPWMChannel);

  last_speed = speed;
  LastPWMChannel = CurrentPWMChannel;
  //Serial.print("last_speed:");
  //Serial.println(last_speed);

  //Serial.print("LastPWMChannel:");
  //Serial.println(LastPWMChannel);
  analogWrite(CurrentPWMChannel, speed);
  delay(50);
}


/**
 *  current_speed = last recorded speed what we sent as PWM value
 *  requested_speed = what we are asking to set the speed to
 *  new_direction = what direction they are asking to go in
 *  last_direction
 */
int increaseSpeed(int current_speed, int requested_speed, int new_direction, int last_direction){
  // direction 1 is forward, 0 is stopped, and -1 is backward
  // check current direction 
  if(new_direction == last_direction){ // same direction just increase speed
    Serial.print("just changing speed");
    Serial.println(requested_speed);
  
    return requested_speed;  
  }else{
    if(requested_speed == 0){ // we want to stop
      Serial.println("Stopping");
  
      comeToStop(current_speed, last_direction);

      //delay(1000);
      return 0;
      Serial.println("came to stop");
    }else{

      Serial.println("swiching directions");
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

int comeToStop(int current_speed, int last_direction){
      // loop and slowly stop
      for(int i=(current_speed/4); i >= 0; i--){
            Serial.print("looping to stop: ");
            Serial.println(i);
            analogWrite(last_direction, i);  
            delay(50);
      }
}

