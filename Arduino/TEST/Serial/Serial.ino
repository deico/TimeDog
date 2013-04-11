/*
Este programa es para probar el serial.
Se conecta el serial del attiny al serial de arduino UNO.
El attiny va con Serial.pde
El arduino UNO va cargada con el programa ECHO.PDE
*/
int contador=0;
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 
}

void loop() {
  // print the results to the serial monitor:
  Serial.print("contador= " );                       
  Serial.println(contador);      

  // wait 10 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(3000);     
  contador++;  
}
