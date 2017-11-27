const int slider1 = A0;
  int sli1 = 0;
  int value1; 

void setup() {
  pinMode(slider1, INPUT);
  Serial.begin(9600);
}

void loop() {
  int s;
  s = slider();
}

int slider() {
  sli1 = analogRead(slider1);
  value1 = int(map(sli1, 0, 1024, -100, 100));

  return value1 ;
}

