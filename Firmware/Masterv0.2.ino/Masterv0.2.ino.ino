#include <LiquidCrystal.h> 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define btn1 40
#define btn2 44
//------ Khai báo biến --------- 
boolean status_btn1 = 0;
boolean status_btn2 = 0;
// Khai báo dử dụng lcd 
LiquidCrystal lcd(A0, A2 , A4, A6, A8, A10);
// Khai báo NRF
const uint64_t pipe = 0xE8E8F0F0E1LL; // địa chỉ để phát
RF24 radio(31,29); // CE, CS
const byte address[6] = "20201";
byte msg[3];
//const int sensor = A3;
int value = 0;
void setup() {
  Serial.begin(9600);
  lcd.begin(20,4);
  lcd.print("Starting ... ");
  delay(500);
  Serial.println("Start");
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
//  pinMode(sensor, INPUT);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  lcd.setCursor(14, 1);
  lcd.print("Done");
  delay(1000);
  display_setting();
  // put your setup code here, to run once:

}

void loop() {
  read_input();
 // radio.write(&msg, sizeof(msg));
  delay(100);
  // put your main code here, to run repeatedly:

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
      msg[0]= 1;
    }
    else msg[0]=0;
    radio.write(&msg, sizeof(msg));
    Serial.println("Nut 1 duoc nhan");
    Serial.print("Data: ");
    Serial.println(msg[0]);
  }
  int value_btn2 = debounce(btn2);
  if (!value_btn2) // check nút 1
  {
    status_btn2 = !status_btn2;
    Serial.println(status_btn2);
    
    if (status_btn2 == 1){
       msg[1] = 3;
    }
    else msg[1] = 4;
    radio.write(&msg, sizeof(msg));
    Serial.println("Nut 2 duoc nhan");
    Serial.print("Data: ");
    Serial.println(msg[1]);
  }
}
