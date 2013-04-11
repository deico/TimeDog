/*
TX (Amarillo) -> D2
RX (Blanco  ) -> D3
*/


int leer_rfid(void){
  //Serial.println("Leer Rfid");
  int count=0;  
  if (mySerial.available())              // if date is comming from softwareserial port ==> data is comming from SoftSerial shield
    {
      delay(100);
    while(mySerial.available())          // reading data into char array 
    {
      buffer[count++]=mySerial.read();     // writing data into array
      if(count == 14){
        return 0;
        Serial.println(buffer);
      }
    }
  }
  return 1;
}

void reset_rfid(void){
  //Serial.println("ledMetro init");
  digitalWrite(vo_rfid, LOW); 
  mySerial.end();
  digitalWrite(2, LOW); 
  digitalWrite(3, LOW); 
  delay(100);
  digitalWrite(vo_rfid, HIGH);
  digitalWrite(2, HIGH); 
  digitalWrite(3, HIGH); 
  mySerial.begin(9600);   
  //Serial.println("ledMetro finit");
}


int chk_rfid(void){
  unsigned long chksum=0;
  int chk=0;
  //el chk lo calculo sumando todos lso valores ascii de cada byte. Los ultimos dos digitos se los resto a 255.
  //08004A012A69tiene chk 202
  //08000AAAB21A tiene chk 177
  
  //Serial.print("COMP: ");
  for(int i=0;i<=14;i++){
    //Serial.print(buffer[i]);
    int num=buffer[i];
    chksum=chksum+num;
    chk=chksum%100;
    chk=255-chk;
  }
  
  //Serial.println(chksum);
  //Serial.println(chk);
  //chksum=0;
  return chk;
}
/*
int target=1;
int user_chk=0;
void loop()
{  
  target=leer_rfid();
  if(!target){
    target=1;
    Serial.println("Encontrado: ");
    Serial.println(buffer);
    user_chk=chk_rfid();
    Serial.println(user_chk);
    if(user_chk==202)Serial.println("Usuario Carlos");
  }
  if (ledMetro.check() == 1){
    reset_rfid();
    ledMetro.reset();    
  }  
}
*/
