#include <Arduino.h>
#include "config.h"
#include "pinout.h"
#include "servo.h"


#define isInvertX true
#define isInvertY false
#define isInvertZ true

#define limitAngleX 180
#define limitAngleY 180
#define limitAngleZ 180

#define delayTime 200

Servo myservo;
int pos;




char recieve;

struct Motor {
  int stepPin;
  int dirPin; 
  int enPin;
  int switchPin;
  bool isInvert;
  float angle = 0;
  float limitAngle;
};

Motor motor[3];

void limitor(int motorNo) {
  if (motor[motorNo].angle > motor[motorNo].limitAngle) {
    Serial.println("Rotate Over LIMIT!!!");
    Serial.println("System Stop");
    while (1);
  }
  if (motor[motorNo].angle < 0) {
    Serial.println("Rotate Over LIMIT!!!");
    Serial.println("System Stop");
    while (1);
  }
}

void homeX(){
  int i = 0;
  digitalWrite(motor[i].enPin, LOW);
  if (motor[i].isInvert)digitalWrite(motor[i].dirPin, LOW);
  else digitalWrite(motor[i].dirPin, HIGH);

  while (!digitalRead(motor[i].switchPin)) {
    digitalWrite(motor[i].stepPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(motor[i].stepPin, LOW);
    delayMicroseconds(delayTime);
  }
  motor[i].angle = 0;
  Serial.println("motor " + String(i) + "homed");
}

void homeY(){
  int i = 1;
  digitalWrite(motor[i].enPin, LOW);
  if (motor[i].isInvert)digitalWrite(motor[i].dirPin, LOW);
  else digitalWrite(motor[i].dirPin, HIGH);

  while (!digitalRead(motor[i].switchPin)) {
    digitalWrite(motor[i].stepPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(motor[i].stepPin, LOW);
    delayMicroseconds(delayTime);
  }
  motor[i].angle = 0;
  Serial.println("motor " + String(i) + "homed");
}

void homeZ(){
  int i = 2;
  digitalWrite(motor[i].enPin, LOW);
  if (motor[i].isInvert)digitalWrite(motor[i].dirPin, LOW);
  else digitalWrite(motor[i].dirPin, HIGH);

  while (!digitalRead(motor[i].switchPin)) {
    digitalWrite(motor[i].stepPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(motor[i].stepPin, LOW);
    delayMicroseconds(delayTime);
  }
  motor[i].angle = 0;
  Serial.println("motor " + String(i) + "homed");
}

void setHome() {
  homeX();
  homeY();
  homeZ();
}


void setAngle(int motorNo, float finalAngle) {
  digitalWrite(motor[0].enPin, LOW);
  digitalWrite(motor[1].enPin, LOW);
  digitalWrite(motor[2].enPin, LOW);
  float offSet = finalAngle - motor[motorNo].angle;
  int offSetM;
  if (offSet < 0 ) {
    if (motor[motorNo].isInvert)digitalWrite(motor[motorNo].dirPin, LOW);
    else digitalWrite(motor[motorNo].dirPin, HIGH);
    Serial.println("Negative Offset");
    offSetM = -offSet;
  } else {
    if (motor[motorNo].isInvert)digitalWrite(motor[motorNo].dirPin, HIGH);
    else digitalWrite(motor[motorNo].dirPin, LOW);
    Serial.println("Positive Offset");
    offSetM = offSet;
  }
  for (int i = 0; i < (offSetM / 0.025); i++) {
    digitalWrite(motor[motorNo].stepPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(motor[motorNo].stepPin, LOW);
    delayMicroseconds(delayTime);
    if (offSet < 0 ) {
      motor[motorNo].angle -= 0.025;
//      Serial.println(motor[motorNo].angle);
      if (motor[motorNo].angle < 0.025) {
        Serial.println("Rotate Over LIMIT!!!");
        Serial.println("System Stop");
        break;
      }
    }
    else {
      motor[motorNo].angle += 0.025;
      Serial.println(motor[motorNo].angle);
      if (motor[motorNo].angle > motor[motorNo].limitAngle-0.025) {
        Serial.println("Rotate Over LIMIT!!!");
        Serial.println("System Stop");
        break;
      }
    }
  }
  digitalWrite(motor[0].enPin, HIGH);
  digitalWrite(motor[1].enPin, HIGH);
  digitalWrite(motor[2].enPin, HIGH);
}

void setAllAngle(float finalX, float finalY, float finalZ){
  setAngle(2, finalZ);
  setAngle(1, finalY);
  setAngle(0, finalX);
}

void testSwitch(){
    Serial.print(digitalRead(motor[0].switchPin));
  Serial.print("\t");
  Serial.print(digitalRead(motor[1].switchPin));
  Serial.print("\t");
  Serial.println(digitalRead(motor[2].switchPin));
  Serial.println("-----------------------------");
  delay(500);
}


void setup() {
  
  motor[0].stepPin = X_STEP_PIN;
  motor[0].dirPin = X_DIR_PIN;
  motor[0].enPin = X_ENABLE_PIN;
  motor[0].switchPin = X_MIN_PIN;
  motor[0].isInvert = isInvertX;
  motor[0].limitAngle = limitAngleX;

  motor[1].stepPin = Y_STEP_PIN;
  motor[1].dirPin = Y_DIR_PIN;
  motor[1].enPin = Y_ENABLE_PIN;
  motor[1].switchPin = Y_MIN_PIN;
  motor[1].isInvert = isInvertY;
  motor[1].limitAngle = limitAngleY;

  motor[2].stepPin = Z_STEP_PIN;
  motor[2].dirPin = Z_DIR_PIN;
  motor[2].enPin = Z_ENABLE_PIN;
  motor[2].switchPin = Z_MIN_PIN;
  motor[2].isInvert = isInvertZ;
  motor[2].limitAngle = limitAngleZ;
  // put your setup code here, to run once:

  myservo.attach(11);

  Serial.begin(115200);
  for (int i = 0; i < 3; i++) {
    pinMode(motor[i].stepPin, OUTPUT);
    pinMode(motor[i].dirPin, OUTPUT);
    pinMode(motor[i].enPin, OUTPUT);
    pinMode(motor[i].switchPin, INPUT);
  }
  Serial.print("System Started");
  for (int i = 0 ; i < 5; i++) {
    Serial.print('.');
    delay(500);
  }
  Serial.println("Setting Home...");
  setHome();

  // Serial.println("TESTING SERVO");
  // for (pos = 0; pos <= 180; pos += 1) {   // sweeps servo from 0 to 180 degrees
  //   myservo.write(pos);
  //   delay(15);
  // }
  Serial.println("System Ready");

}

void loop() {
  if(Serial.available()){
    recieve = Serial.read();
    if(recieve == 'z'){
      setAngle(2, 90);
    }
    else if(recieve == 'a'){
      setAllAngle(60, 80, 100);
      myservo.write(40);
      setAngle(1, 90);
      myservo.write(20);
      setAngle(0, 70);
      delay(3000);
      setAngle(1, 100);
      myservo.write(0);
      setAngle(0, 80);
      delay(3000);
      setAllAngle(50, 60, 100);
      for (pos = 0; pos <= 90; pos += 1) {   // sweeps servo from 0 to 180 degrees
        myservo.write(pos);
        delay(15);
      }
    }
    else if(recieve == 'e'){
      setAngle(0, 15);
      setAngle(1, 30);
      setAngle(2, 0);
      setAngle(0, 25);
      setAngle(1, 70);
    }
    else if(recieve == 'h'){
      setHome();
    }
    // else if(recieve == 't'){
    //   testSwitch();
    // }
    // else if(recieve == 's'){
    //   Serial.print("X: ");
    //   Serial.print(motor[0].angle);
    //   Serial.print("\tY: ");
    //   Serial.print(motor[1].angle);
    //   Serial.print("\tZ: ");
    //   Serial.println(motor[2].angle);
    // }
    // else if(recieve == 'q'){
    //   Serial.println("System Stop");
    //   while(1);
    // }
    else if(recieve == 'f'){
      pos+=1;
      myservo.write(pos);
      Serial.println(pos);
    }
    else if(recieve == 'b'){
      pos-=1;
      myservo.write(pos);
      Serial.println(pos);
    }
  }
  
  

}