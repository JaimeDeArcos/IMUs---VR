#include <Arduino.h>
   
//VALORES DE CALIBRACION HMC
#define EEPROM_HMC_GAIN_OFFSET_X   0
#define EEPROM_HMC_GAIN_OFFSET_Y   4
#define EEPROM_HMC_GAIN_OFFSET_Z   8
#define EEPROM_HMC_OFFSET_X        12
#define EEPROM_HMC_OFFSET_Y        16
#define EEPROM_HMC_OFFSET_Z        20

//VALORES DE CALIBRACION MPU
#define EEPROM_MPU_ACC_BASE_X      24
#define EEPROM_MPU_ACC_BASE_Y      28
#define EEPROM_MPU_ACC_BASE_Z      32
#define EEPROM_MPU_GYRO_BASE_X     36
#define EEPROM_MPU_GYRO_BASE_Y     40
#define EEPROM_MPU_GYRO_BASE_Z     44

String SALTO_LINEA = "\n" ;

union Float_Byte
{
    float datoF;
    byte  datoB[4];
} unionFB;

union Integer_Byte
{
    int  datoI;
    byte datoB[2];
} unionIB;


void floatToMemory(int direccion, float dato){
 
   unionFB.datoF = dato ;  
   
   // guardar un float en la EEPROM en las posiciones 0-3
   EEPROM.write(direccion     , unionFB.datoB[0]);
   EEPROM.write(direccion + 1 , unionFB.datoB[1]);
   EEPROM.write(direccion + 2 , unionFB.datoB[2]);
   EEPROM.write(direccion + 3 , unionFB.datoB[3]);
   
}

float getFloat(int direccion){
  
  unionFB.datoF = 0.0 ;
  
  unionFB.datoB[0] =  EEPROM.read(direccion);
  unionFB.datoB[1] =  EEPROM.read(direccion+1);
  unionFB.datoB[2] =  EEPROM.read(direccion+2);
  unionFB.datoB[3] =  EEPROM.read(direccion+3);
  
  return unionFB.datoF;
}

String EEPROM_toString(){
 
    String eeprom ="";

  eeprom += ("MEMORIA EEPROM: ") + SALTO_LINEA;
  eeprom +=("HMC_GAIN_OFFSET") + SALTO_LINEA;
  eeprom +=("00: ");
  eeprom +=(getFloat(0)) + SALTO_LINEA;
  eeprom +=("04: ");
  eeprom +=(getFloat(4)) + SALTO_LINEA;
  eeprom +=("08: ");
  eeprom +=(getFloat(8)) + SALTO_LINEA;
  eeprom +=("HMC_OFFSET") + SALTO_LINEA;
  eeprom +=("12: ");
  eeprom +=(getFloat(12)) + SALTO_LINEA;
  eeprom +=("16: ");
  eeprom +=(getFloat(16)) + SALTO_LINEA;
  eeprom +=("20: ");
  eeprom +=(getFloat(20)) + SALTO_LINEA;
  eeprom +=("MPU_ACC_BASE") + SALTO_LINEA;
  eeprom +=("24: ");
  eeprom +=(getFloat(24)) + SALTO_LINEA;
  eeprom +=("28: ");
  eeprom +=(getFloat(28)) + SALTO_LINEA;
  eeprom +=("32: ");
  eeprom +=(getFloat(32)) + SALTO_LINEA;
  eeprom +=("MPU_GYRO_BASE") + SALTO_LINEA;
  eeprom +=("36: ");
  eeprom +=(getFloat(36)) + SALTO_LINEA;
  eeprom +=("40: ");
  eeprom +=(getFloat(40)) + SALTO_LINEA;
  eeprom +=("44: ");
  eeprom +=(getFloat(44)) + SALTO_LINEA;

  return eeprom;
};

String memoryInit(){

  //HMC_GAIN_OFFSET 1.03485,1.05958,1.04591
  floatToMemory(EEPROM_HMC_GAIN_OFFSET_X,1.03485);
  floatToMemory(EEPROM_HMC_GAIN_OFFSET_Y,1.05958);
  floatToMemory(EEPROM_HMC_GAIN_OFFSET_Z,1.04591);
  
  //HMC_OFFSET (-24.75360,143.29776,66.39450)
  floatToMemory(EEPROM_HMC_OFFSET_X,-24.75360);
  floatToMemory(EEPROM_HMC_OFFSET_Y,143.29776);
  floatToMemory(EEPROM_HMC_OFFSET_Z,66.39450);
  //MPU_ACC_BASE
  floatToMemory(EEPROM_MPU_ACC_BASE_X,1023.00);
  floatToMemory(EEPROM_MPU_ACC_BASE_Y,-539.00);
  floatToMemory(EEPROM_MPU_ACC_BASE_Z,-17171.00);
  //MPU_GYRO_BASE
  floatToMemory(EEPROM_MPU_GYRO_BASE_X,-307.00);
  floatToMemory(EEPROM_MPU_GYRO_BASE_Y, 98.00);
  floatToMemory(EEPROM_MPU_GYRO_BASE_Z,-8.00);

  return "Memoria Cargada";
};
