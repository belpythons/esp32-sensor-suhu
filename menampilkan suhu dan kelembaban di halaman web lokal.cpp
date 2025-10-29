#include <WiFi.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Nama_WiFi";
const char* password = "Password_WiFi";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
  }

  Serial.println("WiFi terhubung.");
  server.begin();
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  String request = client.readStringUntil('\r');
  client.flush();

  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  client.println("<!DOCTYPE html><html>");
  client.println("<h1>Monitoring Suhu & Kelembaban</h1>");
  client.println("<p>Suhu: " + String(suhu) + " *C</p>");
  client.println("<p>Kelembaban: " + String(kelembaban) + " %</p>");
  client.println("</html>");
}
