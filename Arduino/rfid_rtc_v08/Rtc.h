/*
SCL (Amarillo) -> A5
SDA (Blanco  ) -> A4
*/

//NECESARIO PARA USAR VARIABLES TIPO BYTE 
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

// Constants
#define LINE 18
#define DS1307_I2C_ADDRESS 0x68

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/*
* RTC functions
 */
 
/*
* Leftpads str with zeros 
*/
void strpad(char *str,int length){
  char aux[length+1];
  for(int i = 0; i<length; i++){
    aux[i] = '0';
  }
  aux[length] = '\0';
  for(int i = 0; i<strlen(str); i++){
    aux[length-1-i] = str[strlen(str)-1-i];
  }
  for(int i = 0; i<length+1; i++){
    str[i] = aux[i];
  }
}


/*
* Converts HEX char to number
*/
byte convert(byte a){
  if(a<58)
    return a-48;
  else
    return a-55;
}

byte DecToBcd(byte val)
{
  return ( (val/10*16) + (val%10) );
}
 
// Convert binary coded decimal to normal decimal numbers
byte BcdToDec(byte val)
{
  return ( (val/16*10) + (val%16) );
}

void Date2Str(char* buf)
{
  if(dayOfMonth < 10)
    *(buf++) = '0';
  itoa(dayOfMonth,buf,10);
  buf+=strlen(buf);
  *(buf++) = '/';
  if(month < 10)
    *(buf++) = '0';
  itoa(month,buf,10);
  buf+=strlen(buf);
  strcpy(buf,"/20");
  buf+=strlen(buf);
  itoa(year,buf,10);
  //Serial.print("Fecha: ");
  //Serial.println(*buf);
}

void Time2Str(char* buf)
{
  if(hour < 10)
    *(buf++) = '0';
  itoa(hour,buf,10);
  buf+=strlen(buf);
  *(buf++) = ':';
  if(minute < 10)
    *(buf++) = '0';
  itoa(minute,buf,10);
  buf+=strlen(buf);
  *(buf++) = ':';
  if(second < 10)
    *(buf++) = '0';
  itoa(second,buf,10);  
  //Serial.print("Hora: ");
  //Serial.println(*buf);
}

// 1) Sets the date and time on the ds1307
// 2) Starts the clock
// 3) Sets hour mode to 24 hour clock
// Assumes you're passing in valid numbers
void SetDateDs1307(byte second,        // 0-59
byte minute,        // 0-59
byte hour,          // 1-23
byte dayOfWeek,     // 1-7
byte dayOfMonth,    // 1-28/29/30/31
byte month,         // 1-12
byte year)          // 0-99
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  byte cero=0x00;
  Wire.write(cero);
  Wire.write(DecToBcd(second));    // 0 to bit 7 starts the clock
  Wire.write(DecToBcd(minute));
  Wire.write(DecToBcd(hour));      // If you want 12 hour am/pm you need to set
  // bit 6 (also need to change readDateDs1307)
  Wire.write(DecToBcd(dayOfWeek));
  Wire.write(DecToBcd(dayOfMonth));
  Wire.write(DecToBcd(month));
  Wire.write(DecToBcd(year));
  Wire.endTransmission();
}

// Gets the date and time from the ds1307
void GetDateDs1307(byte *second,byte *minute,byte *hour,byte *dayOfWeek,byte *dayOfMonth,byte *month,byte *year)
{
  // Reset the register pointer
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  byte cero=0;
  Wire.write(cero);
  Wire.endTransmission();

  Wire.requestFrom(DS1307_I2C_ADDRESS, 7);

  // A few of these need masks because certain bits are control bits
  *second     = BcdToDec(Wire.read() & 0x7f);
  *minute     = BcdToDec(Wire.read());
  *hour       = BcdToDec(Wire.read() & 0x3f);  // Need to change this if 12 hour am/pm
  *dayOfWeek  = BcdToDec(Wire.read());
  *dayOfMonth = BcdToDec(Wire.read());
  *month      = BcdToDec(Wire.read());
  *year       = BcdToDec(Wire.read());
}

void WriteDs1307(byte address,byte data)
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission(); 
}

byte ReadDs1307(byte address)
{
  Wire.beginTransmission(DS1307_I2C_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_I2C_ADDRESS, 1);
  return Wire.read();
}

void resetear_tiempo_dia(void){
  //LEO el RTC para saber el dia
  GetDateDs1307(&second,&minute,&hour,&dayOfWeek,&dayOfMonth,&month,&year);
  dia_mes = EEPROM.read(0);
  if(dia_mes!=dayOfMonth){
    Serial.print("Dia Anterior: ");
    Serial.print(dia_mes);
    Serial.print(" | ");
    Serial.print("Dia Actual: ");
    Serial.print(dayOfMonth);
    Serial.println(" | RESET.");
    EEPROM.write(0, dayOfMonth);
    EEPROM.write(1, tiempo_juego);
    EEPROM.write(2, tiempo_juego);
    EEPROM.write(3, usuario1_activar);
    EEPROM.write(4, usuario2_activar);
  }  
}
