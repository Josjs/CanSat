#include <SoftwareSerial.h>

#define nsens 5 //Needed to construct the string

//Define ALL the pins
int temppin = A5;  //temperature
int trykkpin = A1; //pressure
int axelxpin = A2; //axelerometer x
int axelypin = A3; //axelerometer y
int axelzpin = A4; //axelerometer z

//Make an extra serial shiz for OpenLog, in case we need it
//Edit: We don't
SoftwareSerial OpenLog(1, 0);

int rrate = 200; //delay in ms
long timecode = 0;

void setup() {
OpenLog.begin(9600);
Serial.begin(9600);
//delay(1000); //Saw a guy on the internet do this
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

//failsafe method
/*Serial.print(temp);
Serial.print(", ");
Serial.print(trykk);
Serial.print(", ");
Serial.print(ax);
Serial.print(", ");
Serial.print(ay);
Serial.print(", ");
Serial.print(az);
Serial.print("\r\n");
*/

//new fancy shit
//String sensordata = String(temp) + String(" ") + String(trykk) + String(" ") + String(ax) + String(" ") + String(ay) + String(" ") + String(az);
String sensordata = String(timecode);

for (int i = 0; i < nsens; i++){
  sensordata.concat(", ");
  sensordata.concat(sensor[i]);
}


Serial.println(sensordata);

while (millis() < timecode + rrate);
//delay(rrate);
}

/*int sensordata(){
  int temp = analogRead(temppin);
  int trykk = analogRead(trykkpin);
  int ax = analogRead(axelxpin);
  int ay = analogRead(axelypin);
  int az = analogRead(axelzpin);
  //char buff[30];
  //sprintf(buff, "%4d %4d %4d", temppin, trykkpin, axelxpin);
  
  return(temp, trykk, ax, ay, az);
}
*/
