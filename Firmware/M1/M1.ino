#include <RF24Network.h>
#include <SPI.h> 
#include <RF24.h> 
#include <LiquidCrystal.h>

//------ Khai báo biến --------- 
#define btn1 40
#define btn2 44
boolean status_btn1 = 0;
boolean status_btn2 = 0;
//byte st1 = 0;
//char st1[32] = "buzz:on";
//String st1 = "buzz:on";
char st2[32];
byte msgtrs[6];
String data ;

// Khai báo dử dụng lcd 
LiquidCrystal lcd(A0, A2 , A4, A6, A8, A10);
RF24 radio(31, 29);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;      // Address of the other node in Octal format
const uint16_t node02 = 02;
const uint16_t node022 = 022;
unsigned long previousMillis;

void setup()   
{
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial2.setTimeout(200);    
  Serial.println("Start"); 
  radio.begin();
  network.begin(90, this_node);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  //  pinMode(sensor, INPUT);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  lcd.setCursor(14, 1);
  lcd.print("Done");
  delay(1000);
  //display_setting();                
}

void loop()  
{   
    network.update();
    //String getData = "";
    while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    //unsigned long incomingData;
    char incomingData[32];
    network.read(header, &incomingData, sizeof(incomingData)); // Read the incoming data
    Serial.println(incomingData);
    Serial2.println(incomingData);
   // analogWrite(led, incomingData);    // PWM output to LED 01 (dimming)
  }
     if (Serial2.available() > 0 ) { // Nhận dữ liệu từ esp
     data = Serial2.readString();
     data.trim();
     Serial.println(data);
       // str.toCharArray(char_array, str_len);
     String id = getValue(data, '-', 0);
     String load = getValue(data, '-', 1);
     String check1 = getValue(load, ':', 0);
     String check2 = getValue(load, ':', 1);
     Serial.println(load);
     //Serial.println(load.length() + 1);
     char data_conv[load.length()];
     load.toCharArray(data_conv, load.length() + 1);
     //data_conv = load;
     if (id == "room1") { // Gửi dữ liệu xuống node 1
      Serial.println("Room1");
        RF24NetworkHeader header2(node01);
        bool ok2 = network.write(header2, &data_conv, sizeof(data_conv)); 
        Serial.print("Node1: ");
        Serial.println(ok2);
      }
     if (id == "room2") { // Gửi dữ liệu xuống node 2 
        RF24NetworkHeader header4(node02);
        bool ok2 = network.write(header4, &data_conv, sizeof(data_conv)); 
        Serial.print("Node2: ");
        Serial.println(ok2);
      }
     }
     //read_input();
}


void display_setting(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Do an tot nghiep");
  lcd.setCursor(0, 1);
  lcd.print("Dieu khien, giam sat tu xa");
}
// -------Hàm check nhấn nút-------- 
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
//------Hàm lấy giá trị từ chuỗi -----
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

void read_input()
{
  int value_btn1 = debounce(btn1);
  if (!value_btn1) // check nút 1
  {
    //char msg[6];
    Serial.println("Nut 1 duoc nhan");

  }
  int value_btn2 = debounce(btn2);
  if (!value_btn2) // check nút 1
  {
    status_btn2 = !status_btn2;
    Serial.println("Nut 2 duoc nhan");
    
  }
}
