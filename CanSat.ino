#define nsens 5 //Needed to construct the string

//Define ALL the pins
int temppin = A5;  //temperature
int trykkpin = A1; //pressure
int axelxpin = A2; //axelerometer x
int axelypin = A3; //axelerometer y
int axelzpin = A4; //axelerometer z
int buzzpin = 13; //pin for the buzzer

unsigned long buzzdelay = 10000;
bool buzzstate = LOW;
int rrate = 200; //delay in ms
long timecode = 0;
bool raw = 0;
char buffer[10];
char* charpointer = &buffer[0];

char* voltprint(int adcvalue, char* buf)
{
    int t1,t2,t3;
    unsigned long produkt;
    produkt  = (long)adcvalue*4888;
    produkt /= 1000;
    t3       = produkt % 10;
    produkt -= t3;
    produkt /= 10;
    t2       = produkt % 10;
    produkt -= t2;
    produkt /= 10;
    t1       = produkt % 10;
    produkt -= t1;
    produkt /= 10;
    buf[0]   = (char)(produkt+48);
    buf[1]   = '.';
    buf[2]   = (char)(t1+48);
    buf[3]   = (char)(t2+48);
    buf[4]   = 0; // (char)(t3+48);
    //buf[5]   = 0;
    return(buf);
}

void setup() {
  Serial.begin(9600);
  while (millis() < 1000);
  pinMode(buzzpin, OUTPUT);
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
    if (raw == true){
      sensordata.concat(sensor[i]);
    } else{
        charpointer = voltprint(sensor[i], buffer);
        sensordata.concat(buffer);
      }

    } 


  Serial.println(sensordata);
  
  if (millis() > buzzdelay){
   digitalWrite(buzzpin, !buzzstate); 
  }

  while (millis() < timecode + rrate);
}
