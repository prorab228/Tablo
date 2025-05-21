#include <SoftwareSerial.h>
#include <Time.h>

#define rxPin 2
#define txPin 3

SoftwareSerial BTSerial(rxPin, txPin);

void setup() {

   pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
 // # 38400 - для метода №1, 9600 - для метода №2
  BTSerial.begin(9600);
  Serial.begin(9600);
  delay(500);
}

void loop() {
  if (BTSerial.available()) {
    Serial.write(BTSerial.read());
  }
  if (Serial.available()) {
    BTSerial.write(Serial.read());
  }
}