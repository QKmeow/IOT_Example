#include <ArduinoJson.h>

void setup() {
  Serial.begin(9600);

  // 分配 JSON 文檔
  JsonDocument doc;

  // 在文檔中添加值
  doc["greenhouse_A"]["sensor"][0]["meter"] = "temperature";
  doc["greenhouse_A"]["sensor"][0]["value"] = 25.5;
  doc["greenhouse_A"]["sensor"][0]["unit"] = "Celsius";

  doc["greenhouse_A"]["sensor"][1]["meter"] = "light";
  doc["greenhouse_A"]["sensor"][1]["value"] = 50;
  doc["greenhouse_A"]["sensor"][1]["unit"] = "PPFD";

  doc["greenhouse_A"]["controller"]["light"]["status"] = 1;
  doc["greenhouse_A"]["controller"]["light"]["setpoint"] = 60;

  doc["greenhouse_A"]["controller"]["fan"]["status"] = 0;
  doc["greenhouse_A"]["controller"]["fan"]["setpoint"] = 26.0;

  doc["greenhouse_A"]["timestamp"] = "2024-04-27T08:00:00";

  doc["greenhouse_B"]["sensor"][0]["meter"] = "temperature";
  doc["greenhouse_B"]["sensor"][0]["value"] = 26.0;
  doc["greenhouse_B"]["sensor"][0]["unit"] = "Celsius";

  doc["greenhouse_B"]["sensor"][1]["meter"] = "light";
  doc["greenhouse_B"]["sensor"][1]["value"] = 45;
  doc["greenhouse_B"]["sensor"][1]["unit"] = "PPFD";

  doc["greenhouse_B"]["controller"]["light"]["status"] = 0;
  doc["greenhouse_B"]["controller"]["light"]["setpoint"] = 55;

  doc["greenhouse_B"]["controller"]["fan"]["status"] = 1;
  doc["greenhouse_B"]["controller"]["fan"]["setpoint"] = 25.5;

  doc["greenhouse_B"]["timestamp"] = "2024-04-27T08:00:00";

  // 將 JSON 輸出到串口
  serializeJson(doc, Serial);
}

void loop() {
  // 這裡什麼也不做
}
