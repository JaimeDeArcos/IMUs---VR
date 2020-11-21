#include <Arduino.h>

//NUMERO IDENTIFICATIVO I2C (1 byte)
#define EEPROM_I2C_ADDRESS         48                    

//VALORES DE CALIBRACION HMC
#define EEPROM_HMC_GAIN_OFFSET_X   0
#define EEPROM_HMC_GAIN_OFFSET_Y   4
#define EEPROM_HMC_GAIN_OFFSET_Z   8
#define EEPROM_HMC_OFFSET_X        12
#define EEPROM_HMC_OFFSET_Y        16
#define EEPROM_HMC_OFFSET_Z        20

//VALORES DE CALIBRACION MPU
#define EEPROM_MPU_ACC_OFFSET_X      24
#define EEPROM_MPU_ACC_OFFSET_Y      28
#define EEPROM_MPU_ACC_OFFSET_Z      32
#define EEPROM_MPU_GYRO_OFFSET_X     36
#define EEPROM_MPU_GYRO_OFFSET_Y     40
#define EEPROM_MPU_GYRO_OFFSET_Z     44

  
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
 
   unionFB.datoF =dato ;  
   
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
 
  String eeprom = SALTO_LINEA + ("MEMORIA EEPROM: ") + SALTO_LINEA;
 
  eeprom +=("HMC_GAIN_OFFSET") + SALTO_LINEA;
  eeprom += TAB + ("00: ") + (getFloat(0)) + SALTO_LINEA;
  eeprom += TAB + ("04: ") + (getFloat(4)) + SALTO_LINEA;
  eeprom += TAB + ("08: ") + (getFloat(8)) + SALTO_LINEA;
  eeprom +=("HMC_OFFSET") + SALTO_LINEA;
  eeprom += TAB + ("12: ") + (getFloat(12)) + SALTO_LINEA;
  eeprom += TAB + ("16: ") + (getFloat(16)) + SALTO_LINEA;
  eeprom += TAB + ("20: ") + (getFloat(20)) + SALTO_LINEA;
  eeprom +=("MPU_ACC_OFFSET") + SALTO_LINEA;
  eeprom += TAB + ("24: ") + (getFloat(24)) + SALTO_LINEA;
  eeprom += TAB + ("28: ") + (getFloat(28)) + SALTO_LINEA;
  eeprom += TAB + ("32: ") + (getFloat(32)) + SALTO_LINEA;
  eeprom +=("MPU_GYRO_OFFSET") + SALTO_LINEA;
  eeprom += TAB + ("36: ") + (getFloat(36)) + SALTO_LINEA;
  eeprom += TAB + ("40: ") + (getFloat(40)) + SALTO_LINEA;
  eeprom += TAB + ("44: ") + (getFloat(44)) + SALTO_LINEA;

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
  //MPU_ACC_OFFSET
  floatToMemory(EEPROM_MPU_ACC_OFFSET_X,1023.00);
  floatToMemory(EEPROM_MPU_ACC_OFFSET_Y,-539.00);
  floatToMemory(EEPROM_MPU_ACC_OFFSET_Z,-17171.00);
  //MPU_GYRO_OFFSET
  floatToMemory(EEPROM_MPU_GYRO_OFFSET_X,-307.00);
  floatToMemory(EEPROM_MPU_GYRO_OFFSET_Y, 98.00);
  floatToMemory(EEPROM_MPU_GYRO_OFFSET_Z,-8.00);

  return "Memoria Cargada"; 
};

String memoryZero(){
  //HMC_GAIN_OFFSET 1.03485,1.05958,1.04591
  floatToMemory(EEPROM_HMC_GAIN_OFFSET_X,1);
  floatToMemory(EEPROM_HMC_GAIN_OFFSET_Y,1);
  floatToMemory(EEPROM_HMC_GAIN_OFFSET_Z,1);
  
  //HMC_OFFSET (-24.75360,143.29776,66.39450)
  floatToMemory(EEPROM_HMC_OFFSET_X,0);
  floatToMemory(EEPROM_HMC_OFFSET_Y,0);
  floatToMemory(EEPROM_HMC_OFFSET_Z,0);
  //MPU_ACC_OFFSET
  floatToMemory(EEPROM_MPU_ACC_OFFSET_X,0);
  floatToMemory(EEPROM_MPU_ACC_OFFSET_Y,0);
  floatToMemory(EEPROM_MPU_ACC_OFFSET_Z,0);
  //MPU_GYRO_OFFSET
  floatToMemory(EEPROM_MPU_GYRO_OFFSET_X,0);
  floatToMemory(EEPROM_MPU_GYRO_OFFSET_Y,0);
  floatToMemory(EEPROM_MPU_GYRO_OFFSET_Z,0);

  return "Memoria BORRADA"; 
};



