const int pingPin1 = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin1 = 6; // Echo Pin of Ultrasonic Sensor
const int pingPin2 = 4; // Trigger Pin of Ultrasonic Sensor
const int echoPin2 = 5; // Echo Pin of Ultrasonic Sensor
const int tbridgeIn1 = 8;
const int tbridgeIn2 = 12;
const int tbridgeENA = 9;
const int _speed = 254; // between 0 and 255
const int chargintPin = 10;
int Lswitch = 2; 
int Rswitch = 13;
int flag=0;
int q = 0;
float A;
float V;
float h;
float epsilon = 2.14;
float maxEpsilon = 3.5;
boolean isNewDrone = true; // when true, we should accept a new drone (last drone must have full battery)

void setup()
{
  Serial.begin(9600); 
  pinMode(Lswitch, INPUT);
  pinMode(Rswitch, INPUT);
  pinMode(pingPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(pingPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode (A0, INPUT);
  pinMode (A1, INPUT);
  pinMode(chargintPin,OUTPUT);
  digitalWrite(chargintPin,LOW);
  pinMode(tbridgeIn1, OUTPUT);
  pinMode(tbridgeIn2, OUTPUT);
  pinMode(tbridgeENA, OUTPUT);
}

void new_loop() {
  V=0;
  if (isLeftSwitchDoorClosed() == true) {
    if (isDroneThere() == true) {
      if (isThereCurrent() == true) {
        if (isBatteryFull() == true) {
          isNewDrone = false;
          stopChargingDrone();
        }
        else {
          chargeDrone();
        }
      }
      else {
        forwardBlock();
      }
    }
    else {
      // do nothing
    }
  }
  else if (isRightSwitchDoorClosed() == true) {
    if (isDroneThere() == true) {
      if (isThereCurrent() == true) {
        if (isBatteryFull() == true) {
          stopChargingDrone();
          backwardBlock();
        }
        else {
          chargeDrone();
        }
      }
      else {
        backwardBlock();
      }
    }
    else {
      backwardBlock();
    }
  }
  else {
    if (isDroneThere() == true) {
      if (isThereCurrent() == true) {
        if (isBatteryFull() == true) {
          stopChargingDrone();
          backwardBlock();
        }
        else {
          chargeDrone();
        }
      }
      else {
        forwardBlock();
      }
    }
    else {
      backwardBlock();
    }
  }
  delay(1000);
}


void loop() {

  int goBackToOriginalState = 0;
  int doNothing = 0;
  int isDebugging = 0;
  
  if (isDebugging == 1) {
    test_rekhevim(1);
  }
  else if (doNothing == 1) {
    // in order to stop block moving while coding
  }
  else if (goBackToOriginalState == 1) {
    backwardBlock();
  }
else {
  //original_loop();
  new_loop();
  Serial.print("Percentage = "); Serial.print(getBatteryPercentage()); Serial.println("%");
}

  
}


void backwardBlock()
{
  q = 0;
  analogWrite(tbridgeENA, _speed);
  digitalWrite(tbridgeIn1, HIGH);
  digitalWrite(tbridgeIn2, LOW);
}

void forwardBlock()
{
  q++;
  if (q >= 3) {
    backwardBlock();
    delay(300);
    return;
  }

  analogWrite(tbridgeENA, _speed);
  digitalWrite(tbridgeIn1, LOW);
  digitalWrite(tbridgeIn2, HIGH);
}

void stopBlock()
{

  analogWrite(tbridgeENA, 0);
}

bool isDroneThere() 
{  
  int i=0,counter =0;
  for(;i<5;i++){
   long duration, cm, cm2;
   digitalWrite(pingPin1, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin1, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin1, LOW);
   duration = pulseIn(echoPin1, HIGH);
   cm = microsecondsToCentimeters(duration);
   delay(300);
   // ------------------
   digitalWrite(pingPin2, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin2, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin2, LOW);
   duration = pulseIn(echoPin2,HIGH);
   cm2 = microsecondsToCentimeters(duration);
   bool toReturn = (cm <= 42 && cm>=5) || (cm2 <= 42 && cm2>=5);
   if (toReturn == true && isNewDrone == false) { isNewDrone = true; }
    toReturn ? counter++ : counter--;
  }
   return counter>=3;
}

long microsecondsToCentimeters(long microseconds) 
{
   return microseconds / 29 / 2;
}


boolean isLeftSwitchDoorClosed() {
  return digitalRead(Lswitch) == LOW;
}

boolean isRightSwitchDoorClosed(){
  return digitalRead(Rswitch) == LOW;
}

boolean isThereCurrent(){
  V = analogRead (A1)/41.2;   //-------calibrate
  digitalWrite(chargintPin,LOW);
  delay(500);
  A= analogRead (A0)/183.3; 
  if( V >= epsilon) {
    return true;
  }
  return false;
}

boolean isBatteryFull() {
  digitalWrite(chargintPin,LOW);
  delay(450);
  V= analogRead (A1)/41.2;   //-------calibrate
  A= analogRead (A0)/183.3; 
  boolean toReturn = V >= 4.32;
  if (toReturn) q = 0;
  return toReturn;
}

float getBatteryPercentage() {
  V = analogRead (A1)/41.2;
  if (V >= 4.32) V= 4.32;
  if (V < epsilon) V = 0.0;
  return 100.0 * (V / 4.32);
}

void chargeDrone() {
  digitalWrite(chargintPin,HIGH);
  delay(15000); // 15 seconds
  digitalWrite(chargintPin, LOW);
}

void stopChargingDrone() {
  digitalWrite(chargintPin, LOW);
}


void test_rekhevim(int i) {
  if (i == 0) {
    while (true) {
      forwardBlock();
      delay(2500);
      stopBlock();
      delay(2500);
      backwardBlock();
      delay(2500);
    }
  }
  if (i == 1) {
    while (true) {
//      Serial.print("Left Door = ");Serial.println(isLeftSwitchDoorClosed() ? "Closed" : "Open");
//      Serial.print("Right Door = "); Serial.println(isRightSwitchDoorClosed() ? "Closed" : "Open");
      delay(1000);
    }
  }
  if (i == 2) {
    while (true) {
//      Serial.print("isDroneThere() = ");Serial.println(isDroneThere() ? "YES" : "NO");
    }
  }
}
