#include <Arduino.h>
#include "config.h"
#include "pinout.h"

#define isInvertX true
#define isInvertY false
#define isInvertZ true

#define limitAngleX 180
#define limitAngleY 180
#define limitAngleZ 180

#define delayTime 1000

char recieve;

struct Motor {
  int stepPin;
  int dirPin;
  int enPin;
  int switchPin;
  int steppos =0;
  bool isInvert;
  float anglePerStep = 0.50625; // 0.50625 degree per step (1.8 degree per step) (1/16 micro step)(2:9 gear ratio)
  float angle = steppos * 1.975; // 1/0.50625 step per degree
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
// void move(int motorNo, float Targetsteppos){
//   while (Targetsteppos < motor[motorNo].steppos) {  
//     digitalWrite(motor[motorNo].dirPin, !motor[motorNo].isInvert);
//     digitalWrite(motor[motorNo].stepPin, HIGH);
//     digitalWrite(motor[motorNo].stepPin, LOW);
//     motor[motorNo].steppos--;
//     delayMicroseconds(delayTime);
//     Serial.println(motor[motorNo].steppos);
//   }
  
//   while (Targetsteppos > motor[motorNo].steppos) {    
//     digitalWrite(motor[motorNo].dirPin, !motor[motorNo].isInvert);
//     digitalWrite(motor[motorNo].stepPin, HIGH);
//     digitalWrite(motor[motorNo].stepPin, LOW);;
//     digitalWrite(motor[motorNo].stepPin, LOW);
//     motor[motorNo].steppos++;
//     delayMicroseconds(delayTime);
//     Serial.println(motor[motorNo].steppos);
//   }
// }

void move(int motorNo, float targetAngle){
  float Targetsteppos = targetAngle / motor[motorNo].anglePerStep;
  while (Targetsteppos < motor[motorNo].steppos) {  
    digitalWrite(motor[motorNo].dirPin, !motor[motorNo].isInvert);
    digitalWrite(motor[motorNo].stepPin, HIGH);
    digitalWrite(motor[motorNo].stepPin, LOW);
    motor[motorNo].steppos--;
    delayMicroseconds(delayTime);
    Serial.println(motor[motorNo].steppos);
  }
  while (Targetsteppos > motor[motorNo].steppos) {    
    digitalWrite(motor[motorNo].dirPin, motor[motorNo].isInvert);
    digitalWrite(motor[motorNo].stepPin, HIGH);
    digitalWrite(motor[motorNo].stepPin, LOW);;
    digitalWrite(motor[motorNo].stepPin, LOW);
    motor[motorNo].steppos++;
    delayMicroseconds(delayTime);
    Serial.println(motor[motorNo].steppos);
  }
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
  setAngle(0, finalX);
  setAngle(1, finalY);
  setAngle(2, finalZ);
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
  Serial.println("System Ready");

}

void loop() {
  if(Serial.available()){
    recieve = Serial.read();
    if(recieve == 'x'){
      move(0, 45);
      delay(1000);
      move(1, 45);
      delay(1000);
      move(2, 45);
        //print all angle
      Serial.print(motor[0].steppos);
      Serial.print("\t");
      Serial.print(motor[0].steppos);
      Serial.print("\t");
      Serial.println(motor[0].steppos);
      Serial.println("-----------------------------");
    }
    delay(100);
  }
}
