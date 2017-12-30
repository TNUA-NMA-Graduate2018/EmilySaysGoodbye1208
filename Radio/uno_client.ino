boolean modeChange = 1; //0 =selfs  1=other

const int modeChanging = 3;

const int RelayR1 = 4;
const int RelayR2 = 5;
const int RelayR3 = 6;
const int RelayR4 = 7;

const int RelayL1 = 8;
const int RelayL2 = 9;
const int RelayL3 = 10;
const int RelayL4 = 11;

const int resR = A0;
const int resL = A1;

const int testHigh = 100;
const int testLow = 150;

int FromOtherR = 0;
int FromOtherL = 0;

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(RelayR1, OUTPUT);
  pinMode(RelayR2, OUTPUT);
  pinMode(RelayR3, OUTPUT);
  pinMode(RelayR4, OUTPUT);
  pinMode(RelayL1, OUTPUT);
  pinMode(RelayL2, OUTPUT);
  pinMode(RelayL3, OUTPUT);
  pinMode(RelayL4, OUTPUT);

  pinMode(resR, INPUT);
  pinMode(resL, INPUT);
  pinMode(modeChanging, INPUT);
}

void loop() {
  modeChange = digitalRead(modeChanging);

  if (!modeChange) {
    sliderControlSelf(resR, resL);
  }
  else {
    sliderControlByOther(FromOtherR, FromOtherL);
  }
  ConnectCheck();
}


void ConnectCheck() {
  return;
}


void sliderControlByOther(int FromOtherR, int FromOtherL) {
  Rgoahead(FromOtherR);
  Rgoback(FromOtherR);
  Rnomove(FromOtherR);

  Lgoahead(FromOtherL);
  Lgoback(FromOtherL);
  Lnomove(FromOtherL);
}


void sliderControlSelf(int resValueR, int resValueL) {
  Rgoahead(resValueR);
  Rgoback(resValueR);
  Rnomove(resValueR);

  Lgoahead(resValueL);
  Lgoback(resValueL);
  Lnomove(resValueL);
}

void Rgoahead(int valuein) {
  if (valuein > testHigh) {
    digitalWrite(RelayR1, LOW);
    digitalWrite(RelayR2, HIGH);
    digitalWrite(RelayR3, HIGH);
    digitalWrite(RelayR4, LOW);
  }
}

void Rgoback(int valuein) {
  if (valuein < testLow) {
    digitalWrite(RelayR1, HIGH);
    digitalWrite(RelayR2, LOW);
    digitalWrite(RelayR3, LOW);
    digitalWrite(RelayR4, HIGH);
  }
}

void Rnomove(int valuein) {
  if (valuein < testHigh && valuein > testLow){
  digitalWrite(RelayR1, HIGH);
    digitalWrite(RelayR2, HIGH);
    digitalWrite(RelayR3, HIGH);
    digitalWrite(RelayR4, HIGH);
  }
}


void Lgoahead(int valuein) {
  if (valuein > testHigh) {
    digitalWrite(RelayL1, LOW);
    digitalWrite(RelayL2, HIGH);
    digitalWrite(RelayL3, HIGH);
    digitalWrite(RelayL4, LOW);
  }
}

void Lgoback(int valuein) {
  if (valuein < testLow) {
    digitalWrite(RelayL1, HIGH);
    digitalWrite(RelayL2, LOW);
    digitalWrite(RelayL3, LOW);
    digitalWrite(RelayL4, HIGH);
  }
}

void Lnomove(int valuein) {
  if (valuein < testHigh && valuein > testLow) {
    digitalWrite(RelayL1, HIGH);
    digitalWrite(RelayL2, HIGH);
    digitalWrite(RelayL3, HIGH);
    digitalWrite(RelayL4, HIGH);
  }
}
