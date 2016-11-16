const byte dataA = 2;  //pin 1 on CN0/CN1
const byte dataB = 3;  //pin 2 on CN0/CN1
const byte dataC = 4;  //pin 3 on CN0/CN1

const byte G2AU38_ENABLE = 5;   //pin 4 on CN0/CN1
const byte G2AU37_ENABLE = 6;  //pin 5 on CN0/CN1
const byte G1_ENABLE = 7;  //pin 6 on CN0/CN1
const byte G2B_ENABLE = 8;  //pin 7 on CN0/CN1

const byte serialClock = 9;  //pin 8 on CN0/CN1
const byte serialData = 10;  //pin 9 on CN0/CN1

const int pixleLength = 85;  //how many pixles in serial string


void setup() {
  pinMode(dataA, OUTPUT); digitalWrite(dataA, LOW);  //active LOW
  pinMode(dataB, OUTPUT); digitalWrite(dataB, LOW);  //active LOW
  pinMode(dataC, OUTPUT); digitalWrite(dataC, LOW);  //active LOW

  pinMode(G2AU38_ENABLE, OUTPUT); digitalWrite(G2AU38_ENABLE, HIGH);  //active LOW top
  pinMode(G2AU37_ENABLE, OUTPUT); digitalWrite(G2AU37_ENABLE, HIGH);  //active LOW botom
  pinMode(G1_ENABLE, OUTPUT); digitalWrite(G1_ENABLE, HIGH);  //active HIGH
  pinMode(G2B_ENABLE, OUTPUT); digitalWrite(G2B_ENABLE, LOW);  //active LOW

  pinMode(serialData, OUTPUT); digitalWrite(serialData, HIGH);  //active HIGH
  pinMode(serialClock, OUTPUT); digitalWrite(serialClock, LOW);  //rising edge
  clearDisplay();
}

void loop() {
  digitalWrite(G2AU38_ENABLE, LOW);  //active LOW
  for (int i = 0; i < pixleLength; i++) {
    digitalWrite(serialData, HIGH);  //rising edge
    digitalWrite(serialClock, HIGH);  //rising edge
    delayMicroseconds(10);
    digitalWrite(serialClock, LOW);  //rising edge
    delay(50);
  }
  delay(1000);
  clearDisplay();
  delay(3000);
}

void clearDisplay() {
  digitalWrite(G2AU38_ENABLE, HIGH);  //active LOW
  digitalWrite(G2AU37_ENABLE, HIGH);  //active LOW
  digitalWrite(serialData, LOW);  //active HIGH
  digitalWrite(serialClock, LOW);  //active HIGH
  for (int i = 0; i < pixleLength; i++) {
    digitalWrite(serialClock, HIGH);  //rising edge
    delayMicroseconds(10);
    digitalWrite(serialClock, LOW);  //rising edge
    delayMicroseconds(10);
  }
}

