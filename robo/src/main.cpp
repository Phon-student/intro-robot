#include <Arduino.h>
#include <pinout.h>
#include <config.h>

#define isINVERTED_X false
#define isINVERTED_Y false
#define isINVERTED_Z false

#define limitAngle_X 180
#define limitAngle_Y 180
#define limitAngle_Z 180

#define delayTime 500

char inputString[100];

struct Motor{
  int stepPin;
  int dirPin;
  int enablePin;
  int switchPin;
  bool isINVERTED;
  float limitAngle;
  float angle = 0;
};

Motor motor[3];

void limitCheck(int i){
  if(motor[i].angle > motor[i].limitAngle){
    motor[i].angle = motor[i].limitAngle;
  }
  if(motor[i].angle < 0){
    motor[i].angle = 0;
  }
}

void setAngle(int motorNo, float Finalangle){
  float offset = Finalangle - motor[motorNo].angle;
  if(offset != 0){
    if(offset > 0){
      digitalWrite(motor[motorNo].dirPin, HIGH);
    }
    else{
      digitalWrite(motor[motorNo].dirPin, LOW);
    }
    for(int i = 0; i<abs(offset); i++){
      digitalWrite(motor[motorNo].stepPin, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(motor[motorNo].stepPin, LOW);
      delayMicroseconds(delayTime);
    }
    motor[motorNo].angle = Finalangle;
  }
  for(int  i=0 ; i<offset ; i++){
    digitalWrite(motor[motorNo].stepPin, HIGH);
    digitalWrite(motor[motorNo].stepPin, LOW);
    delayMicroseconds(delayTime);
    if(offset > 0){
      motor[motorNo].angle++;
    }
    else{
      motor[motorNo].angle--;
    }
  }
  
}



void DH_matrix(float theta, float d, float a, float alpha, float T[4][4]){
  T[0][0] = cos(theta);
  T[0][1] = -sin(theta)*cos(alpha);
  T[0][2] = sin(theta)*sin(alpha);
  T[0][3] = a*cos(theta);

  T[1][0] = sin(theta);
  T[1][1] = cos(theta)*cos(alpha);
  T[1][2] = -cos(theta)*sin(alpha);
  T[1][3] = a*sin(theta);

  T[2][0] = 0;
  T[2][1] = sin(alpha);
  T[2][2] = cos(alpha);
  T[2][3] = d;

  T[3][0] = 0;
  T[3][1] = 0;
  T[3][2] = 0;
  T[3][3] = 1;
}


void Homeset(){
  for(int i = 0; i<3; i++){
    while(digitalRead(motor[i].switchPin) == 1){
      digitalWrite(motor[i].dirPin, HIGH);
      digitalWrite(motor[i].stepPin, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(motor[i].stepPin, LOW);
      delayMicroseconds(delayTime);
    }
    motor[i].angle = 0;
  }

}


void setup() {
  Serial.begin(9600);
  Serial.println("Start");
  motor[0].stepPin = X_STEP_PIN;
  motor[0].dirPin = X_DIR_PIN;
  motor[0].enablePin = X_ENABLE_PIN;
  motor[0].switchPin = X_MIN_PIN;
  motor[0].isINVERTED = isINVERTED_X;
  motor[0].limitAngle = limitAngle_X;

  motor[1].stepPin = Y_STEP_PIN;
  motor[1].dirPin = Y_DIR_PIN;
  motor[1].enablePin = Y_ENABLE_PIN;
  motor[1].switchPin = Y_MIN_PIN;
  motor[1].isINVERTED = isINVERTED_Y;
  motor[1].limitAngle = limitAngle_Y;

  motor[2].stepPin = Z_STEP_PIN;
  motor[2].dirPin = Z_DIR_PIN;
  motor[2].enablePin = Z_ENABLE_PIN;
  motor[2].switchPin = Z_MIN_PIN;
  motor[2].isINVERTED = isINVERTED_Z;
  motor[2].limitAngle = limitAngle_Z;

  for(int i = 0; i<3; i++){
    pinMode(motor[i].stepPin, OUTPUT);
    pinMode(motor[i].dirPin, OUTPUT);
    pinMode(motor[i].enablePin, OUTPUT);
    pinMode(motor[i].switchPin, INPUT);
    digitalWrite(motor[i].enablePin, LOW);
  }

  Serial.println("system ready");
  Homeset();
  
}

void loop(){

}