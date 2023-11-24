#define DATA_PIN 11   // пин данных
#define LATCH_PIN 8  // пин защелки 11
#define CLOCK_PIN 12  // пин тактов синхронизации 12
#define ENABLE_PIN 8  // пин тактов синхронизации

//int latchPin = 8;   // B0
//int clockPin = 12;  // B4
//int dataPin = 11;   // B3

byte numbers[10] = {
  0b11111101,  //0
  0b00101000,  //1
  0b10110110,  //2
  0b10101110,  //3
  0b01101010,  //4
  0b11001110,  //5
  0b11011110,  //6
  0b10101000,  //7
  0b11111111,  //8
  0b11101111   //9
};




byte data[10] = {  // байты, который будут последовательно циклически выводиться в регистре
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};

void setup() {
  Serial.begin(9600);
  pinMode(DATA_PIN, OUTPUT);  // инициализация пинов
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(LATCH_PIN, ENABLE_PIN);
  digitalWrite(LATCH_PIN, HIGH);

  ClearDisplay();

  delay(5000);
}

void loop() {
  /*for(int i=0;i<12;i++)
  {
    test(i); 
    delay(4000);
  }*/

  /*digitalWrite(LATCH_PIN, LOW);               // "открываем защелку"
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0b10110110); // отправляем данные
  digitalWrite(LATCH_PIN, HIGH);              // "закрываем защелку", выходные ножки регистра установлены
delay(4000);
    digitalWrite(LATCH_PIN, LOW);               // "открываем защелку"
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0b00000000); // отправляем данные
  digitalWrite(LATCH_PIN, HIGH);              // "закрываем защелку", выходные ножки регистра установлены*/

  for (int i = 0; i < 10; i++) {
    clearData();
    setData(i, 0);
    setData(i, 1);
    setData(i, 2);
    setData(i, 3);
    setData(i, 4);
    setData(i, 5);
    setData(i, 6);
    setData(i, 7);
    setData(i, 8);
    setData(i, 9);
 
    
    delay(500);
    showTablo();
  }
  delay(4000);
  ClearDisplay();
  delay(400);
}

void setData(int number, int num)
{
  data[9-num] = numbers[number];
}

void showTablo() {
  digitalWrite(ENABLE_PIN, LOW); 
  
  for (int i = 0; i < 10; i++) {
    digitalWrite(LATCH_PIN, LOW);                         // "открываем защелку"
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, data[i]);  // отправляем данные
    digitalWrite(LATCH_PIN, HIGH);                        // "закрываем защелку", выходные ножки регистра установлены
    delay(500);
    Serial.println(data[i]);
  }
  Serial.println();
  digitalWrite(ENABLE_PIN, HIGH); 
}


void clearData()
{
    for (int i = 0; i < 12; i++) {
      data[i] = 0b00000000;
    }
}


void ClearDisplay() {
  for (int i = 0; i < 12; i++) {
    digitalWrite(LATCH_PIN, LOW);                         // "открываем защелку"
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0b00000000);  // отправляем данные
    digitalWrite(LATCH_PIN, HIGH);                        // "закрываем защелку", выходные ножки регистра установлены
    delay(5);
  }
}

void test(int x) {
  Serial.println(x);
  byte zero1[12] = { // байты, который будут последовательно циклически выводиться в регистре
                     0b00000000,
                     0b00000000,
                     0b00000000,
                     0b00000000,
                     0b00000000,
                     0b00000000,
                     0b00000000,
                     0b00000000,
                     0b00000000,
                     0b00000000,
                     0b00000000,
                     0b00000000
  };


  zero1[x] = 0b10110110;

  for (int i = 0; i < 12; i++) {
    out_595_shift(zero1[i]);
    Serial.println(zero1[i]);
  }

  Serial.println();
}


void out_595_shift(byte x) {
  digitalWrite(LATCH_PIN, LOW);                         // "открываем защелку"
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, 0b10110110);  // отправляем данные
  digitalWrite(LATCH_PIN, HIGH);                        // "закрываем защелку", выходные ножки регистра установлены
  delay(10);
}

void out_shift(byte x[]) {
  digitalWrite(LATCH_PIN, LOW);  // "открываем защелку"
  for (int i = 0; i < 12; i++) {
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, x[i]);  // отправляем данные
    digitalWrite(LATCH_PIN, HIGH);
    delay(500);
  }
  digitalWrite(LATCH_PIN, HIGH);  // "закрываем защелку", выходные ножки регистра установлены
}