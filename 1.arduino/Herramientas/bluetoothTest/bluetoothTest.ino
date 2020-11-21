#include <SoftwareSerial.h> //Librería que permite establecer comunicación serie en otros pins
 

//Aquí conectamos los pins RXD,TDX del módulo Bluetooth.
SoftwareSerial bluetooth(10,11); //10 RX, 11 TX.
 
void setup()
{
  bluetooth.begin(115200); //38400 Por defecto
  Serial.begin(115200); //Abrimos la comunicación serie con el PC y establecemos velocidad

  Serial.println("Prueba Del Modulo Bluetooth HC-05"); 
  Serial.println("El modulo hara echo a todo lo que se envie");
 
   
}
 
void loop()
{ 
    
  if(bluetooth.available()){
    int datoBT = bluetooth.read(); 
    
    Serial.write(datoBT);
    bluetooth.write(datoBT);
  } 
  
  if(Serial.available()){
    int dato = Serial.read();  
    bluetooth.write(dato); 
  }

}

