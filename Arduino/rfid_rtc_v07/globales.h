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

//RFID Variables
//Variable global para identificar el codigo
byte code[6];
//Variable global para identificar usuario
int usuario = 0;
//Es necesario reiniciar el rfid para detecta que se mantiene el tag
int reset = 8;
//Variable global para activar o desactivar usuario. Tambien indica las tandas
int usuario1_activar=1; //EEPROM 3
int usuario2_activar=1; //EEPROM 4
//Variables para llevar el tiempo restante
int tiempo_juego=60;
int minutos_juego1=0; //EEPROM 1
int minutos_juego2=0; //EEPROM 2
int pausa=0;
int minutos_stop1=0; 
int minutos_stop2=0; 
//Variable para comparar minuto actual con el anterior
int minuto_ant=0;

//EEPROM VARIABLES
int dia_mes; //EEPRTOM 0

//RELE
int forzar_apagado=0;
