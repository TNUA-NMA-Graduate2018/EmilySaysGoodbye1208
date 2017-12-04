/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>

const char* ssid = "dlink-DC88";
const char* password = "ydeok89348";

WiFiServer server(80);
boolean modeChange = 0; //0 =selfs  1=other
const int modeChanging = D3;
const int readSlider1 = A0;
const int Ot2 = D4;
const int Ot4 = D5;
const int Ot7 = D6;
const int Ot8 = D7;

int testHigh = 155;
int testLow = 130;

void setup() {
  Serial.begin(115200);
  delay(10);


  pinMode(readSlider1, INPUT);
  pinMode(modeChanging, INPUT);

  pinMode(Ot2, OUTPUT);
  pinMode(Ot4, OUTPUT);
  pinMode(Ot7, OUTPUT);
  pinMode(Ot8, OUTPUT);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  int FromOther;
  int ToOther;
  modeChange = digitalRead(modeChanging);

  if (!modeChange) {
    sliderControlSelf();
  }
  else {
    ToOther = slider(readSlider1);
    sliderControlByOther(FromOther);
  }
  internetCheck(ToOther);
}
void internetCheck(int sendToOther) {

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  //Serial.println(req);
  client.flush();

  // Match the request
  int val;

  val = int(req[0]);
  Serial.println(val);

  // Set GPIO2 according to the request
  digitalWrite(D7, val);
  //delay(val + 50);

  // Send the response to the client
  client.print(sendToOther);
  client.flush();

  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}

void sliderControlByOther(int FromOther) {
  int sliderValue1 = FromOther;

  if (sliderValue1 > testHigh) {
    digitalWrite(Ot2, LOW);
    digitalWrite(Ot4, HIGH);
    digitalWrite(Ot7, HIGH);
    digitalWrite(Ot8, LOW);
  }

  else if (sliderValue1 < testLow) {
    digitalWrite(Ot2, HIGH);
    digitalWrite(Ot4, LOW);
    digitalWrite(Ot7, LOW);
    digitalWrite(Ot8, HIGH);
  }

  else {
    digitalWrite(Ot2, HIGH);
    digitalWrite(Ot4, HIGH);
    digitalWrite(Ot7, HIGH);
    digitalWrite(Ot8, HIGH);
  }

}


void sliderControlSelf() {
  int slidervalue1 = slider(readSlider1);

  if (slidervalue1 > testHigh) {
    digitalWrite(Ot2, LOW);
    digitalWrite(Ot4, HIGH);
    digitalWrite(Ot7, HIGH);
    digitalWrite(Ot8, LOW);
  }

  else if (slidervalue1 < testLow) {
    digitalWrite(Ot2, HIGH);
    digitalWrite(Ot4, LOW);
    digitalWrite(Ot7, LOW);
    digitalWrite(Ot8, HIGH);
  }

  else {
    digitalWrite(Ot2, HIGH);
    digitalWrite(Ot4, HIGH);
    digitalWrite(Ot7, HIGH);
    digitalWrite(Ot8, HIGH);
  }

}


int slider(int slider) {
  int sli = analogRead(slider);
  int value = int(map(sli, 0, 1024, 0, 255));
  Serial.println(value);
  return value ;
}

