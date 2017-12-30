const int readSlider1 = A0;
const int readSlider2 = A1;

int ToOtherRight = 0;
int ToOtherLeft = 0;

void setup() {
  Serial.begin(115200);
  delay(10);
  
  pinMode(readSlider1, INPUT);
  pinMode(readSlider2, INPUT);
}

void loop() {
  ToOtherRight = slider(readSlider1);
  ToOtherLeft = slider(readSlider2);

  SendClient(ToOtherRight, ToOtherLeft);
}


void SendClient(int sendToOtherR, int sendToOtherL) {
  // Send the response to the client
  Serial.print(String(sendToOtherR) + " " + String(sendToOtherL) + "\r");
  // client.flush();

  delay(50);
  Serial.println("Client disonnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}


int slider(int slider) {
  int sli = analogRead(slider);
  int value = int(map(sli, 0, 1024, 0, 255));
  Serial.println(value);
  delay(100);
  return value ;
}
