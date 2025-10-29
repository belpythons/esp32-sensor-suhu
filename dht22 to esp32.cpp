#include "DHTesp.h"

DHTesp dht;
const int dhtPin = 15; // GPIO15

void setup() {
  Serial.begin(115200);
  dht.setup(dhtPin, DHTesp::DHT22);
}

void loop() {
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  
  if (!isnan(humidity) && !isnan(temperature)) {
    Serial.print("Suhu: ");
    Serial.print(temperature);
    Serial.print("°C, Kelembaban: ");
    Serial.print(humidity);
    Serial.println("%");
  }
  
  delay(2000);
}