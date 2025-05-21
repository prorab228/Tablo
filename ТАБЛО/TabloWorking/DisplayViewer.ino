#define DisplayCount 11

bool bitData[100]; //88



static inline void latchPinH() {
  bitSet(PORTB, 1); //D9     //0 -d8
}
static inline void latchPinL() {
  bitClear(PORTB, 1); //D9     //0 - d8
}
static inline void clockPinH() {
  bitSet(PORTB, 2); //D10
}
static inline void clockPinL() {
  bitClear(PORTB, 2); //D10
}
static inline void dataPinH() {
  bitSet(PORTB, 3); //D11
}
static inline void dataPinL() {
  bitClear(PORTB, 3); //D11
}



int latchPin = 9;   //b2          8 - B0  12
int clockPin = 10;  // B4
int dataPin = 11;   // B3

void DisplayVieverSetup()
{ 
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}
void RebootDisplay() /
{
 // digitalWrite(latchPin,0);
  latchPinL();
 // digitalWrite(clockPin,0); 
//digitalWrite(dataPin,0); 
}


void SetNumberToDisplay(int number, int displayNumberL,bool AddDot)
{
  //bool AddDot = true;
  //Так-как наши экраны могут всего в 2 разряда, перевести число в цифру не сложно
  int firstDigit = number/10;
  int secondDigit = number%10;

  //т.к. индикаторы расположены в странном порядке, и передача идет наоборот приходится костылить
  //указываем какой индикатор надо использовать для первой и второй цифры
  int firstDigitNumber = 0; //Номер индикатора для первой цифры
  int secondDigitNumber = 0;//Номер индикатора для второй цифры
  switch (displayNumberL)
  {
    case 1: 
      firstDigitNumber = 11;
      secondDigitNumber = 10;
      break;
    case 2: 
      firstDigitNumber = -1; //Индикатор с одной секцией
      secondDigitNumber = 9;
      break;
    case 3: 
      firstDigitNumber = 8;
      secondDigitNumber = 7;
      break;
    case 4: 
      firstDigitNumber = -1; //Индикатор с одной секцией
      secondDigitNumber = 6;
      break;
    case 5: 
      firstDigitNumber = 5;
      secondDigitNumber = 4;
      break;
    case 6: 
      firstDigitNumber = 3;
      secondDigitNumber = 2;
      break;
    case 7: 
      firstDigitNumber = -1; //Индикатор с одной секцией
      secondDigitNumber = 1;
      break;
    default: 
      
      break;
  }
 /* Serial.print(firstDigitNumber);
  Serial.print('\t');
  Serial.print(secondDigitNumber);
  Serial.print('\t');
  Serial.print(displayNumberL);
  Serial.print('\n');*/
  

  if(firstDigit>0 && firstDigitNumber>=0) setData(numbers[firstDigit], firstDigitNumber);
  if(displayNumberL == 5 || displayNumberL == 6 && firstDigitNumber>=0) setData(numbers[firstDigit], firstDigitNumber); // костыль на часы
  
  
  if(!AddDot) setData(numbers[secondDigit], secondDigitNumber);
  else setData(numbers[10] + numbers[secondDigit], secondDigitNumber); // двоеточие на часах управляется самым первым битов, просто прибавляем её

}

void setData(byte number, int num)
{
  for(int i = 0; i<8; i++)
  {
    if (number & (0B10000000 >> i)) bitData[num*8-i] = 1;
    else bitData[num*8-i] = 0;    
  }
 // ShowBitData();
}

void clearData()
{
  for(int i = 0; i<100; i++)
  {
    bitData[i] = 0;    
  }
  //ShowBitData(); 
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

 // latchPinL();
  clockPinL();
  dataPinL();
  //

  for (int i = 0; i < 88; i++) {
    if (bitData[i]) dataPinH();
    else dataPinL();

    clockPinH();
    dataPinL();
    clockPinL();
  }

  latchPinH();
  
}
