#include <ArduinoJson.h>

// 定義溫室數據的結構
struct GreenhouseData {
  struct SensorData {
    const char* meter;
    float value;
    const char* unit;
  } sensor[2];  // 這裡假設每個溫室有兩個sensor
  struct ControllerData {
    struct LightData {
      int status;
      float setpoint;
    } light;
    struct FanData {
      int status;
      float setpoint;
    } fan;
  } controller;
  const char* timestamp;
};

// 函數用於將溫室數據轉換為 JSON 格式
void convertToJson(const GreenhouseData& data, JsonDocument& doc) {
  JsonObject greenhouse = doc.createNestedObject("greenhouse");
  JsonArray sensorArray = greenhouse.createNestedArray("sensor");
  
  // 添加 sensor 數據
  for (int i = 0; i < 2; ++i) {
    JsonObject sensor = sensorArray.createNestedObject();
    sensor["meter"] = data.sensor[i].meter;
    sensor["value"] = data.sensor[i].value;
    sensor["unit"] = data.sensor[i].unit;
  }

  // 添加 controller 數據
  JsonObject controller = greenhouse.createNestedObject("controller");
  controller["light"]["status"] = data.controller.light.status;
  controller["light"]["setpoint"] = data.controller.light.setpoint;
  controller["fan"]["status"] = data.controller.fan.status;
  controller["fan"]["setpoint"] = data.controller.fan.setpoint;

  greenhouse["timestamp"] = data.timestamp;
}

void setup() {
  Serial.begin(9600);

  // 創建 JSON 文檔
  StaticJsonDocument<1000> doc;

  // 初始化溫室數據
  GreenhouseData greenhouse_A = {
    .sensor = { 
      {"temperature", 25.5, "Celsius"},
      {"light", 50, "PPFD"}
    },
    .controller = {
      .light = {1, 60},
      .fan = {0, 26.0}
    },
    .timestamp = "2024-04-27T08:00:00"
  };

  // 將溫室數據轉換為 JSON
  convertToJson(greenhouse_A, doc);

  // 將 JSON 輸出到串口
  serializeJson(doc, Serial);
}

void loop() {
  // 這裡什麼也不做
}
