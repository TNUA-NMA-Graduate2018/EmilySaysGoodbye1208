/*
 *	This sketch sends a message to a TCP server
 *	This is the sketch of the Client
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include<Arduino.h>
#include <Wire.h>
#include <HMC5883L.h>

ESP8266WiFiMulti WiFiMulti;

HMC5883L compass;
int previousDegree;
const int readSlider1 = A0;
const int readSlider2 = A1;
boolean modeChange = 0; //0 =selfs  1=other

// ************************可變電阻值***********************
const int topValue = 50;
const int lowValue = 10;
// ************************可變電阻值***********************

WiFiClient client;


void setup() {
  Serial.begin(115200);
  delay(10);

  client.setNoDelay(1);
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

  pinMode(readSlider1, INPUT);
  pinMode(readSlider2, INPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);//繼電器

  while (!compass.begin())
  {
    delay(500);
  }

  // Set measurement range
  compass.setRange(HMC5883L_RANGE_1_3GA);

  // Set measurement mode
  compass.setMeasurementMode(HMC5883L_CONTINOUS);

  // Set data rate
  compass.setDataRate(HMC5883L_DATARATE_30HZ);

  // Set number of samples averaged
  compass.setSamples(HMC5883L_SAMPLES_8);

  // Set calibration offset. See HMC5883L_calibration.ino
  compass.setOffset(0, 0);

}
void loop() {


  int leftFromOther;
  int rightFromOther;
  int leftToOther;
  int RightToOther;

  int returnDegree = detectDegree();//頭部方位數值

  if (!modeChange) {
    sliderControlSelf();
  }
  else {
    leftToOther = slider(readSlider1);//leftToOther 是要送出去給對方的左邊輪子
    RightToOther = slider(readSlider2);
    sliderControlByOther(leftFromOther, rightFromOther);//接收對方左右輪數值，控制自己馬達
  }
  internetCheck();
}
void internetCheck() {
  const uint16_t port = 80;
  const char * host = "192.168.0.101"; // ip or dns

  Serial.print("connecting to ");
  Serial.println(host);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // Use WiFiClient class to create TCP connections

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
  digitalWrite(7, val);
  // This will send the request to the server

  char n1 = leftToOther;
  char n2 = RightToOther;

  client.print(n1);
  client.print(n2);
  client.flush();

  delay(50);

  //client.stop();
}
int detectDegree() {

  long x = micros();
  Vector norm = compass.readNormalize();

  // Calculate heading
  float heading = atan2(norm.YAxis, norm.XAxis);

  // Set declination angle on your location and fix heading
  // You can find your declination on: http://magnetic-declination.com/
  // (+) Positive or (-) for negative
  // For Bytom / Poland declination angle is 4'26E (positive)
  // Formula: (deg + (min / 60.0)) / (180 / M_PI);
  float declinationAngle = (4.0 + (26.0 / 60.0)) / (180 / M_PI);
  heading += declinationAngle;

  // Correct for heading < 0deg and heading > 360deg
  if (heading < 0)
  {
    heading += 2 * PI;
  }

  if (heading > 2 * PI)
  {
    heading -= 2 * PI;
  }

  // Convert to degrees
  float headingDegrees = heading * 180 / M_PI;

  // Fix HMC5883L issue with angles
  float fixedHeadingDegrees;

  if (headingDegrees >= 1 && headingDegrees < 240)
  {
    fixedHeadingDegrees = map(headingDegrees, 0, 239, 0, 179);
  } else if (headingDegrees >= 240)
  {
    fixedHeadingDegrees = map(headingDegrees, 240, 360, 180, 360);
  }

  // Smooth angles rotation for +/- 3deg
  int smoothHeadingDegrees = round(fixedHeadingDegrees);

  if (smoothHeadingDegrees < (previousDegree + 3) && smoothHeadingDegrees > (previousDegree - 3))
  {
    smoothHeadingDegrees = previousDegree;
  }

  previousDegree = smoothHeadingDegrees;


  Serial.print(norm.XAxis);
  Serial.print(":");
  Serial.print(norm.YAxis);
  Serial.print(":");
  Serial.print(norm.ZAxis);
  Serial.print(":");
  Serial.print(headingDegrees);
  Serial.print(":");
  Serial.print(fixedHeadingDegrees);
  Serial.print(":");
  Serial.print(smoothHeadingDegrees);
  Serial.println();
  return smoothHeadingDegrees;
  // One loop: ~5ms @ 115200 serial.
  // We need delay ~28ms for allow data rate 30Hz (~33ms)
  delay(30);
}


void sliderControlByOther(int leftFromOther, int rightFromOther) {
  int sliderValue1 = (int)map(leftFromOther, 0, 255, -255, 255);
  int sliderValue2 = (int)map(rightFromOther, 0, 255, -255, 255);

  if (sliderValue1 > topValue) {
    digitalWrite(8, LOW);
    forward(abs(sliderValue1), 11, 10);
    delay(50);
  }

  else if (sliderValue1 < lowValue) {
    digitalWrite(8, LOW);
    backward(abs(sliderValue1), 11, 10);
    delay(50);
  }

  else {
    digitalWrite(8, HIGH);
    motorstop(11, 10);
  }

  if (sliderValue2 > topValue) {
    digitalWrite(7, LOW);
    forward(abs(sliderValue2), 6, 5);
    delay(50);
  }
  else if (sliderValue2 < lowValue) {
    digitalWrite(7, LOW);
    backward(abs(sliderValue2), 6, 5);
    delay(50);
  }
  else {
    digitalWrite(7, HIGH);
    motorstop(6, 5);
  }
}
void sliderControlSelf() {
  int sliderValue1 = (int)map(slider(readSlider1), 0, 255, -255, 255);
  int sliderValue2 = (int)map(slider(readSlider2), 0, 255, -255, 255);

  if (sliderValue1 > topValue) {
    digitalWrite(8, LOW);
    forward(abs(sliderValue1), 11, 10);
    delay(50);
  }

  else if (sliderValue1 < lowValue) {
    digitalWrite(8, LOW);
    backward(abs(sliderValue1), 11, 10);
    delay(50);
  }

  else {
    digitalWrite(8, HIGH);
    motorstop(11, 10);
  }

  if (sliderValue2 > topValue) {
    digitalWrite(7, LOW);
    forward(abs(sliderValue2), 6, 5);
    delay(50);
  }
  else if (sliderValue2 < lowValue) {
    digitalWrite(7, LOW);
    backward(abs(sliderValue2), 6, 5);
    delay(50);
  }
  else {
    digitalWrite(7, HIGH);
    motorstop(6, 5);
  }
}

int slider(int slider) {
  int sli = analogRead(slider);
  int value = int(map(sli, 0, 1024, 0, 255));
  return value;
}

void motorstop(const int x, const int y)
{
  digitalWrite(x, LOW);
  digitalWrite(y, LOW);
}

void forward(int gospeed, const int x, const int y)
{
  digitalWrite(x, gospeed);
  digitalWrite(y, 0);
}

void backward(int backspeed, const int x, const int y)
{
  digitalWrite(x, 0);
  digitalWrite(y, backspeed);
}
