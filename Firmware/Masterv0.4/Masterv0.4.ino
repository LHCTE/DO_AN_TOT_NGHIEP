#include <SPI.h> 
#include <nRF24L01.h> 
#include <RF24.h> 
#include <LiquidCrystal.h>

//------ Khai báo biến --------- 
#define btn1 40
#define btn2 44
boolean status_btn1 = 0;
boolean status_btn2 = 0;
//byte st1 = 0;
char st1[32] = "control1;ld1";
String st2 = "";
String data = "";

// Khai báo dử dụng lcd 
LiquidCrystal lcd(A0, A2 , A4, A6, A8, A10);

const int pinCE = 31; 
const int pinCSN = 29; 
RF24 radio(pinCE, pinCSN); 

const uint64_t rAddress[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL };

byte daNumber = 0; 
byte gotByte = 0; 

void setup()   
{
  Serial.begin(9600);
  Serial2.begin(9600);    
  Serial.println("Start"); 
  radio.begin();  //Start the nRF24 module

  radio.setPALevel(RF24_PA_LOW);  
  radio.setChannel(108);          

  // Open up to six pipes for PRX to receive data
  radio.openReadingPipe(0,rAddress[0]);
  radio.openReadingPipe(1,rAddress[1]);
  radio.openReadingPipe(2,rAddress[2]);
  radio.openReadingPipe(3,rAddress[3]);
  radio.openReadingPipe(4,rAddress[4]);
  radio.openReadingPipe(5,rAddress[5]);
  radio.startListening(); 
  //  pinMode(sensor, INPUT);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  lcd.setCursor(14, 1);
  lcd.print("Done");
  delay(1000);
  display_setting();                
}

void loop()  
{   
    byte pipeNum = 0; 
    gotByte = 0; 
    char getData[32] = {0};
    //String getData = "";
    while(radio.available(&pipeNum)){ //Check du lieu tu node gui ve
     radio.read( &getData, sizeof(getData) ); 
     Serial.print("Received from transmitter: "); 
     Serial.println(pipeNum + 1); 
     Serial.print("Data receive :");
     Serial.println(getData);
     // Xác thực data node1
     // Xác thực data node2
     // Gửi dữ liệu lên esp
     }
//     if (Serial2.available() > 0 ) { // Nhận dữ liệu từ esp
//     data = Serial2.readString();
//     String id = getValue(data, ';', 0);
////     if ((id == "slave1")&&(data.length() == 35)) { // Gửi dữ liệu xuống node 1 
////      sendDatatoNode(1);
////      }
////     if ((id == "slave1")&&(data.length() == 35)) { // Gửi dữ liệu xuống node 2 
////      sendDatatoNode(1);
////      }
//    }
     read_input();
}
bool sendDatatoNode(byte xMitter) 
{
    bool worked; 
//   unsigned char st2[] = st1;
    radio.stopListening(); 
    radio.openWritingPipe(rAddress[xMitter]); 
    if(!radio.write(&st1, sizeof(st1)))  worked = false; 
    else worked = true; 
    radio.startListening(); 
    return worked;  
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
    status_btn1 = !status_btn1;
    //msg[] = status_btn1 ;
    Serial.println(status_btn1);
    //char msg[6];
    if (status_btn1 == 1)
    {
      //msg[0]= 1;
      //st1 = "";
      //st1 = "control1;ld1";
      st1[7] = '1';
      st1[11] = '1';
    }
    else {
      st1[7] = '1';
      st1[11] = '2';
    }
    Serial.println("Nut 1 duoc nhan");
    if(sendDatatoNode(1)){
     Serial.println("Sending success"); 
    }
    else Serial.println("Sending fail"); 

  }
  int value_btn2 = debounce(btn2);
  if (!value_btn2) // check nút 1
  {
    status_btn2 = !status_btn2;
    Serial.println(status_btn2);
    
    if (status_btn2 == 1){
      // st1 = 1;
      //st1 = 22;
      st1[7] = '2';
      st1[11] = '1';
    }
    else {
      st1[7] = '2';
      st1[11] = '2';
    }
    Serial.println("Nut 2 duoc nhan");
    if(sendDatatoNode(2)){
     Serial.println("Sending success"); 
    //Serial.println("Sending success");
    }
    else Serial.println("Sending fail"); 
  }
}
