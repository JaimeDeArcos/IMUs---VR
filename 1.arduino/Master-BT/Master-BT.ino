/***********************************************************************
 * --------------------------------IMU---------------------------------*
 * Autor: Jaime de Arcos Sanchez                                       *
 * Fecha: 6 Abril 2016                                              *
 *                                                                     *
 *  - VERSION v1.0                                                    *
 *                                                                     *
 ***********************************************************************/
//LIBRERIAS OFICIALES 
#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>  

//CONFIGURACION
#include "config.h"
#include "Memoria.h"

//LIBRERIAS GNU
#include "SoftI2CMaster.h"

//LIBRERIAS PROPIAS 
#include "FuncionesAuxiliares.h"
#include "vector.h"
#include "MPU6050.h" 
#include "HMC5883L.h" 
#include "IMU.h"
#include "Slave.h" 

IMU imu;

SoftwareSerial BT(10,11); //10 RX, 11 TX.

void setup() {
  
  //COM serial
  Serial.begin(250000);

  BT.begin(115200);

  //COM Hardware I2C - Master
  Wire.begin();
  Wire.setClock(400000);   //400kHz
  
  //COM Software I2C - Maestro
  if (!i2c_init()) Serial.println("ERROR: i2c software");
   
  imu.init();

  blink();
}
//***************************************************
void loop() {  
  if(BT.available()){

    #ifdef DEBUG_TIME
         tPrincipio = micros();
    #endif
    
    
    int dato = BT.read(); 

    if ((dato>=49) && (dato<=57)){ 
      //NUMERO del 1-9
      dato -= 48;
      BT.println(Slave_toString(dato));
      Slave_update(dato);
      
    }else{
    
    switch (dato) {
         case 'a':  BT.println(imu.DCMtoString());       //I,J,K
                    imu.update();
                    break;      
         case 'b':  BT.println(imu.RAWtoString());    //Gyro,ACC,Mag
                    imu.update();
                    break;
         case 'c':  BT.print(EEPROM_toString());  break;
         case 'd':  imu.calibrarMPU();  break;
         case 'e':  imu.calibrarHMC(1); break;
         case 'f':  imu.calibrarHMC(2); break;  
         case 'g':  BT.println(memoryInit());     break;
      }
    }
    blink();  

    #ifdef DEBUG_TIME
        tFinal = micros();
        tGastado = tFinal - tPrincipio;
        BT.print("T= ");
        BT.print(tGastado); 
        BT.println(" uS");
    #endif 
  }
}
//***************************************************
void serialEvent() {
  
 while(Serial.available()){

    #ifdef DEBUG_TIME
         tPrincipio = micros();
    #endif
    
    
    int dato = Serial.read(); 

    if ((dato>=49) && (dato<=57)){ 
      //NUMERO del 1-9
      dato -= 48;
      Serial.println(Slave_toString(dato));
      Slave_update(dato);
      
    }else{
    
    switch (dato) {
         case 'a':  Serial.println(imu.DCMtoString());       //I,J,K
                    imu.update();
                    break;      
         case 'b':  Serial.println(imu.RAWtoString());    //Gyro,ACC,Mag
                    imu.update();
                    break;
         case 'c':  Serial.print(EEPROM_toString());  break;
         case 'd':  imu.calibrarMPU();  break;
         case 'e':  imu.calibrarHMC(1); break;
         case 'f':  imu.calibrarHMC(2); break;  
         case 'g':  Serial.println(memoryInit());     break;
      }
    }
    blink();  

    #ifdef DEBUG_TIME
        tFinal = micros();
        tGastado = tFinal - tPrincipio;
        Serial.print("T= ");
        Serial.print(tGastado); 
        Serial.println(" uS");
    #endif 
  }
}
