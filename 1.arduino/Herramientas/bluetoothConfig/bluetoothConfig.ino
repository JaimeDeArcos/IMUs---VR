#include <SoftwareSerial.h> //Librería que permite establecer comunicación serie en otros pins

#define RETARDO 300

//Aquí conectamos los pins RXD,TDX del módulo Bluetooth.
SoftwareSerial bluetooth(10,11); //10 RX, 11 TX.
 
void setup()
{
  bluetooth.begin(38400); //38400 Por defecto
  Serial.begin(38400); //Abrimos la comunicación serie con el PC y establecemos velocidad

  Serial.println("Configuracion Del Modulo Bluetooth HC-05"); 
  
  Serial.println("BT-OFF"); 
  pinMode(12, OUTPUT);    // set pin to input
  digitalWrite(12, LOW);  // turn on pullup resistors

  delay(1000);

  Serial.println("BT-ON"); 
  digitalWrite(12, HIGH);

  delay(1000);
 
  Informacion();
  configuracion();
  Informacion();
   
}
 
void loop()
{ 
    
  if(bluetooth.available())
    Serial.write(bluetooth.read());
 
  if(Serial.available())
     bluetooth.write(Serial.read());
}

void Informacion(){
  
  comando("TEST Comunicacion: ","AT");
  comando("Version: ","AT+VERSION");
  comando("ADDRESS: ","AT+ADDR");
  comando("NOMBRE: ","AT+NAME");
  comando("ROLE: ","AT+ROLE");
  comando("BAUDRATE: ","AT+UART"); 
  comando("PASSWORD: ","AT+PSWD");
}

void configuracion(){
 
 //FACTORY RESET
  comando("Factory Reset: ","AT+ORGL"); 
  delay(1000);

  comando("setNombre: ","AT+NAME","BT_NHECKRO");  
  comando("setBaudRate: ","AT+UART","115200,0,0");  
  comando("setPassword: ","AT+PSWD","0808"); 
  
}

void comando(char* msg , char* cmd){ 

  char comando[80]; 
  sprintf(comando,"%s%s",cmd,"\r\n"); 
 
  Serial.println(msg); 
  bluetooth.write(comando);
  delay(RETARDO); 
  recibir(); 
  delay(RETARDO); 
}

void comando(char* msg , char* cmd, char* valor){ 

  char comando[80]; 
  sprintf(comando,"%s=%s\r\n",cmd,valor); 
 
  Serial.println(msg); 
  bluetooth.write(comando);
  delay(RETARDO); 
  recibir(); 
  delay(RETARDO); 
}

void recibir(){
  while(bluetooth.available())
     Serial.write(bluetooth.read()); 
}
 

