/*

 */

#include <WiFi.h>
#include <Wire.h>
#include <BH1750.h>
#include <PubSubClient.h>
#include "DHT.h"


// The digital pin we're connected to.
#define DHTPIN 8
#define DHTTYPE DHT11   // DHT 11
#define soilPin 11

//設定 wifi 參數
char ssid[] = "network"; //  your network SSID (name)
char pass[] = "password";    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;

//設定 MQTT 參數
char mqttServer[]    = "mqttgo.io"; // MQTT Broker
char clientId[]      = "amebaClient";
char publishTopic[]  = "YourTopic/class/Temp"; 
char HumTopic[]  = "YourTopic/class/Hum"; 
char publishPayload[] = "hello world";
char subscribeTopic[] = "YourTopic/class/sensor";

String msgStr = "";      // 暫存MQTT訊息字串


// 設定時間間隔
unsigned long prevMillis = 0;  // 暫存經過時間（毫秒）
const long interval = 5000;  // 上傳資料的間隔時間，20秒。


WiFiClient wifiClient;
PubSubClient client(wifiClient);
DHT dht(DHTPIN, DHTTYPE);

BH1750 lightMeter;

void reconnect() {
  // Loop until we're reconnected
  while (!(client.connected())) {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect(clientId)) {
          Serial.println("connected");
          //Once connected, publish an announcement
          client.publish(publishTopic, publishPayload);
          client.subscribe(subscribeTopic);
      } else {
          Serial.println("failed, rc=");
          Serial.print(client.state());
          Serial.println(" try again in 5 seconds");
          //Wait 5 seconds before retrying
          delay(5000);
          }
  }
}

void setup()
{
    // Initialize serial and wait for port to open:
    Serial.begin(115200);
    dht.begin();
    Wire.begin();
    lightMeter.begin();
    pinMode(soilPin, INPUT);//土壤濕度感測器的接腳，讀取數值
    // wait for serial port to connect.
    while (!Serial) {
        ;
    }

    // 嘗試連線 WIFI
    while (status != WL_CONNECTED) {
        Serial.print("\r\nAttempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
        // wait 10 seconds for connection:
        delay(10000);
    }
    // 設定MQTT Broker
    client.setServer(mqttServer, 1883);

    //Allow Hardware to sort itself out
    delay(1500);
}

void loop()
{
    if (!(client.connected())) {
        reconnect();
    }

    // Wait a few seconds between measurements.
    if (millis() - prevMillis > interval) {
      prevMillis = millis();  
     
      // 讀取溫溼度
      int hum = dht.readHumidity();
      int temp = dht.readTemperature();
      float lux = lightMeter.readLightLevel();
      int soilsensor = analogRead(soilPin); //土壤濕度感測器的接腳，讀取數值
      float vwc = -0.0644 * soilsensor + 63.5;
      // 建立MQTT訊息（JSON格式的字串）
      msgStr = msgStr + "{\"temp\":" + temp + ",\"humid\":" + hum + ",\"light\":" + String(lux,1) +",\"vwc\":" + String(vwc,2) + "}";
      // 計算訊息長度 
      byte arrSize = msgStr.length() + 1;
      
      // 把String字串轉換成字元陣列格式
      char json[arrSize];
      msgStr.toCharArray(json, arrSize);
    
      // 發布MQTT主題與訊息
      client.publish(publishTopic, json);
      Serial.println(msgStr);
      msgStr = "";
    }   
}