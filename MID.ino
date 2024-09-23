#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
const char* ssid = "xxx";
const char* password = "xxxx";
const char* mqtt_server = "broker.mqtt-dashboard.com";
const int mqtt_port = 1883;
const char* mqtt_client = "a76fec4dc-41ac-4f5a-8aec-7618a387fc2b";
const char* mqtt_username = "sHvvhP4Q4qTaceLiAZ8b2TbsNZ2H1rJ41";
const char* mqtt_password = "dbJtdfdaY3Ga9njMpVtruck3ZLDmssiSt";
WiFiClient espClient;
PubSubClient client(espClient);
char msg[100];
const int TRIG_PIN = D4;  
const int ECHO_PIN = D2;  
int duration, distanceiei;
#define DHTTYPE DHT11
#define DHTPIN D9 
DHT dht(DHTPIN, DHTTYPE, 15);
unsigned long int humidei,humid_old;
unsigned long int tempei, temp_old;
unsigned long int humid_max=100,temp_max=50;
void reconnect() 
{
  while (!client.connected() ) 
  {
Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_client, mqtt_username, mqtt_password)) 
    {
Serial.println("Connected");
client.subscribe("@msg/Ultrastatus");
    }
    else 
    {
Serial.print("failed, rc=");
Serial.print(client.state());
Serial.println("Try again in 5 seconds...");
      delay(5000);
    }
  }
}
void callback(char* topic,byte* payload, unsigned int length) 
{
Serial.print("Message arrived [");
Serial.print(topic);
Serial.print("]: ");
  String msg;
      for (int i = 0; i< length; i++) 
         {
           msg = msg + (char)payload[i];
         }
Serial.println(msg); 
}
void setup() 
{
pinMode(TRIG_PIN,OUTPUT);
pinMode(ECHO_PIN,INPUT);
Serial.begin(115200);
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
Serial.print(".");
  }
Serial.println("WiFi connected");
Serial.print("IP address: ");
Serial.println(WiFi.localIP());
client.setServer(mqtt_server, mqtt_port);
client.setCallback(callback);
}
void project()
{
humidei = dht.readHumidity();
tempei = dht.readTemperature();
if(humidei<=humid_max)
{
humid_old=humidei;
}
else
{}
if(humidei>humid_max)
{           
      humidei=humid_old;
}
else
{}
if(tempei<=temp_max)
{
temp_old=tempei;
}
else
{}
if(tempei>temp_max)
{                
     tempei=temp_old;
}          
else
{}
digitalWrite(TRIG_PIN, LOW);      
delayMicroseconds(2);             //หน่วงเวลา 2 msec
digitalWrite(TRIG_PIN, HIGH);     //สั่งให้ขาTRIG_PIN มีลอจิก HIGH
delayMicroseconds(10);            //หน่วงเวลา 10 msec
digitalWrite(TRIG_PIN, LOW);      //สั่งให้ขาTRIG_PIN มีลอจิก LOW 
duration = pulseIn(ECHO_PIN,HIGH); //จับเวลาสัญญาณลอจิกHIGHของขาECHOโมดูล Ultrasonicที่เข้ามายังขา2  
distanceiei = duration / 29.1 / 2 ; //คำนวณหาระยะทางมีหน่วยเป็นเซนติเมตร  
}
void loop() 
{
  if (!client.connected()) 
  {
    reconnect();
  }
project(); //เรียกให้ฟังก์ชันชื่อprojectทำงาน    
String data = String(tempei)+"," +String(humidei)+","+String(distanceiei);    
data.toCharArray(msg, (data.length() + 1));
client.publish("@msg/update", msg);
delay(1000);   
client.loop();
}
