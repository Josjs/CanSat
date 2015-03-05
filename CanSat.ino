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
bool raw = 1;

void setup() {
  Serial.begin(9600);
  while (millis() < 1000);
}

void loop() {
  timecode = millis();  

  if (raw = true){
    int temp = analogRead(temppin);
    int trykk = analogRead(trykkpin);
    int ax = analogRead(axelxpin);
    int ay = analogRead(axelypin);
    int az = analogRead(axelzpin);  
    int sensor[nsens] = {temp, trykk, ax, ay, az};
  } else {
      float temp = analogRead(temppin);
      float trykk = analogRead(trykkpin);
      float ax = analogRead(axelxpin);
      float ay = analogRead(axelypin);
      float az = analogRead(axelzpin);
      float sensorf[nsens] = {temp, trykk, ax, ay, az};
  }  
//new fancy shit
  String sensordata = String(timecode);

  for (int i = 0; i < nsens; i++){
    sensordata.concat(", ");
    if (raw = true){
      sensordata.concat(sensor[i]);
    } else{
        sensordata.concat(sensorf[i]);
      }

    } 
}

  Serial.println(sensordata);

  while (millis() < timecode + rrate);
}
