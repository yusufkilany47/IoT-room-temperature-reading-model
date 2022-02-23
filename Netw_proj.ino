/*
    This sketch sends a string to a TCP server, and prints a one-line response.
    You must run a TCP server in your local network.
    For example, on Linux you can use this command: nc -v -l 3000
*/
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#ifndef STASSID
#define STASSID "Galaxy Note20"
#define STAPSK  "Kilany47"
#endif

#define DHTPIN 4
#define DHTTYPE DHT11   // DHT 11

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.163.137";
const uint16_t port = 3686;

ESP8266WiFiMulti WiFiMulti;
DHT dht(DHTPIN, DHTTYPE);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);

  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(5000);
    return;
  }

  // This will send the request to the server
  client.println("hello from ESP8266");

  //read back one line from server
  Serial.println("Sending data to remote server");
  String line = client.readStringUntil('\r');
  Serial.println(line);

  //Serial.println("closing connection");
  //client.println("close socket");

  //Serial.println("wait 5 sec...");
  //delay(5000);
}


void loop() {

  delay(5000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    client.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  client.print(F(" ")) ;
  client.print(F("Humidity: "));
  client.print(h);
  client.print(F("%  Temperature: "));
  client.print(t);
  client.print(F("°C "));
//  client.print(f);
//  client.print(F("°F  Heat index: "));
//  client.print(hic);
//  client.print(F("°C "));
//  client.print(hif);
//  client.println(F("°F"));
}
