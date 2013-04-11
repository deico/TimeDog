#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); //Rx=2;Tx=3 ; 

int contador=0;
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
  mySerial.begin(9600); 
  //mySerial.begin(1200);
}

void loop() {
  //mySerial.println(contador);
  delay(1000);
  while (mySerial.available()){
    delay(100);
    Serial.write(mySerial.read());
  }
  /*if (Serial.available())
    mySerial.write(Serial.read());
    */
  //delay(3000);     
  //contador++;  
}
