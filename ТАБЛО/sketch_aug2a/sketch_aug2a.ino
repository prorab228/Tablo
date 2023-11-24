//#include <bitset>
#define DisplayCount 11

bool bitData[88];

long oldMillis;

static inline void latchPinH() {
  bitSet(PORTB, 0);
}
static inline void latchPinL() {
  bitClear(PORTB, 0);
}
static inline void clockPinH() {
  bitSet(PORTB, 4);
}
static inline void clockPinL() {
  bitClear(PORTB, 4);
}
static inline void dataPinH() {
  bitSet(PORTB, 3);
}
static inline void dataPinL() {
  bitClear(PORTB, 3);
}


int latchPin = 8;   // B0
int clockPin = 12;  // B4
int dataPin = 11;   // B3

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  Serial.begin(9600);
}

byte numbers[11] = {
  0b01111110,  //0
  0b00010100,  //1
  0b01011011,  //2
  0b01010111,  //3
  0b00110101,  //4
  0b01100111,  //5
  0b01101111,  //6
  0b01010100,  //7
  0b01111111,  //8
  0b01110111,   //9
  0b10000000   //:
};

void loop() {
  
  for(int i=1;i<=11;i++)
  {  
    setData(numbers[i%10], i);
    setData(numbers[i%10], 11-i);
    Show();
    delay(400);
    clearData();
   // Show();
   // delay(100);
  }
//  showTime();
  delay(400);

}

void showTime()
{
    setData(numbers[10] + numbers[3], 2);
    setData(numbers[4], 3);
    //setData(4, 3);
    setData(numbers[4], 4);
    setData(numbers[4], 5);
    Show();
    
    clearData();
}

void setData(byte number, int num)
{
  for(int i = 0; i<8; i++)
  {
    if (number & (0B10000000 >> i)) bitData[num*8-i] = 1;
    else bitData[num*8-i] = 0;    
  }
  ShowBitData();
}

void clearData()
{
  for(int i = 0; i<88; i++)
  {
    bitData[i] = 0;    
  }
  ShowBitData();
}

void ShowBitData()
{
  for(int i=0; i<88;i++)
  {
    Serial.print(bitData[i]);
  }
  Serial.println();
}

void Show() {

  latchPinL();
  boolean pinState;
  clockPinL();
  dataPinL();

  for (int i = 0; i < 88; i++) {
    if (bitData[i]) dataPinH();
    clockPinH();
    dataPinL();
    clockPinL();
  }

  latchPinH();
}








  void myShiftOutH() {
    boolean pinState;
    clockPinL();
    dataPinL();
    for (int i = 0; i < 88; i++) {
      if (i % 2 == 0) dataPinH();
      if (i > 70) dataPinH();
      clockPinH();
      dataPinL();
      clockPinL();
      // delayMicroseconds(10);
      clockPinH();
      clockPinL();
      //  delayMicroseconds(10);
    }
  }

  void myShiftOutL() {
    boolean pinState;
    clockPinL();
    dataPinL();

    for (int i = 0; i < 88; i++) {

      dataPinL();
      clockPinH();
      clockPinL();
    }
  }


