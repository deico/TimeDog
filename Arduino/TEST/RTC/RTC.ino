#include <string.h>
#include <Wire.h>

#include "globales.h"
#include "Rtc.h"

void setup(){
  Serial.begin(9600);
  
  // Wire RTC
  Wire.begin();

  // RTC init
  //Para volver a setear la hora y fecha hay que quitar el if y el WriteDs1307
  if(ReadDs1307(0x08) != 0xaa)
  {
    second = 0;
    minute = 42;
    hour = 16;
    dayOfWeek = 1;
    dayOfMonth = 4;
    month = 3;
    year = 13;
    SetDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year);   
    WriteDs1307(0x08,0xaa); 
  }
}

void loop () {
  GetDateDs1307(&second,&minute,&hour,&dayOfWeek,&dayOfMonth,&month,&year);
  Date2Str(date);
  Time2Str(time);
  for(int i = 0; i<10; i++)Serial.print(date[i]);
  Serial.println("");
  for(int i = 0; i<8; i++)Serial.print(time[i]);
  Serial.println("");
  delay(3000);
}


