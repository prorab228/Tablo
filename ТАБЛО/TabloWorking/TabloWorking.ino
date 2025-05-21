//#include <bitset>
#include <SoftwareSerial.h>
#define rxPin 2
#define txPin 3

#define ShowDelay 1
#define ClearDelay 10000
SoftwareSerial mySerial(rxPin, txPin);


bool OperatingMode = false;
bool debug = false;
bool debugTime = false;

bool Chronometer = false;
bool StartChronometer = false;
bool StartTimer = false;
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
  0b01110111,  //9
  0b10000000   //:
};

struct ScoreCounter {
public:
  int OwnerScore = 0;
  int VisitorScore = 0;

  int OwnerFoul = 0;
  int VisitorFoul = 0;

  int Period = 0;

  void ClearScore() {
    OwnerScore = 0;
    VisitorScore = 0;
  }

  void ClearFoul() {
    OwnerFoul = 0;
    VisitorFoul = 0;
  }
  void ClearPeriod()
  {
    Period = 0;
  }

  void ClearAll()
  {
    ClearScore();
    ClearFoul();
    ClearPeriod();
  }
} Score;


void PrintScore() {
  
  SetNumberToDisplay(Score.OwnerScore, 1,false);
  SetNumberToDisplay(Score.VisitorScore, 3,false);
  //Show();
}

void PrintPeriod() {
  SetNumberToDisplay(Score.Period, 2,false);
}

void PrintFoul() {
  SetNumberToDisplay(Score.OwnerFoul, 4,false);
  SetNumberToDisplay(Score.VisitorFoul, 7,false);
}
void setup() {

  DisplayVieverSetup();
  Serial.begin(9600);
  //Score.OwnerScore=13;
 // Score.VisitorScore++;
 // Score.Period = 3;

  // Define pin modes for TX and RX
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    
    // Set the baud rate for the SoftwareSerial object
    //mySerial.begin(38400);
    mySerial.begin(9600);
    TimeSetup();
}

void Reboot()
{
  asm volatile("jmp 0x00");
}

void RunCommand(String command)
{
  if(command.indexOf("GAME") == 0) OperatingMode = !OperatingMode; //Вкл-выкл режим игры
  else if(command.indexOf("OSCOREADD") == 0) Score.OwnerScore+=command.substring(9).toInt(); //ДОБАВИТЬ ОЧКОВ ХОЗЯИНУ
  else if(command.indexOf("OSCORETAKE") == 0) Score.OwnerScore-=command.substring(10).toInt(); //отнять ОЧКОВ ХОЗЯИНУ
  else if(command.indexOf("VSCOREADD") == 0) Score.VisitorScore+=command.substring(9).toInt(); //ДОБАВИТЬ ОЧКОВ ГОСТЮ
  else if(command.indexOf("VSCORETAKE") == 0) Score.VisitorScore-=command.substring(10).toInt(); //отнять ОЧКОВ Гостю

  else if(command.indexOf("OSCORECLEAR") == 0) Score.OwnerScore=0; //ОЧИСТИТЬ ИГРОКА
  else if(command.indexOf("VSCORECLEAR") == 0) Score.VisitorScore=0; //ОЧИСТИТЬ ГОСТЯ
  else if(command.indexOf("OSCORESET") == 0) Score.OwnerScore=command.substring(9).toInt(); //ЗАДАТЬ ОЧКИ ХОЗЯИНУ
  else if(command.indexOf("VSCORESET") == 0) Score.VisitorScore=command.substring(9).toInt(); //АДАТЬ ОЧКИ ГОСТЮ


  else if(command.indexOf("PERIODADD") == 0) Score.Period++;
  else if(command.indexOf("PERIODTAKE") == 0) Score.Period--;
  else if(command.indexOf("PERIODCLERA") == 0) Score.Period=0;
  else if(command.indexOf("PERIODSET") == 0) Score.Period = command.substring(9).toInt();

  else if(command.indexOf("OFOULADD") == 0) Score.OwnerFoul+=command.substring(8).toInt();
  else if(command.indexOf("OFOULTAKE") == 0) Score.OwnerFoul-=command.substring(9).toInt();
  else if(command.indexOf("VFOULADD") == 0) Score.VisitorFoul+=command.substring(8).toInt();
  else if(command.indexOf("VFOULTAKE") == 0) Score.VisitorFoul-=command.substring(9).toInt();
  else if(command.indexOf("OFOULCLEAR") == 0) Score.OwnerFoul=0;
  else if(command.indexOf("VFOULCLEAR") == 0) Score.VisitorFoul=0;
  else if(command.indexOf("OFOULSET") == 0) Score.OwnerFoul=command.substring(8).toInt();
  else if(command.indexOf("VFOULSET") == 0) Score.VisitorFoul=command.substring(8).toInt();



  else if(command.indexOf("CHRONOMETERCL") == 0) ChronometerClear();   
  else if(command.indexOf("CHRONOMETERSTART") == 0) StartChronometer = !StartChronometer; 
  else if(command.indexOf("CHRONOMETER") == 0) Chronometer = !Chronometer;   
  
  else if(command.indexOf("SETTIMER") == 0) SetTimerStr(command);
  else if(command.indexOf("TIMER") == 0) StartTimer =!StartTimer;


  else if(command.indexOf("CL") == 0) Score.ClearAll();

  else if(command.indexOf("DEBUGTIME") == 0) debugTime = !debugTime;
  else if(command.indexOf("DEBUG") == 0) debug = !debug; 

  else if(command.indexOf("TIMEAD") == 0) SetTimeAdjustment(command.substring(7).toInt());
  else if(command.indexOf("TIME") == 0) SetTimeStr(command);
  else if(command.indexOf("REBOOT") == 0) Reboot();
}

void ReadCommand()
{
  if (Serial.available()) {
    String command = Serial.readString();
    Serial.println("OK");
    Serial.println(command);

    RunCommand(command);
  }

  if (mySerial.available()) {
    String command = mySerial.readString();
    mySerial.println("OK");
    if(debug) mySerial.println(command);

    RunCommand(command);
  
    if(debug)
    {
      mySerial.println(OperatingMode);
      mySerial.println(debug);

      mySerial.print("SCORE: ");
      mySerial.print(Score.OwnerScore);
      mySerial.print(" ");
      mySerial.println(Score.VisitorScore); 

      mySerial.print("PERIOD: ");
      mySerial.println(Score.Period);

      mySerial.print("FOUL: ");
      mySerial.print(Score.OwnerFoul);
      mySerial.print(" ");
      mySerial.println(Score.VisitorFoul);
    }
  }
}



unsigned long oldShow;
unsigned long oldClear;
void loop() {

  /* for(int i=1;i<=11;i++)
  {  
    setData(numbers[i%10], i);
    setData(numbers[i%10], 11-i);
    Show();
    delay(400);
    clearData();
   // Show();
   // delay(100);
  }*/
  ReadCommand(); //Чтение и выполнение команды
  if (OperatingMode) //Вывод счета в режиме игры
  {
    PrintScore();
    PrintPeriod();
    PrintFoul();
  }

  if(Chronometer) //Вывод времени или таймера/секундомера
  {    
    showTimer();
  }
  else
  {
    showTime();  
  }

  if(StartChronometer) //вкл секундомер
  {
    ChronometerTick();      
  }
  else if(StartTimer) //вкл таймер
  {
    TimerTick();
  }


  if(millis() - oldShow > ShowDelay) //обновление индикаторов по времени
  {
    Show();
    oldShow = millis();
  } 

  /*if(millis() - oldClear > ClearDelay)
  {
    oldClear = millis();
    RebootDisplay();
  } */
  clearData(); //чистим буфер
  delay(5);
}