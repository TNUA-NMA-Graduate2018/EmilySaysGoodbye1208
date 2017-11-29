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


const int motorIn1 = 11;
const int motorIn2 = 10;
const int slider1 = A0;

int sli1 = 0;
int value1;

void setup()
{
  Serial.begin(9600);
  pinMode(slider1, INPUT);
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop()
{
  int slidervalue = slider();

  if (slidervalue > 30) {
    digitalWrite(8, LOW);
    forward(abs(slidervalue));
    delay(200);
  }

  else if (slidervalue < -30) {
    digitalWrite(8, LOW);
    backward(abs(slidervalue));
     delay(200);
  }

  else {
    digitalWrite(8, HIGH);
    motorstop();
  }

}


int slider() {
  sli1 = analogRead(slider1);
  value1 = int(map(sli1, 0, 1024, -255, 255));

  return value1 ;
}

void motorstop()
{
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
}

void forward(int gospeed)
{
  digitalWrite(motorIn1, gospeed);
  digitalWrite(motorIn2, 0);
}

void backward(int backspeed)
{
  digitalWrite(motorIn1, 0);
  digitalWrite(motorIn2, backspeed);
}
