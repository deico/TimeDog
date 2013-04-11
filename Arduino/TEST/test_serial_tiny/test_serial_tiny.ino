//#include <SoftwareSerial.h>
//SoftwareSerial mySerial(2, 3); //Rx=2;Tx=3 ; 

//int contador=0;
void setup() {
  //Serial.begin(9600);
  delay(2000);
}

void loop() {
  //contador++;  
  //if(contador==10)contador=0;
  Serial.begin(9600);
  delay(1000);  
  Serial.println("El contador es: ");
  //Serial.println(contador); //ATTINY
  //Serial.end();
  delay(1000);     
  
}
