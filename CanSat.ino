#include <SoftwareSerial.h>

#define nsens 5 //Needed to construct the string

//Define ALL the pins
int temppin = A5;  //temperature
int trykkpin = A1; //pressure
int axelxpin = A2; //axelerometer x
int axelypin = A3; //axelerometer y
int axelzpin = A4; //axelerometer z

int rrate = 200; //delay in ms
long timecode = 0;

void setup() {
  Serial.begin(9600);
  while (millis() < 1000);
}

void loop() {
  timecode = millis();  

  int temp = analogRead(temppin);
  int trykk = analogRead(trykkpin);
  int ax = analogRead(axelxpin);
  int ay = analogRead(axelypin);
  int az = analogRead(axelzpin);  

  int sensor[nsens] = {temp, trykk, ax, ay, az};

//new fancy shit
  String sensordata = String(timecode);

  for (int i = 0; i < nsens; i++){
    sensordata.concat(", ");
    sensordata.concat(sensor[i]);
}

  Serial.println(sensordata);

  while (millis() < timecode + rrate);
}
