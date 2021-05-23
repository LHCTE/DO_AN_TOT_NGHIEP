#include <SPI.h>
#include "RF24.h"

#define leddigital 4
#define buzz       A4

const uint64_t pipe = 0xE8E8F0F0E1LL; 
const byte address[6] = "20201";
RF24 radio(7,8); // CE, CS
byte msg[3];
const int led = 3;
int led_st = 0;
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  pinMode(leddigital, OUTPUT);
  pinMode(buzz, OUTPUT);
  digitalWrite(buzz , HIGH);
  // put your setup code here, to run once:
}

void loop()
{
  if (radio.available())
  {
    while (radio.available())
    {
      radio.read(&msg, sizeof(msg));
      Serial.print("Data nhan: ");
      Serial.println(msg[0]);
      if (msg[1] == 3)
      {        
        digitalWrite(buzz,LOW);
      }
      else
      {
       digitalWrite(buzz,HIGH); 
      }      

    }
  }

}
void  blink_buzz (int n) 
{
  for (int i = 0 ; i < n ; i++) 
  {
    digitalWrite(buzz , LOW);
    delay (300) ;
    digitalWrite(buzz , HIGH);
    delay (300) ;
  }
}
