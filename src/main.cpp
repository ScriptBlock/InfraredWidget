#include <Arduino.h>
#include <IRremote.h>
//#include <EEPROM.h>
#include <EEPROMex.h>


IRsend irsend;

char incomingChar; 
long startupDelay = 0;
String startupDelaySerial; //i smell a buffer overflow waiting to happen
int serialReadCounter = 0;




String lts(long l) {
  char buffer[10];
  String retVal = ltoa(l, buffer, 10);
  return retVal;
}

void logToSerial(String s) {
  Serial.println(lts(millis()) + ": " + s);
}


void sendPowerCommand() {
  irsend.sendNEC(1474553879, 32);
}

void setup() {
  Serial.begin(19200);
  EEPROM.setMemPool(0,EEPROMSizeNano);
  EEPROM.setMaxAllowedWrites(100);
  
  logToSerial("IR Widget starting");
  
  startupDelay = EEPROM.readLong(0);

  logToSerial("Startup delay time = " + lts(startupDelay));
  
  //Just sleep for the amount of time set in eeprom
  delay(startupDelay);

  //This code is specifically the power cycle code for the NEC.  For other TVs
  //you would need to replace this.  If someone wants to be a pal and look up
  //the most popular codes and make that a feature, have at it.
  logToSerial("Delay over, sending IR Code for power");
  sendPowerCommand();
}

void setupdebug() {
  Serial.begin(19200);
  EEPROM.writeLong(0,30000);
  long tep = EEPROM.readLong(0);
  Serial.println(tep);
}


void loop() {
  //while(Serial.available()) {
    if (Serial.available() > 0) {
      incomingChar = Serial.read();

      //use Dxxxxx through serial to tell the chip to store that xxxxx value as the startup time
      if(incomingChar == 'D') {
        logToSerial("Received command to set startup delay time");
        serialReadCounter = 0;
        startupDelaySerial = "";

        //watch for EOL to know we're done collecting the digits for the delay value (max 99,999)
        while(Serial.available() > 0 && incomingChar != '\n' && incomingChar != '\r' && serialReadCounter++ < 5) { 
          logToSerial("Read another character from serial");
          incomingChar = Serial.read(); 
          if(isDigit(incomingChar)) {
            logToSerial("added a digit");
            startupDelaySerial += incomingChar;
          }
        }
        logToSerial("Received the following string from serial: " + startupDelaySerial);
        //we've gotten our digits
        startupDelay = atol(startupDelaySerial.c_str());
        
        logToSerial("Received a command to set the delay startup to " + startupDelaySerial + " millis");
        EEPROM.writeLong(0,startupDelay);
        logToSerial("Successfully wrote to EEPROM"); //that's a bit presumptuous. i know.

      }

      if(incomingChar == 'P') {
        logToSerial("Received command to send power signal");
        sendPowerCommand();
      }

    }
  //}  
}
