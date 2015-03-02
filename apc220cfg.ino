#include <SoftwareSerial.h>

/* plug in apc220 at pin 8-GND with apc pcb pointing away from Arduino
* Jens(AAU), Thomas(ARS)
*/

const int vrs = 104;//
const int rxPin = 10; //rx pin is tx pin on apc
const int txPin = 11; //tx pin is rx pin on apc
const int setPin = 8; //set pin
const int enPin = 12; // no use
const int fiveV = 13;  // 5V to the APC220

int echo_mode = 0; 

#define TIMEOUT 10000L
#define PING_INTERVAL 2000L

unsigned long timeout,lastPing;

#define PARM_LGT 10
#define CMD_BUF_LGT 50

char var1[PARM_LGT], var2[PARM_LGT], var3[PARM_LGT], var4[PARM_LGT], var5[PARM_LGT], var6[PARM_LGT];

SoftwareSerial apc220(rxPin, txPin); // Crt softserial port and bind tx/rx to appropriate PINS

void drawRadio()
{
 
  Serial.println(F(">>>>>>>>>>>>>>>>>>>>>>>>> "));
  Serial.println(F("Attach radio directly to Arduino on pin 8 to gnd as shown below"));
  Serial.println(F(" "));
  Serial.println(F("  ---------------"));
  Serial.println(F("  |      A   aref"));
  Serial.println(F("  |      R    GND-++-GND-------------"));
  Serial.println(F("  |      D     13 ++ 5V             |=antenna connector ->"));
  Serial.println(F("  IOREF  U     12 -- not used       |"));
  Serial.println(F("  RESET  I     11 ++ RX             |"));
  Serial.println(F("  3.3V   N     10 ++ TX             |"));
  Serial.println(F("  5V     O     09 -- not used       |"));
  Serial.println(F("  GND          08-++ CONFIG----------"));
  Serial.println(F("  GND    P      |"));
  Serial.println(F("  Vin    C     07"));
  Serial.println(F("  |      B   ~ 06"));
  Serial.println(F("  A0         ~ 05"));
  Serial.println(F("  A1           04"));
  Serial.println(F("  A2         ~ 03"));
  Serial.println(F("  A3           02"));
  Serial.println(F("  A4        TX 01"));
  Serial.println(F("  A5        RX 00"));
  Serial.println(F("  ---------------"));
 }
 
 void drawConnect()
 {
   Serial.println("7 pins - no 1 closest to XTAL no 7 closest to antenna");
   Serial.println("1(D08): set  pin ");
   Serial.println("2(D09): not used");
   Serial.println("3(D10): TX");
   Serial.println("4(D11): RX");
   Serial.println("5(D12): No use");
   Serial.println("6(D13): 5V");
   Serial.println("7(GND): GND");
   Serial.println("");
   Serial.println("");
 }

void set_para(char hz[], char rf_rate[], char pwr[], char uart_rate[], char sc[]) {
// sanity chk
  if (strlen(hz) != 6) {
    Serial.println("Freq parm not 6 digits... - legal is 418000 - 455000");
  return;
  }
  if (strlen(rf_rate) != 1 ) {
    Serial.println("RF parm is not 1 digit: legal values is 1/2/3/4");
    return;
  }
  if (strlen(pwr) != 1 ) {
    Serial.println("Power parm is not 1 digit: legal values is 1..9"); 
    return;
  }

  if (strlen(uart_rate) != 1 ) {
    Serial.println("Uart baudrate parm is not 1 digit: legal values is 0..6"); 
    return;
  }
    if (strlen(sc) != 1 ) {
    Serial.println("Parity parm is not 1 digit: legal values is 0/1/2"); 
    return;
  }


  digitalWrite(setPin, LOW); // set radio in config mode
  delay(50);
  apc220.print("WR");
  apc220.print(" ");
  
  apc220.print(hz);
  apc220.print(" ");
  
  apc220.print(rf_rate);
  apc220.print(" ");
  
  apc220.print(pwr);
  apc220.print(" ");
  
  apc220.print(uart_rate);
  apc220.print(" ");
  
  apc220.print(sc);
  
  apc220.write(0x0D);
  apc220.write(0x0A);
  delay(10);

   // read feedback from radio	
 
  while (apc220.available()) {
    Serial.write(apc220.read());
  }

  digitalWrite(setPin, HIGH); // set radio back in normal rx/tx mode
}

void get_para(void) {
  int i = 0;
  char buff[CMD_BUF_LGT];

  digitalWrite(setPin, LOW); // set radio in config mode
  
  delay(10); // stabilize please
  
  apc220.println("RD"); // ask for data
  delay(10);
  
  while (apc220.available()) {
    Serial.write(apc220.read()); 
  }
  
  digitalWrite(setPin, HIGH); // set radio back in normal tx/rx mode
}

void resetPing(void)  {
  echo_mode = 0;
  timeout = millis();  
}

void helpMenu(void) {
  Serial.println("");
  Serial.println(F("commands:"));
  Serial.println(F("  r :  Read apc220 radio config")); 
  Serial.println(F("  e :  go into echo mode: receive char, add 1 and return"));
  Serial.println(F("  n :  no more echo - back to normal")); 
  Serial.println(F("  p:   print how to attach radio to Arduino"));
  Serial.println(F("  c:   print pinout on apc220")); 
  Serial.println(F("  w :  Write apc radio config ...")); 
  Serial.println(F("     'W'  FFFFFF R P B C - number of letters indicates precise number of digits" ));
  Serial.println(F("      FFFFFF: frequency: 434000 (434 MHz default ) range 418000-455000  ")); 
  Serial.println(F("      R:      Rf data rate       - 1/2/3/4 equals 2400(1km)/4800/9600/19200bps"));
  Serial.println(F("      P:      Radio output power - 0 .. 9 9 equals 13dBm(20mW)."));
  Serial.println(F("      B:      UART baudrate      - 0/1/2/3/4/5/6 equals 1200/2400/4800/9600/19200/38400/57600bps"));
  Serial.println(F("      C:      Byte Chek Parity   - 0/1/2 equals NoCheck(8N1)/EvenParity(8E1)/OddParity(8O1)")); 
  Serial.println(F(""));
  Serial.println(F("Write example: w 434000 3 9 3 0 is..."));
  Serial.println(F("    434,000 MHz 9600 baud in air, 20mW, 9600baud on UART, No Parity(8N1)"));
  Serial.println(F("After 10 seconds with no keyboard input we will emit a char every two second"));
}

void get_cmd(void) {

  /*
  if ((echo_mode == 0) && (TIMEOUT < (millis() - timeout))) { // time and state for beacon mode ?
    apc220.write('!');
    Serial.println("going into ping mode - emit a char every two second on apc220  !");    
    lastPing = millis();
    echo_mode = 2;
    goto xxx;
  }
  
  
  if (echo_mode == 2) { // beacon mode
    if (PING_INTERVAL < (millis() - lastPing)) {
      apc220.write('x');
      Serial.println(lastPing/1000);       
      lastPing = millis();
    }
    goto xxx;
  }
*/

  if (echo_mode == 1) { // echo mode
    char c;
    digitalWrite(4,HIGH); delay(1000); digitalWrite(4,LOW);
     if (apc220.available()) {
       digitalWrite(4,HIGH);
        c = apc220.read();
        Serial.write(c); 
        apc220.write(c+1); 
        delay(200);
        digitalWrite(4,LOW);
      }
      goto xxx;
  }
  
  xxx:
 
   if (Serial.available()) {  
    int i=0;
    char buff[CMD_BUF_LGT];
   
   delay(100);
   
    while (Serial.available() && (i < CMD_BUF_LGT-1)) {
      buff[i++] = Serial.read();
    }
    buff[i] = '\0';
    Serial.println(buff); //echo input
 
    resetPing();

    var1[0] = 0x00; // reset
    sscanf(buff, "%s %s %s %s %s %s", var1, var2, var3, var4, var5, var6);
    switch (var1[0]) { // one letter commands :-)
    case 'r': {
      get_para();
      break;
    }
    case 'w': {
      set_para(var2, var3, var4, var5, var6);
      break;
    }
    case 'e' :{       
      Serial.println(F("entering echo mode"));
      echo_mode = 1;
      break;
    }
    case 'n' :{
      Serial.println(F("leaving echo mode."));
      echo_mode = 0;
      break;
    }
    case 'p':{
      drawRadio();
      delay(2000);
      break;
    }
    case 'c':{
      drawConnect();
      delay(2000);
      break;
    }
    default:
      echo_mode = 0;    
      helpMenu();
    }
   }
}

void setupSoftAPC(void)
{
  pinMode(setPin,OUTPUT);
  digitalWrite(setPin,HIGH);
  pinMode(fiveV,OUTPUT);  // 5V
  digitalWrite(fiveV,HIGH); // turn on 5V
  delay(50);
  pinMode(enPin,OUTPUT); // ENABLE
  digitalWrite(enPin,HIGH); //
  delay(100);
  apc220.begin(9600);
  pinMode(4,OUTPUT);
  digitalWrite(4,LOW);
}

void setup() {
   Serial.begin(9600);
   Serial.print("APC version: ");
   Serial.println(vrs);
   Serial.println(__DATE__);
   Serial.println("type h for help");
   setupSoftAPC();
   timeout = millis(); // just to start
}

void loop() {
	get_cmd();
}
