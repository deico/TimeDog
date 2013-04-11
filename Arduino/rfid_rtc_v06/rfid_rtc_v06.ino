//RTC
#include <string.h>
#include <Wire.h>
//EPROM
#include <EEPROM.h>

//SERIAL DEL RFID
#include <SoftwareSerial.h>
SoftwareSerial mySerial(5, 6); // RX, TX

#include "globales.h"
#include "Rtc.h"
#include "Rfid.h"

//Utilizado para el control del rele
#include <Metro.h> //Include Metro library
Metro releMetro = Metro(2000); 
int state = LOW;
#define RELE 7

//Variables para controlar el tiempo de juego
Metro minutoMetro = Metro(5000); 

void setup() {
  //SERIAL DEBUG
  Serial.begin(9600);      
  //SERIAL RFID
  mySerial.begin(9600);  // connect to the serial port
  //Reset necesario para el RFID
  pinMode(reset, OUTPUT);
  digitalWrite(reset, HIGH); 
  //RELE
  pinMode(RELE,OUTPUT);
  digitalWrite(RELE,state);  
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
  
  //Si es otro dia lo almaceno en eeprom y reinicio los minutos de juego a cada jugador en variable de programa y en EEPROM
  //delay(10000);
  resetear_tiempo_dia();
  Serial.println("Esperando byte para reiniciar.");
  delay(2000);
  if (Serial.available() > 0){
    EEPROM.write(0, 50); //ASIGNO UN DIA DEL MES QUE NO EXISTE PARA RESETAEAR TIEMPO
    resetear_tiempo_dia();
    Serial.println("Resetear");
  }
  
  minutos_juego1 = EEPROM.read(1);
  minutos_juego2 = EEPROM.read(2);
  usuario1_activar = EEPROM.read(3);
  usuario2_activar = EEPROM.read(4);
  
}

int tiempo_pausa=0;

void loop () {
  while(true){
  int target=0;
  
  //SI HAY TARGETA RETORNA 0 Y ALCMACENA EL NUMERO EN code[]
  target=leer_rfid();
  
  //Revisa si hay que apagar
  if (releMetro.check() == 1 || forzar_apagado){
    forzar_apagado = 0;
    usuario=0;
    Serial.print(usuario);
    Serial.println("Apagar");
    state=LOW;
  }
  
  //DESCUENTO TIEMPO STOP JUGADOR 1
  if(minutos_stop1>0){
    GetDateDs1307(&second,&minute,&hour,&dayOfWeek,&dayOfMonth,&month,&year);
    if(tiempo_pausa!=minute){
      Serial.println("PAUSA DE ENZO");
      minutos_stop1=minutos_stop1-1;
      tiempo_pausa=minute;
    }
  }
  //DESCUENTO TIEMPO STOP JUGADOR 2
  if(minutos_stop2>0){
    GetDateDs1307(&second,&minute,&hour,&dayOfWeek,&dayOfMonth,&month,&year);
    if(tiempo_pausa!=minute){
      Serial.println("PAUSA DE VIXO");
      minutos_stop2=minutos_stop2-1;
      tiempo_pausa=minute;
    }
  }
  //SI HAY TARJETA
  if (target==0)
  {
    //Serial.print("Tarjeta: ");
    //Serial.println(code[4], HEX);
    
   //Ve si la tarjeta corresponde a uno de los 2 usuaruios y si este esta activo
    if((code[4]==0xba&&usuario1_activar&&minutos_stop1==0)||(code[4]==0xef&&usuario2_activar&&minutos_stop2==0)){     
      //Seteo el tiempo en que comenzo a jugar
      if(usuario==0){    //SI LA WII ESTABA APAGADA
        //SEÑALO USUARIO
        if(code[4]==0xba)usuario=1;
        if(code[4]==0xef)usuario=2;
        //OBTENGO TIEMPO
        GetDateDs1307(&second,&minute,&hour,&dayOfWeek,&dayOfMonth,&month,&year);
        //GUARDO EL MINUTO PREVIO
        minuto_ant=minute;
        Date2Str(date);
        Time2Str(time);
        //IMPRIMO FECHA
        for(int i = 0; i<10; i++)Serial.print(date[i]);
        Serial.print(" | ");
        //IMPRIMO HORA
        for(int i = 0; i<8; i++)Serial.print(time[i]);
        Serial.println("");
        //IMPRIMO MINUTOS DE JUGADOR 1        
        if(usuario==1){
          Serial.print("A enzo le quedan: ");
          Serial.print(minutos_juego1);
          Serial.println(" minutos");
        }
        //IMPRIMO MINUTOS DE JUGADOR 2  
        if(usuario==2){
          Serial.print("A vixo le quedan: ");
          Serial.print(minutos_juego2);
          Serial.println(" minutos");
        }
      }
      
      //SEÑALO USUARIO NO SOLO DESPUES DE APAGAR LA WII
      //if(code[4]==0xba)usuario=1;
      //if(code[4]==0xef)usuario=2;
      
      //FORZO A APAGAR LA WII SI SE CAMBIO RAPIDAMENTE DE TARJETA
      if(usuario==1)
        if(code[4]!=0xba){
        usuario=0;
        resetear_rfid();
        forzar_apagado = 1;
        continue;
      }
      //FORZO A APAGAR LA WII SI SE CAMBIO RAPIDAMENTE DE TARJETA
      if(usuario==2)
        if(code[4]!=0xef){
        usuario=0;
        resetear_rfid();
        forzar_apagado = 1;
        continue;
      }
      
      //Reviso el tiempo
      GetDateDs1307(&second,&minute,&hour,&dayOfWeek,&dayOfMonth,&month,&year);
      //Descuento un minuto de juego
      if (minuto_ant!=minute){
        //Actualizo el minuto
        GetDateDs1307(&second,&minute,&hour,&dayOfWeek,&dayOfMonth,&month,&year);
        minuto_ant=minute;     
        //Resto minutos y modifico valor en EEPROM cada 5 minutos
        if(usuario==1)Serial.print("Enzo ");
        if(usuario==2)Serial.print("Vixo ");
        Serial.print("Menos un minuto: ");
        if(usuario==1){
          minutos_juego1=minutos_juego1-1;
          Serial.println(minutos_juego1);
          if(minutos_juego1!=0&&minutos_juego1%1==0){
            Serial.println("A EEPROM J1");
            EEPROM.write(1, minutos_juego1);
          }
        }if(usuario==2){
          minutos_juego2=minutos_juego2-1;
          Serial.println(minutos_juego2);
          if(minutos_juego2!=0&&minutos_juego2%1==0){
            Serial.println("A EEPROM J2");
            EEPROM.write(2, minutos_juego2);
          }
        }     
      }
      
      //Desactivo tarjeta
      if((minutos_juego1<=0&&usuario==1)||(minutos_juego2<=0&&usuario==2)){
        Serial.println("Fin de tu tiempo");
        if(usuario==1){
          if(usuario1_activar>0){
            usuario1_activar=usuario1_activar-1;
            EEPROM.write(3, usuario1_activar);
            EEPROM.write(1, tiempo_juego);
            minutos_juego1=tiempo_juego;
            if(usuario1_activar!=0){
              Serial.println("Otra tanda a ENZO");
              minutos_stop1=pausa;
            }
            else Serial.println("Fin tandas a ENZO");
          }
        }
        if(usuario==2){
          if(usuario2_activar>0){
            usuario2_activar=usuario2_activar-1;
            EEPROM.write(4, usuario2_activar);
            EEPROM.write(2, tiempo_juego);
            minutos_juego2=tiempo_juego;
            if(usuario2_activar!=0){
              Serial.println("Otra tanda a VIXO");
              minutos_stop2=pausa;
            }
            else Serial.println("Fin tandas a VIXO");
              
            
          }
        }
        /*
        //MUESTRO FECHA Y HORA
        GetDateDs1307(&second,&minute,&hour,&dayOfWeek,&dayOfMonth,&month,&year);
        Date2Str(date);
        Time2Str(time);
        for(int i = 0; i<10; i++)Serial.print(date[i]);
        Serial.println("");
        for(int i = 0; i<8; i++)Serial.print(time[i]);
        Serial.println("");
        */
      }
      /*
      Serial.print(usuario);
      if(usuario==1)Serial.println("enzo");
      if(usuario==2)Serial.println("vixo");
      */
      
      releMetro.reset();
      state=HIGH;
    }
    
    resetear_rfid();
  }
  //Control rele
  digitalWrite(RELE,state);
  //Pausa
  delay(100);
  }
  
}
