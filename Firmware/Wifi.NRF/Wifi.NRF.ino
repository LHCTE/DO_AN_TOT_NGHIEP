// Khai báo thư viện
#include <ESP8266WiFi.h>
#include <string.h>
#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
//#include <SimpleTimer.h>
#include <ArduinoJson.h>

String ver = "1.0";
// Thư viện simpletimer
//SimpleTimer timer;
// Khai báo led báo kết nối wifi
#define wifi  D3
#define lamp  D4  
// Khai báo các biến lưu trữ 
char ssid_new[20] = "" ; 
char pass_new[20] = "" ;
char ssid_buff[20] = "" ; 
char pass_buff[20] = "" ;

boolean statusld1 =  0 ;
boolean statusld2 =  0 ;
boolean statusld3 =  0 ;
boolean statusld4 =  0 ;

int addr = 0;

// wifi kết nỗi mặc định
char ssid_default[] = "Nha Co Bay";
char pass_default[] = "01645965164";
// Thông tin server MQTT
#define mqtt_server "m16.cloudmqtt.com"  
#define mqtt_topic_pub "subscribeTopic4"    
#define mqtt_topic_sub "publishTopic4"  
#define mqtt_user "tnuxyfho"    
#define mqtt_pwd "JrFzY67tUXXT"

char sever[] = "";
char mqtt2[200] = "";

const uint16_t mqtt_port = 19928; 
WiFiClient espClient;
PubSubClient client(espClient);

String data = "";
String data2 = "";
String id = "";
String l1 = "";
String l2 = "";
String l3 = "";
String l4 = "";
String receiver = "";
boolean over = 1;
// Chuỗi điều khiển 
char control[] = "control;ldx"; 
// hàm xử lý tách chuỗi
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

void setup()
{
  Serial.begin(9600);
  Serial.setTimeout(200);
  WiFi.begin(ssid_default, pass_default);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi default connected"); 
  // Khai báo input 
  pinMode(wifi, OUTPUT);
  pinMode(lamp, OUTPUT);
  digitalWrite(wifi, HIGH);
  digitalWrite(lamp, HIGH);                       
  // Setup gửi nhận lên server 
  client.setServer(mqtt_server, mqtt_port); 
  client.setCallback(callback);
  
}
// Hàm nhận dự liệu từ server
void callback(char* topic, byte* payload, unsigned int length) {
  for (int i = 0; i < length; i++) {
  char receivedChar = (char)payload[i];
  receiver += receivedChar;
  }
  //Serial.println();
  //Serial.print("Chuoi nhan duoc: ");
  Serial.println(receiver);
  receiver = "";
}
// Hàm kết nối lại nếu bị ngắt kết nối
void reconnect() {
  // Chờ tới khi kết nối
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    // Thực hiện kết nối với mqtt user và pass
    if (client.connect("ESP8266Client",mqtt_user, mqtt_pwd)) {
     // digitalWrite(ledPin, HIGH);
      Serial.println("connected");
      digitalWrite(wifi, HIGH);
      digitalWrite(lamp, HIGH);
      // Khi kết nối sẽ publish thông báo
      client.publish(mqtt_topic_pub, "ESP_reconnected");
      // ... và nhận lại thông tin này
      client.subscribe(mqtt_topic_sub);
    } else {
      //digitalWrite(ledPin, LOW);
      digitalWrite(wifi, LOW);
      digitalWrite(lamp, LOW);
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1 seconds");
      // Đợi 5s
      delay(1000);
    }
  }
}

void loop()
{
  // Nếu nhận được dữ liệu từ master 
if (Serial.available() > 0 )
{
    data = Serial.readString();
    //Serial.println(data);//    
    data.trim();
   //Serial.println(data);
   //Serial.println(data.length());
    // Xử lý tách dữ liệu
     id = getValue(data, ';', 0);
     l1 = getValue(data, ';', 1);
     l2 = getValue(data, ';', 2);
    // Serial.println(id);
    //Serial.println(l2);
    //data= "";
     checkStatus();
  }    
   // Kết nối lại server nếu bị ngắt kết nối
     if (!client.connected()) {
      reconnect();
     }
      client.loop(); 
}

// Hàm xử lý khi nhận được chuỗi
void checkStatus()
{
 //Serial.println("Step1");
 // xác thực đúng chuỗi nhận được
 if (data.length() == 18)  // update
 { 
  String infix = "";
  if (id == "slave1") { 
    infix = "room1-";
  }
  if (id == "slave2") { 
    infix = "room2-";
  }
  // Dữ liệu gửi lên web server
  String temp = infix + l1 + String(";") + l2 + String("-statusA");
  temp.toCharArray(mqtt2,100);
  client.publish(mqtt_topic_pub, mqtt2);
  //Serial.println(temp);
 // sendmqtt();
 }
}
// Hàm truyền dữ liệu lên web server mqtt

  
