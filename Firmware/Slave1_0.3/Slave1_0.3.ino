#include <SPI.h> 

#include <nRF24L01.h> 

#include <RF24.h> 

const int pinCE = 7; 

const int pinCSN = 8; 

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

RF24 radio(pinCE, pinCSN); 

#define WHICH_NODE 2     

const uint64_t wAddress[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL};

const uint64_t PTXpipe = wAddress[ WHICH_NODE - 1 ];   

byte counter = 1;
int count = 0;
unsigned long time_check = 0;  

char stringsta[] = "slave1;ld1:b;ld2:b";
const int sttsta[] = {11,17};
const int load[] = {4,2};

unsigned long printPeriod = 1000; 
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

void setup()  

{

  Serial.begin(9600);   
  radio.begin();            
  radio.setPALevel(RF24_PA_LOW);  // note
  radio.setChannel(108);          
  radio.openReadingPipe(0,PTXpipe);  
  radio.stopListening(); 
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
    byte gotByte = 0;
    char getData[32] = {0};
    while(radio.available()){ //Check du lieu tu Master
     radio.read( &getData, sizeof(getData) ); 
     Serial.print("Received from master: "); 
     Serial.println(getData);
     //Serial.println(getData);
     // Xác thực data
      data = getData;
      Serial.println(data.length());
      id  = getValue(data, ';', 0);
      t2  = getValue(data, ';', 1);
      if ((id == "control1")&&(data.length() == 12)) { 
      check_control(); 
      }
     // Xác thực data node2
     // Gửi dữ liệu lên esp
     }
     if((unsigned long)(millis() - previousMillis) >= printPeriod) { 
      previousMillis = millis();
      //count++;
      readstatus();
      byte dataSend = 11; 
      //String tesst = "aaa";
      //String dataSend = String(stringsta); 
      radio.stopListening();
      radio.openWritingPipe(PTXpipe);
      if (!radio.write( &stringsta, sizeof(stringsta) )){  // Check error 
         Serial.println(" Write failed");      
     } 
     else { //if the write was successful 
      Serial.print("Success sending: ");
      Serial.println(stringsta);
     }
     radio.startListening();
     }
     read_input();
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
  if (t2 == "ld1")
  {
    status_btn1 = !status_btn1;
    action();
  }
  if (t2 == "ld2")
  {
    status_btn2 = !status_btn2;
    action();
  }
  if (t2 == "ld3")
  {
    action();
  }
  if (t2 == "ld4")
  {
    action();
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
  
  boolean value = debounce(btn1);
  if (!value)
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
