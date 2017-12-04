/*
  HMC5883L Triple Axis Digital Compass. Output for HMC5883L_processing.pde
  Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-magnetometr-hmc5883l.html
  GIT: https://github.com/jarzebski/Arduino-HMC5883L
  Web: http://www.jarzebski.pl
  (c) 2014 by Korneliusz Jarzebski
*/
#include <ESP8266WiFi.h>
#include<Arduino.h>

#include <Wire.h>
#include <HMC5883L.h> 

HMC5883L compass;
int previousDegree;
int returnDegree;

int sli1 = 0;
int sli2 = 0;
int value1;
int value2;
const int readSlider1 = A0;
//const int readSlider2 = A1;

void setup() {
  Serial.begin(9600);
  
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
  returnDegree = detectDegree();
  slidercontrol();
}

int detectDegree(){

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
  float headingDegrees = heading * 180/M_PI; 

  // Fix HMC5883L issue with angles
  float fixedHeadingDegrees;
 
  if (headingDegrees >= 1 && headingDegrees < 240)
  {
    fixedHeadingDegrees = map(headingDegrees, 0, 239, 0, 179);
  } else
  if (headingDegrees >= 240)
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



void slidercontrol(){
  int slidervalue1 = slider(readSlider1);
  int slidervalue2 = slider(readSlider2);

  if (slidervalue1 > 50) {
    digitalWrite(8, LOW);
    forward(abs(slidervalue1),11,10);
    delay(200);
  }

  else if (slidervalue1 < 10) {
    digitalWrite(8, LOW);
    backward(abs(slidervalue1),11,10);
     delay(200);
  }

  else {
    digitalWrite(8, HIGH);
    motorstop(11,10);
  }
  
  if (slidervalue2 > 50) {
    digitalWrite(7, LOW);
    forward(abs(slidervalue2),6,5);
    delay(200);
  }
  else if (slidervalue2 < 10) {
    digitalWrite(7, LOW);
    backward(abs(slidervalue2),6,5);
     delay(200);
  }
  else {
    digitalWrite(7, HIGH);
    motorstop(6,5);
  }
}

int slider(int slider) {
  sli1 = analogRead(slider);
  value1 = int(map(sli1, 0, 1024, -255, 255));

  return value1 ;
}

void motorstop(const int x,const int y)
{
  digitalWrite(x, LOW);
  digitalWrite(y, LOW);
}

void forward(int gospeed,const int x,const int y)
{
  digitalWrite(x, gospeed);
  digitalWrite(y, 0);
}

void backward(int backspeed,const int x,const int y)
{
  digitalWrite(x, 0);
  digitalWrite(y, backspeed);
}
