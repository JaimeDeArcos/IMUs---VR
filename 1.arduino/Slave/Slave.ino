/***********************************************************************
 * --------------------------------IMU---------------------------------*
 * Autor: Jaime de Arcos Sanchez                                       *
 * Fecha: 6 Abril 2016                                                 *
 *                                                                     *
 *  - VERSION v1.0                                                     *
 *                                                                     *
 ***********************************************************************/
//LIBRERIAS STANDAR
#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
  
//CONFIGURACION
#include "config_Main.h"
#include "Config_Memoria.h"
#include "Config_Slave.h"
 
//LIBRERIAS GNU
#include "SoftI2CMaster.h"

//LIBRERIAS PROPIAS 
#include "vector.h"
#include "MPU6050.h"
#include "HMC5883L.h"
#include "IMU.h"

IMU      imu; 
volatile byte rQ;
volatile bool flag = true;

//-------------------------------------------------------------
void setup() 
{
  //COM serial
  Serial.begin(250000);
 
  //COM Software I2C - Maestro
  if (!i2c_init()) Serial.println("ERROR: SW I2C");
    
  //COM Hardware I2C - Esclavo
  byte i2cAdd = getI2CADDRESS();
  Wire.begin(i2cAdd);
  Wire.setClock(400000);        //400kHz 
  Wire.onRequest(requestEvent); 
  Wire.onReceive(receiveEvent);
    
  //Sensores y DCM 
  imu.init();
  
  //Led de estatus
  pinMode(PIN_LED,OUTPUT);
}
//-------------------------------------------------------------
void loop()
{
  if (flag){
      imu.filtroComplementario();  
      flag = false;    
  }
}
//-------------------------------------------------------------
void serialEvent() {
  while(Serial.available()){

     int dato = Serial.read();
     
     switch (dato) {
         case 'a':  //DCM:     I,J,K
                    Serial.println(imu.dcmToString());    
                    imu.filtroComplementario();
                    break; 
                    
         case 'b':  //RAW:    Gyro,ACC,Mag
                    Serial.println(imu.valuesToString());    
                    imu.filtroComplementario();
                    break;
                    
         case 'c':  //VALUES: Gyro,ACC,Mag
                    Serial.println(imu.rawToString()); 
                    imu.filtroComplementario();
                    break;
         case 'd':  Serial.println(EEPROM_toString());         break;
         case 'e':  Serial.println(imu.calibrarMPU());         break;
         case 'f':  Serial.println(imu.calibrarHMC(1));        break;
         case 'g':  Serial.println(imu.calibrarHMC(2));        break;
         case 'h':  Serial.println(memoryInit());              break; 
         case 'i':  Serial.println(memoryZero());              break;
         case 'j':  changeAdress();                            break;  
         case 'k':  Serial.println(imu.setPesos(1,0,0));       break;    
         case 'l':  Serial.println(imu.setPesos(0,1,0));       break;   
         case 'm':  Serial.println(imu.setPesos(0,0,1));       break;   
         case 'n':  Serial.println(imu.setPesos(0.8,0.1,0.1)); break;  
     } 
     blink();  
  }
}
//-------------------------------------------------------------
void requestEvent()
{
   switch (rQ) {
    case 0x00:  imu.enviar(0); break;
    case 0x01:  imu.enviar(1); break;
    case 0x02:  imu.enviar(2); break;
    case 0x03:  imu.enviar(3); break;
    case 0x04:  imu.enviar(4); break;
    case 0x05:  imu.enviar(5); break;
    case 0x06:  imu.enviar(6); break;
    case 0x07:  imu.enviar(7); break;
    case 0x08:  imu.enviar(8); 
                flag = true;  
                blink();       break;  
  }
}
//-------------------------------------------------------------
void receiveEvent(int iData){ rQ = Wire.read(); }
//-------------------------------------------------------------
