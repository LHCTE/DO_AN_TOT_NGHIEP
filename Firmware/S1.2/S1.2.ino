#include <SPI.h> 
#include <RF24Network.h>
#include <RF24.h> 

#define leddigital 4
#define ledsensor  2
#define buzz       A4
#define btn1       A0
#define btn2       A1
#define btn3       A2
#define btn4       A3
#define pir        10
#define dimmer     3
#define ledR       9
#define ledG       6
#define ledB       5

RF24 radio(7, 8);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 02;   // Address of our node in Octal format ( 04,031, etc)
const uint16_t master00 = 00;    // Address of the other node in Octal format   

byte counter = 1;
int count = 0;
unsigned long time_check = 0;  

char stringsta[] = "slave2;ld1:b;ld2:b";
const int sttsta[] = {11,17};
const int load[] = {4,2};

unsigned long printPeriod = 1400; 
unsigned long previousMillis = 0;

bool done = false; 
String data = "";
String ld1 = "";
String ld2 = "";
String ld3 = "";
String ld4 = "";
String t2 = "";
String id = "";

boolean status_btn1 = 0;
boolean status_btn2 = 0;
boolean status_btn3 = 0;
boolean status_btn4 = 0;
unsigned long incomingData;
unsigned long valueR;
unsigned long valueG;
unsigned long valueP;

void setup()  

{

  Serial.begin(9600);   
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);         
  pinMode(leddigital, OUTPUT);
  pinMode(ledsensor, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(btn4, INPUT_PULLUP);
  pinMode(pir, INPUT_PULLUP);
  pinMode(dimmer, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  analogWrite(ledR,100);
  analogWrite(ledG,255);
  analogWrite(ledB,100);
  analogWrite(dimmer,0);
  digitalWrite(buzz , HIGH);

}

void loop()  

{
    network.update();
    //unsigned long getData;
    while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    //unsigned long incomingData;
    char incomingData[128]= {0};
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    Serial.println(incomingData);
    
    
    if (header.from_node == 0) {    
    Serial.println("Data ok");
    }
    String data = String(incomingData);
    id = getValue(data, ':', 0);
    t2 = getValue(data, ':', 1);
    check_control();
    }
     if((unsigned long)(millis() - previousMillis) >= printPeriod) { 
      previousMillis = millis();
      //count++;
      readstatus();
      //byte datasend[6]; 
      //datasend[0] = 11;
      RF24NetworkHeader header10(master00);
      bool ok2 = network.write(header10, &stringsta, sizeof(stringsta)); 
      Serial.print("Node2: ");
      Serial.println(ok2);
     }
     read_input();
/*     
     if (digitalRead(pir) == 0)
    {
     digitalWrite(buzz, LOW); 
     delay(1000);
     digitalWrite(buzz, HIGH);
    }
*/
      if (digitalRead(pir) == 0)
    {
     digitalWrite(buzz, LOW); 
     delay(500);
    }
    else
    {
     digitalWrite(buzz, HIGH);
    }   
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void readstatus(){  
  for (int i = 0 ; i < 2 ; i++){
    if(digitalRead(load[i]) == 0) {
      stringsta[sttsta[i]] = 'b';
    }
    else {
      stringsta[sttsta[i]] = 'a';
    }
  }
}
void check_control()
{
  if (id == "ld1")
  {
    status_btn1 = !status_btn1;
    action();
  }
  if (id == "ld2")
  {
    status_btn2 = !status_btn2;
    action();
  }
  if (id == "ld3")
  {
    analogWrite(dimmer,t2.toInt());
  }
  if (id == "ld4")
  {
    String val1 = getValue(t2, ',', 0);
    String val2 = getValue(t2, ',', 1);
    String val3 = getValue(t2, ',', 2);
    analogWrite(ledG,val1.toInt());
    analogWrite(ledR,val2.toInt());
    analogWrite(ledB,val3.toInt());
  }
}
void action()
{
  if (status_btn1 == 1)
  {
    digitalWrite(leddigital, HIGH);
  }
  else digitalWrite(leddigital, LOW);
  
  if (status_btn2 == 1) 
  {
    digitalWrite(ledsensor, HIGH);
  }
  else digitalWrite(ledsensor, LOW);
  
  if (status_btn3 == 1) 
  {
    analogWrite(dimmer,255);
  }
  else analogWrite(dimmer,0);
  
  if (status_btn4 == 1) 
  {
    analogWrite(ledR,255);
    analogWrite(ledG,255);
    analogWrite(ledB,255);
  }
  else {
    analogWrite(ledR,0);
    analogWrite(ledG,0);
    analogWrite(ledB,0);
  }
}

// Hàm đọc nút nhấn điều khiển
void read_input()
{
//  if ((digitalRead(pir) == 0)&& (count == 0)) {
//    digitalWrite(ledsensor, HIGH); 
//    count = 1;
//    time_check = millis(); 
//  }
//  if(((unsigned long)(millis() - time_check) >= 3000)&&(digitalRead(pir) == 1)){
//     digitalWrite(ledsensor, LOW);
//     count = 0; 
//  }
 /* boolean valueP = debounce(pir);
  if (!valueP)
  {
    digitalWrite(buzz, LOW);
    time_check = millis(); 
   // status_btn1 = !status_btn1;
   // action();
  }*/
//  if(((unsigned long)(millis() - time_check) >= 3000)&&(!digitalRead(buzz)))
//  {
//    digitalWrite(buzz, HIGH);
//  }

  boolean value0 = debounce(btn1);
  if (!value0)
  {
    status_btn1 = !status_btn1;
    action();
  }
  boolean value2 = debounce(btn2);
  if (!value2)
  {
    status_btn2 = !status_btn2;
    action();
  }
   boolean value3 = debounce(btn3);
  if (!value3)
  {
    status_btn3 = !status_btn3;
    action();
  }
   boolean value4 = debounce(btn4);
  if (!value4)
  {
    status_btn4 = !status_btn4;
    action();
  }
}

// Hàm check nút nhấn 
boolean debounce(int pin)
{
  boolean state;
  boolean previousState;
  const int debounceDelay = 60;

  previousState = digitalRead(pin);
  for (int counter = 0; counter < debounceDelay; counter++)
  {
    delay(1);
    state = digitalRead(pin);
    if (state != previousState)
    {
      counter = 0;
      previousState = state;
    }
  }
  return state;
}
