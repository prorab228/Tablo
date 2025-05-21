#include "iarduino_RTC.h"
#include <EEPROM.h>

iarduino_RTC time(RTC_DS1302,A3,A5,A4);  // для модуля DS1302 - RST, CLK, DAT
// iarduino_RTC time(RTC_DS1307);       // для модуля DS1307 с i2C
// iarduino_RTC timeRTC_DS3231);        // для модуля DS3231 с i2C
int TimeAdjustmentSec = 0;

void TimeSetup()
{

  time.begin();
  //time.settime(0, 30, 18, 12, 6, 20, 5); // 0  сек, 30 мин, 18 часов, 12, июня, 2020, четверг
  TimeAdjustmentSec = EEPROM.read(0)-128;
}


void SetTimeStr(String command)
{
 // mySerial.println("Time set");
  if(command.indexOf("TIME ")!=-1)
  {
    String time = command.substring(command.indexOf("TIME ")+5);
    //mySerial.println(time);
    if(time.indexOf(':')!=-1)   
    {
      mySerial.println(time);
        int pos = time.indexOf(':');
        String h = time.substring(0,pos);
        String m = time.substring(pos+1);

       // mySerial.println(h+" "+m);

        SetTime(h.toInt(), m.toInt());

        mySerial.println("The time is set");
        mySerial.print(h.toInt());
        mySerial.print(":");
        mySerial.println(m.toInt());
    }
  }
}

void SetTime(int hour, int minutes)
{
  time.settime(0, minutes, hour); // 0  сек, 30 мин, 18 часов, 12, июня, 2020, четверг
  //time.settime(0,-1, values); 
}


void SetTimeAdjustment(int sec)
{
  if(TimeAdjustmentSec<-59) TimeAdjustmentSec = -59;
  if(TimeAdjustmentSec>59) TimeAdjustmentSec = 59;
  
  TimeAdjustmentSec = sec;
  EEPROM.update(0, TimeAdjustmentSec+128);

  mySerial.print(TimeAdjustmentSec);
  mySerial.println(" TimeAdjustment OK");
}

void TimeAdjustment()
{
  time.gettime();
  //Получаем текущее время и ставим его же с поправкой
  time.settime(time.seconds + TimeAdjustmentSec, time.minutes, time.Hours); // 0  сек, 30 мин, 18 часов, 12, июня, 2020, четверг
}

//bool ShowDots = true;
bool isAdjustment = false;
void showTime()
{   
    time.gettime();

    SetNumberToDisplay(time.Hours ,5, false);
    SetNumberToDisplay(time.minutes,6, time.seconds%2); // с точками

    if(debugTime)
    {
      mySerial.println(time.gettime("H:i:s"));
      mySerial.print("Time Ad: ");
      mySerial.println(TimeAdjustmentSec);
    } 

    if(/*time.Hours == 1 &&*/ time.minutes==1 && time.seconds>=(TimeAdjustmentSec*-1)  && !isAdjustment)
    {
      TimeAdjustment(); //Подстройка времени
      isAdjustment = true;
    } 
    else if(time.minutes==2)
    {
      isAdjustment = false;
    } 

   /* SetNumberToDisplay(20 ,5,false);
    SetNumberToDisplay(56,6,true);*/
   /* setData(numbers[10] + numbers[3], 2);
    setData(numbers[4], 3);
    //setData(4, 3);
    setData(numbers[4], 4);
    setData(numbers[4], 5);*/
    //Show();
    
    //clearData();
}


int timer = 0;
void SetTimer(int minutes, int seconds)
{
  timer=minutes *60 + seconds;

}

void SetTimerStr(String command)
{
  mySerial.println("Timer set");
  if(command.indexOf("SETTIMER ")!=-1)
  {
    String time = command.substring(command.indexOf("SETTIMER ")+9);
    mySerial.println(time);
    if(time.indexOf(':')!=-1)   
    {
      mySerial.println(time);
        int pos = time.indexOf(':');
        String m = time.substring(0,pos);
        String s = time.substring(pos+1);

       // mySerial.println(h+" "+m);

        SetTimer(m.toInt(), s.toInt());

        mySerial.println("The timer is set");
        mySerial.print(m.toInt());
        mySerial.print(":");
        mySerial.println(s.toInt());
    }
  }
}

unsigned long oldTick;
void TimerTick()
{
  if(millis()-oldTick > 1000 && timer>0)
  {
    timer-=1;
    oldTick = millis();
  }
  if(timer <= 0)
  {
    timer=0;
    StartTimer = false;
  }
}


void ChronometerClear()
{
  timer=0;
}

void ChronometerTick()
{
  if(millis()-oldTick > 1000)
  {
    timer+=1;
    oldTick = millis();
  }

  if(timer >= 6039)
  {
    StartChronometer = false;
    timer=0;
  }
}

void showTimer()
{
  SetNumberToDisplay(timer/60 ,5, false);
  SetNumberToDisplay(timer%60,6, true);
}