const int readSlider1 = A0;
const int testHigh = 155;
const int testLow = 130;

boolean modeChange = 0; //0 =selfs  1=other

void setup() {
  Serial.begin(9600);

  pinMode(readSlider1, INPUT);

  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

}

void loop() {
  int FromOther;
  int ToOther;

  if (!modeChange) {
    sliderControlSelf();
  }
  else {
    ToOther = slider(readSlider1);
    sliderControlByOther(FromOther);
  }

}

void sliderControlByOther(int FromOther) {
  int sliderValue1 = FromOther;

  if (sliderValue1 > testHigh) {
    digitalWrite(2, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
  }

  else if (sliderValue1 < testLow) {
    digitalWrite(2, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
  }

  else {
    digitalWrite(2, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
  }

}


void sliderControlSelf() {
  int slidervalue1 = slider(readSlider1);

  if (slidervalue1 > testHigh) {
    digitalWrite(2, LOW);
    digitalWrite(4, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
  }

  else if (slidervalue1 < testLow) {
    digitalWrite(2, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
  }

  else {
    digitalWrite(2, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
  }

}


int slider(int slider) {
  int sli = analogRead(slider);
  int value = int(map(sli, 0, 1024, 0, 255));
  Serial.println(value);
  return value ;
}

