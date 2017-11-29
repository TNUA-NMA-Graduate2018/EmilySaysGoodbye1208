#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel stripB;
Adafruit_NeoPixel stripA;

boolean WinBlue = false;
boolean WinRed = false;
boolean flagBlue = false, preflagBlue = false;
boolean flagRed = false, preflagRed = false;
int LineA[240];
int LineB[240];
int LineA_Start , LineA_End;
int LineB_Start , LineB_End;
int light;
void setup() {
  stripA = Adafruit_NeoPixel(120, 8, NEO_GRB + NEO_KHZ800);
  stripB = Adafruit_NeoPixel(120, 7, NEO_GRB + NEO_KHZ800);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  stripA.begin();
  stripB.begin();
  stripA.show();
  stripB.show();

  for (int i = 0; i < 60; i++)LineA[i] = i;
  for (int i = 0; i < 60; i++)LineB[i] = 60 - i;
}

void loop() {

  digitalWrite(13, OUTPUT);
  digitalWrite(12, OUTPUT);
  WhiteLight(10);

  detectWinRed();
  if (WinRed == true) {
    colorWipe(stripA.Color(65, 0, 0), 15);
    //randomlight(stripA.Color(80, 0, 0), 35);
    theaterChase(stripA.Color(65, 0, 0), 35);
    WinRed = false;
    delay(1000);
  }

  detectWinBlue();
  if (WinBlue == true) {
    colorWipeBack(stripA.Color(0, 0, 100), 15);
    //randomlight(stripA.Color(0, 0, 80), 35);
    theaterChaseBack(stripA.Color(0, 0, 100), 35);
    //rainbow(30);
    WinBlue = false;
    delay(1000);
  }

}


void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < 120; i++) {
    stripA.setPixelColor(i, c);
    stripB.setPixelColor(i, c);
    stripA.show();
    stripB.show();
    delay(wait);
  }
}
void colorWipeBack(uint32_t c, uint8_t wait) {
  for (uint16_t i = 119; i >= 0 ; i--) {
    if (i == 65535)break;
    stripA.setPixelColor(i, c);
    stripB.setPixelColor(i, c);
    stripA.show();
    stripB.show();
    delay(wait);
  }
}
void WhiteLight(uint8_t wait) {
  for (uint16_t i = 0; i < 60; i++) {
    LineA[i]++;
    if (LineA[i] >= 120)LineA[i] = 0;
    int colorPoint = (int)map(i, 0, 60, 10, 35);
    int flag;
    uint32_t c = stripA.Color(colorPoint, colorPoint, colorPoint);
    if (flag < 120)flag = LineA[i];
    else {
      flag = LineA[i] - 120;
    }
    stripA.setPixelColor(flag, c);
  }
  for (uint16_t i = 0; i < 60; i++) {
    LineB[i]--;
    if (LineB[i] <= 0)LineB[i] = 119;
    int colorPoint = (int)map(i, 0, 60, 10, 35);
    int flag;
    uint32_t c = stripA.Color(colorPoint, colorPoint, colorPoint);

    if (flag < 120)flag = LineB[i];
    else {
      flag = LineB[i] - 120;
    }
    stripB.setPixelColor(flag, c);

  }
  stripA.show();
  stripB.show();
  delay(wait);
}
void detectWinRed() {
  int lightRed = analogRead(3);
  //  Serial.print("lightRed");
  //  Serial.println(lightRed);
  if (lightRed > 700) {
    flagRed = true;
  } else {
    flagRed = false;
  }
  if (flagRed != preflagRed) {
    if (flagRed) {
      Serial.write('1');
      WinRed = true;
    }
  }
  preflagRed = flagRed;
}
void detectWinBlue() {
  int lightBlue = analogRead(5);
  Serial.print("lightBlue");
  Serial.println(lightBlue);
  if (lightBlue > 700) {
    flagBlue = true;
  } else {
    flagBlue = false;
  }
  if (flagBlue != preflagBlue) {
    if (flagBlue) {
      Serial.write('2');
      WinBlue = true;
    }
  }
  preflagBlue = flagBlue;
}
int Add(int x) {
  if (x + 1 >= 240)return 0;
  else return x + 1;
}
void randomlight(uint32_t c, uint8_t wait) {
  for (int i = 0; i < 20; i++) {
    for (int s = 0; s < 30; s++) {
      i = (int)random(120);
      stripA.setPixelColor(i, c);
      stripB.setPixelColor(i, c);
    }
    stripA.show();
    stripB.show();
    delay(wait);
  }
}
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) {
    for (int q = 0; q < 3; q++) {
      for (int i = 0; i < 120; i = i + 3) {
        stripA.setPixelColor(i + q, c);
        stripB.setPixelColor(i + q, c);
      }
      stripA.show();
      stripB.show();
      delay(wait);

      for (int i = 0; i < 120; i = i + 3) {
        stripA.setPixelColor(i + q, 0);
        stripB.setPixelColor(i + q, 0);
      }
    }
  }
}

void theaterChaseBack(uint32_t c, uint8_t wait) {
  //Serial.println("in");
  for (int j = 0; j < 10; j++) {
    for (int q = 0; q < 3; q++) {
      for (int i = 119; i >= 0; i = i - 3) {
        stripA.setPixelColor(i - q, c);
        stripB.setPixelColor(i - q, c);
      }
      stripA.show();
      stripB.show();
      delay(wait);

      for (int i = 119; i >= 0; i = i - 3) {
        stripA.setPixelColor(i - q, 0);
        stripB.setPixelColor(i - q, 0);
      }
    }
  }
}
