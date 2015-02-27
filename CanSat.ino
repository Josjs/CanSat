#include <SoftwareSerial.h>

//Define ALL the pins
int temppin = 1;
int trykkpin = 2;
int axelxpin = 3;
int axelypin = 4;
int axelzpin = 5;

//Make an extra serial shiz for OpenLog, in case we need it
SoftwareSerial OpenLog(3, 2);

int rrate = 555; //delay in m

void setup() {
OpenLog.begin(9600);
Serial.begin(9600);
delay(1000); //Saw a guy on the internet do this
}

void loop() {
int temp = analogRead(temppin);
int trykk = analogRead(trykkpin);
int ax = analogRead(axelxpin);
int ay = analogRead(axelypin);
int az = analogRead(axelzpin);  
//OpenLog.println(temp, trykk, ax, ay, az);
OpenLog.print(temp);
Serial.print(", ");
Serial.print(trykk);
Serial.print(", ");
Serial.print(ax);
Serial.print(", ");
Serial.print(ay);
Serial.print(", ");
Serial.print(az);
Serial.print("\r\n");
delay(rrate);
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
