/*
 *  This sketch sends a message to a TCP server
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  WiFiMulti.addAP("dlink-DC88", "ydeok89348");
  //const char* ssid     = "dlink-DC88";
  //const char* password = "ydeok89348";
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
}


void loop() {
  const uint16_t port = 80;
  const char * host = "192.168.0.101"; // ip or dns

  Serial.print("connecting to ");
  Serial.println(host);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(5000);
    return;
  }
  String req = client.readStringUntil('\r');
  int val;
  val = int(req[0]);
  Serial.println(val);
  digitalWrite(D7, val);
  // This will send the request to the server

  char n = int(map(analogRead(A0), 0, 1024, 0, 255));
  client.print(n);
  client.flush();

  delay(300);
  
  //client.stop();
}
