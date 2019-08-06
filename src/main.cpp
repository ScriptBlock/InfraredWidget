#include <Arduino.h>
#include <IRremote.h>

const int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
IRsend irsend;
decode_results results;

const int sendMode = 1;

void setup() {

  Serial.begin(19200);
  Serial.println("IR Widget starting");
  
  if(sendMode ==1 ) {
    irsend.sendNEC(1474553879, 32);
  } else {
    irrecv.enableIRIn();
  }
}



void printCodeStuff(decode_results *results) {
  Serial.print("decode type: " );
  Serial.println(results->decode_type);

  switch(results->decode_type) {
    case NEC: Serial.println("NEC"); break;
    case SONY: Serial.println("SONY"); break;
    case PANASONIC: Serial.println("PANASONIC"); break;
    case JVC: Serial.println("JVC"); break;
    default:
      Serial.println("Unknown type");

  }

  Serial.print("value: " );
  Serial.println(results->value, HEX);
  Serial.println(results->value);

  Serial.print("bits: " );
  Serial.println(results->bits);

 
}


void loop() {
  if(sendMode != 1) {
    if(irrecv.decode(&results)) {
      Serial.println("got code");
      printCodeStuff(&results);
      irrecv.resume();
    }
  }
}