/*
    This sketch sends a message to a TCP server

*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;

const int readSlider1 = A0;

boolean modeChange = 1; //0 =selfs  1=other
//const int modeChanging = D3;
const int Ot2 = 4;
const int Ot4 = D5;
const int Ot7 = D6;
const int Ot8 = D7;

const int testHigh = 155;
const int testLow = 130;

int FromOther = 0;
int ToOther = 0;
void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(readSlider1, INPUT);
  //pinMode(modeChanging, INPUT);


  pinMode(Ot2, OUTPUT);
  pinMode(Ot4, OUTPUT);
  pinMode(Ot7, OUTPUT);
  pinMode(Ot8, OUTPUT);

  // We start by connecting to a WiFi network
  WiFiMulti.addAP("dlink-DC88", "ydeok89348");
  //const char* ssid     = "dlink-DC88";
  //const char* password = "ydeok89348";
  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(50);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}


void loop() {
  //modeChange = digitalRead(modeChanging);

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
    delay(50);
    return;
  }

  client.print(sendToOther);
  String req;
  while(client.available()==0){
    //Serial.println("Wait");
  }
    
  req = client.readStringUntil('\r');
  int val;
  val = req.toInt();
  FromOther = val;
  Serial.println(req);
  //digitalWrite(D7, val);
  // This will send the request to the server

  //client.flush();

  //client.stop();

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
  delay(100);
  return value ;
}
