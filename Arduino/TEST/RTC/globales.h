//NECESARIO PARA USAR VARIABLES TIPO BYTE 

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// RTC variables
byte second;        // 0-59
byte minute;       // 0-59
byte hour;          // 1-23
byte dayOfWeek;     // 1-7
byte dayOfMonth;    // 1-28/29/30/31
byte month;         // 1-12
byte year;
char date[11];
char time[9];

// States variables
int state;
int master;
unsigned long cstime;
unsigned long mtime;
