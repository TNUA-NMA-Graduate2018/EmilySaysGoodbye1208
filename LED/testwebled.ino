
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(300, D7, NEO_GRB + NEO_KHZ800);

int value = 1;
int valueold = 0;
WiFiServer server(80);

void setup(){
 Serial.begin(115200);
  delay(10);
  
  
  strip.begin();
  strip.show();
  
  //const char* ssid     = "dlink-DC88";
  //const char* password = "ydeok89348";
  Serial.println();
  Serial.print("Connecting to ");
  WiFi.begin("dlink-DC88", "ydeok89348");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  Serial.print("Use this URL :");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  delay(500);
  
  }
void loop(){
  LedChangeOhyeah (value);
  WiFiClient client=server.available();
  if (!client){
    return;
  }
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  String request =client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  value = LOW;
  if (request.indexOf("/LED=one") != -1) {
   LedChangeOhyeah (1);
   value=1;
  } 
  if (request.indexOf("/LED=two") != -1){
    LedChangeOhyeah (2);
    value = 2;
  }
    if (request.indexOf("/LED=three") != -1){
    LedChangeOhyeah (3);
    value = 3;
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.print("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("Led pin is now: ");
 
  if(value == 1) {
    client.print("one");  
  } else if(value==2){
    client.print("two");
  }
  else if(value==3){
    client.print("three");
    }
    else{
      client.print("one");
    }
  client.println("<br><br>");
  client.println("Click <a href=\"/LED=one\">here</a> turn the LED pin D7 to mode one<br>");
  client.println("Click <a href=\"/LED=two\">here</a> turn the LED  pin D7 to mode two<br>");
  client.println("Click <a href=\"/LED=three\">here</a> turn the LED  pin D7 to mode three<br>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
  }


void   LedChangeOhyeah (int num) {
      if (num == 1) {
        theaterChase(50, 5);
        
      }
      else if (num == 2) {
        breath(20, 80);
        strip.show();
        
      }
      else if (num == 3) {
        rainbowCycle(50);
        
      }
      if (value != valueold) {
        valueold = value;
        for (int n = 0; n < strip.numPixels(); n++) {
          strip.setPixelColor(n, strip.Color(0, 0, 0));
        }
        strip.show();
      }
    }
    void theaterChase(int c, uint8_t wait) {
      for (int j = 0; j < 1; j++) { //do 10 cycles of chasing
        for (int q = 0; q < 2; q++) {
          for (int i = 0; i < strip.numPixels() + 20; i = i + 10) {
            for (int a = 0; a < 10; a++) {
              strip.setPixelColor(i + q - a, strip.Color(c - (5 * (a + 1)), c - (5 * (a + 1)), c - (5 * (a + 1))));
              strip.setPixelColor(q - 9 - a, strip.Color(0, 0, 0));
              strip.setPixelColor(i + q - a, strip.Color(c - (5 * (a + 1)), c - (5 * (a + 1)), c - (5 * (a + 1))));
              strip.setPixelColor(i + q - 9 - a, strip.Color(0, 0, 0));
              delay(wait);
            }//turn every third pixel on

            strip.show();


          }
        }
      }
    }
    void breath(int c, uint8_t wait) {
      int i, j;

      for (j = 0; j < 21; j++) {
        for (i = 0; i < strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(c - j, c - j, c - j));
        }
        strip.show();
        delay(wait);
      }
      for (j = 0; j < 21; j++) {
        for (i = 0; i < strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(j, j, j));
        }
        strip.show();
        delay(wait);
      }
    }

    void rainbowCycle(uint8_t wait) {
      uint16_t i, j;

      for (j = 0; j < 128; j++) { // 5 cycles of all colors on wheel
        for (i = 0; i < strip.numPixels(); i++) {
          strip.setPixelColor(i, Wheel(((i * 255 / strip.numPixels()) + j ) & 127));
        }
        strip.show();
        //delay(wait);
      }
    }


    uint32_t Wheel(byte WheelPos) {
      if (WheelPos < 42) {
        return strip.Color(WheelPos * 3, 127 - WheelPos * 3, 0);
      } else if (WheelPos < 85) {
        WheelPos -= 42;
        return strip.Color(127 - WheelPos * 3, 0, WheelPos * 3);
      } else {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 127 - WheelPos * 3);
      }
    }

