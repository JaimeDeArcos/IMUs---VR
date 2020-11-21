/* *************************************************************************
 * -----------------------------MPU6050------------------------------------*
 *                                                                         *
 * Autor: Jaime de Arcos Sánchez                                           *
 * Fecha: 1 Mayo 2016                                                      *
 *                                                                         *
 * Libreria para el dispositivo MPU6050(acelerometro + Giroscopo)          *
 *                                                                         *
 * FUNCIONES:                                                              *
 *  - testI2C()                                                            *
 *  - reset()                                                              *
 *  - wakeUp()                                                             *
 *  - init()                                                               *
 *  - getRaw()   - Lee los registros de los sensores                       *
 *  - getTemp()  - Lee los registros de los sensores                       *
 *  - update()                                                             *
 *  - getOfsets()                                                          *
 *  - setOffsetsGyro()                                                     *
 *  - setOffsetsAcc()                                                      *
 *  - calibrarOffsets()                                                    *
 *  - setRangeGyro()                                                       *
 *  - setRangeAcc()                                                        *
 *  - setDLPF()                                                            * 
 *  - toString()                                                           * 
 *  - rawToString()                                                        * 
 ***************************************************************************
     FUNCIONES I2C SOFTWARE
      - read_one()
      - read_two()
      - write_one()
 ***************************************************************************/

//=================== REGISTROS MPU6050 ====================================

//Registros de inicializacion
#define MPU6050_DIR          0xD0   // 0x68 DIR I2C del MPU6050 
#define MPU6050_WHO_AM_I     0x75   // R
#define MPU6050_PWR_MGMT_1   0x6B   // R/W
#define MPU6050_PWR_MGMT_2   0x6C   // R/W

//RAW values (2 Bytes por eje)
//Acelerometro
#define MPU6050_ACCEL_XOUT_H       0x3B   // R  
#define MPU6050_ACCEL_XOUT_L       0x3C   // R  
#define MPU6050_ACCEL_YOUT_H       0x3D   // R  
#define MPU6050_ACCEL_YOUT_L       0x3E   // R  
#define MPU6050_ACCEL_ZOUT_H       0x3F   // R  
#define MPU6050_ACCEL_ZOUT_L       0x40   // R  
//Temperatura
#define MPU6050_TEMP_OUT_H         0x41   // R  
#define MPU6050_TEMP_OUT_L         0x42   // R  
//Giroscopo
#define MPU6050_GYRO_XOUT_H        0x43   // R  
#define MPU6050_GYRO_XOUT_L        0x44   // R  
#define MPU6050_GYRO_YOUT_H        0x45   // R  
#define MPU6050_GYRO_YOUT_L        0x46   // R  
#define MPU6050_GYRO_ZOUT_H        0x47   // R  
#define MPU6050_GYRO_ZOUT_L        0x48   // R  

//OFFSETS (calibracion)
#define MPU6050_RA_XA_OFFS_H        0x06 //[15:0] XA_OFFS
#define MPU6050_RA_XA_OFFS_L_TC     0x07
#define MPU6050_RA_YA_OFFS_H        0x08 //[15:0] YA_OFFS
#define MPU6050_RA_YA_OFFS_L_TC     0x09
#define MPU6050_RA_ZA_OFFS_H        0x0A //[15:0] ZA_OFFS
#define MPU6050_RA_ZA_OFFS_L_TC     0x0B

//FSYC Y DLPF
#define MPU6050_RA_CONFIG           0x1A

//CONFIGURACION RANGO/ESCALA
#define MPU6050_RA_GYRO_CONFIG      0x1B  //bit 4-3 F_SEL
#define MPU6050_RA_ACCEL_CONFIG     0x1C  //bit 4-3 AF_SEL

//CONGIG RANGO Gyro (para setRangeGyro) 
#define MPU_RANGE_GYRO_250 0
#define MPU_RANGE_GYRO_500 1
#define MPU_RANGE_GYRO_1000 2
#define MPU_RANGE_GYRO_2000 3 

 //CONGIG RANGO ACC (para setRangeAcc)
#define MPU_RANGE_ACC_2  0 
#define MPU_RANGE_ACC_4  1 
#define MPU_RANGE_ACC_8  2 
#define MPU_RANGE_ACC_16 3

//CONFIG DLPF(low pass filter) para setDLPF()
#define MPU_DLPF_0    0
#define MPU_DLPF_184  1
#define MPU_DLPF_94   2
#define MPU_DLPF_44   3
#define MPU_DLPF_21   4
#define MPU_DLPF_10   5
#define MPU_DLPF_5    6 


 
const float GRADOS_A_RADIANES = 3.14159 / 180;
 
//=================== TIPO DATOS AUXILIAR ==============================
typedef union reg16Bit
{
  struct {
    uint8_t lByte;
    uint8_t hByte;
  } reg;
  int16_t value;
};
//======================================================================

class MPU6050 {

  public:
  
    // Acelerometro
    int16_t   accRaw[3];
    int16_t   accOffset[3];     //Valor de calibracion 
    float     accFuerzaG[3];
    float     LSB_2_G;         // Conversion (LSB) Bits -> Fuerza G

    // Giroscopo
    int16_t   gyroRaw[3]; 
    int16_t   gyroOffset[3];    //Valor de calibracion
    float     gyroGrados[3];
    float     gyroRadianes[3];

    float     LSB_2_GradosXSegundo;        // Conversion (LSB) Bits -> º/s
    
    //Temperatura
    int16_t   tempRaw;
    float     tempGrados;  
    
    //------------------------------------------------------------ 
    bool testI2C() {
      //Lee MPU6050_WHO_AM_I (deberia ser 0x68)
      return (read_one(MPU6050_WHO_AM_I) == 0x68);
    };
    //------------------------------------------------------------
    void reset() { 
      write_one(MPU6050_PWR_MGMT_1, B10000000); 
    };
    //------------------------------------------------------------
    void wakeUp() {
      //Sleep Mode OFF
      write_one(MPU6050_PWR_MGMT_1, B00000000); 
    }; 
    //------------------------------------------------------------ 
    //void init(int rangeGyro, int rangeAcc, int DLPF) { 
    void init() { 
      testI2C(); 
      delay(50);
        
      //PWR_MGMT_1    -- DEVICE_RESET 1
      reset();
      delay(300);
        
      //PWR_MGMT_1    -- Salir del sleep
      wakeUp();
      delay(100);

      testI2C(); 
      delay(50);

      //setRangeGyro(rangeGyro);  //±250 °/s 
      setRangeGyro(MPU_RANGE_GYRO_500);   
      delay(100);
      
      //setRangeAcc(rangeAcc);   //  ±2 g
      setRangeAcc(MPU_RANGE_ACC_4);    
      delay(100);
       
      //setDLPF(DLPF);
      setDLPF(MPU_DLPF_184);
      delay(100);

      getOffsets(); 
    }; 
    //------------------------------------------------------------
    void getRaw() { 
      //Acelerometro
      accRaw[0] = read_two (MPU6050_ACCEL_XOUT_H);
      accRaw[1] = read_two (MPU6050_ACCEL_YOUT_H);
      accRaw[2] = read_two (MPU6050_ACCEL_ZOUT_H);
      //Giroscopio
      gyroRaw[0] = read_two (MPU6050_GYRO_XOUT_H);
      gyroRaw[1] = read_two (MPU6050_GYRO_YOUT_H);
      gyroRaw[2] = read_two (MPU6050_GYRO_ZOUT_H);
    };
    //------------------------------------------------------------
    void getTemp() {
      tempRaw = read_two  (MPU6050_TEMP_OUT_H); 
      tempGrados = (tempRaw / 340) + 36.53;
    }; 
    //------------------------------------------------------------
    void update()
    {    
      
      getRaw();

      //Corregir Offset
      gyroRaw[X] -= gyroOffset[X];
      gyroRaw[Y] -= gyroOffset[Y];
      gyroRaw[Z] -= gyroOffset[Z];
 
      //Gyro RAW --> Grados/s 
      gyroGrados[X] = gyroRaw[X] / LSB_2_GradosXSegundo;
      gyroGrados[Y] = gyroRaw[Y] / LSB_2_GradosXSegundo;
      gyroGrados[Z] = gyroRaw[Z] / LSB_2_GradosXSegundo;
 
      //Grados/s a Rad/s
      gyroRadianes[X] = gyroGrados[X] * GRADOS_A_RADIANES;
      gyroRadianes[Y] = gyroGrados[Y] * GRADOS_A_RADIANES;
      gyroRadianes[Z] = gyroGrados[Z] * GRADOS_A_RADIANES;

      //Corregir Offset
      accRaw[X] -= accOffset[X];
      accRaw[Y] -= accOffset[Y];
      accRaw[Z] -= accOffset[Z];
      
      //Acc RAW --> Fuerza G
      accFuerzaG[X] = accRaw[X] / LSB_2_G;
      accFuerzaG[Y] = accRaw[Y] / LSB_2_G;
      accFuerzaG[Z] = accRaw[Z] / LSB_2_G;
    };
    //------------------------------------------------------------
    void getOffsets() { 
      accOffset[X]  = getFloat(EEPROM_MPU_ACC_OFFSET_X);
      accOffset[Y]  = getFloat(EEPROM_MPU_ACC_OFFSET_Y);
      accOffset[Z]  = getFloat(EEPROM_MPU_ACC_OFFSET_Z);
      gyroOffset[X] = getFloat(EEPROM_MPU_GYRO_OFFSET_X);
      gyroOffset[Y] = getFloat(EEPROM_MPU_GYRO_OFFSET_Y);
      gyroOffset[Z] = getFloat(EEPROM_MPU_GYRO_OFFSET_Z);

      // Si la Memoria esta vacia o corrupta devuelve NaN
      for (int i=0;i<=2;i++){
         if (isnan(accOffset[i])  ==1)  accOffset [i]=0;
         if (isnan(gyroOffset[i]) ==1)  gyroOffset[i]=1; 
      }   
    };
        //------------------------------------------------------------
    void setRangeGyro(int FS_SEL) {

      /**************************************************
        FSSEL   -   Full Scale Range   - LSB sensitivity
          0              ±250°/s            131 LSB/º/s
          1              ±500°/s           65.5 LSB/º/s
          2             ±1000°/s           32.8 LSB/º/s
          3             ±2000°/s           16.4 LSB/º/s
      ***************************************************/ 
      byte byteFSEL = read_one(MPU6050_RA_GYRO_CONFIG);  
      byte result = (byteFSEL & B11100111);
  
      switch (FS_SEL) {
        case 0: result |= B00000000;
          LSB_2_GradosXSegundo =  131; 
          break;
        case 1: result |= B00001000;
          LSB_2_GradosXSegundo =  65.5; 
          break;
        case 2: result |= B00010000;
          LSB_2_GradosXSegundo =  32.8; 
          break;
        case 3: result |= B00011000;
          LSB_2_GradosXSegundo =  16.4; 
          break;
      }
      write_one(MPU6050_RA_GYRO_CONFIG , result); 

      byteFSEL = read_one(MPU6050_RA_GYRO_CONFIG); 
    }; 
    //------------------------------------------------------------
    void setRangeAcc(int FS_SEL) { 
      /**************************************************
         FSSEL   -   Full Scale Range   - LSB sensitivity
           0              ±2g              16384 LSB/g
           1              ±4g               8192 LSB/g
           2              ±8g               4096 LSB/g
           3             ±16g               2048 LSB/g
       ***************************************************/

      byte byteFSEL = read_one(MPU6050_RA_ACCEL_CONFIG);  
      byte result = (byteFSEL & B11100111); 

      switch (FS_SEL) {
        case 0: result |= B00000000;
          LSB_2_G =  16384; 
          break;
        case 1: result |= B00001000;
          LSB_2_G =  8192; 
          break;
        case 2: result |= B00010000;
          LSB_2_G =  4096; 
          break;
        case 3: result |= B00011000;
          LSB_2_G =  2048; 
          break;
      }
      write_one(MPU6050_RA_ACCEL_CONFIG , result);

      byteFSEL = read_one(MPU6050_RA_ACCEL_CONFIG);  
    };
    //------------------------------------------------------------
    void setDLPF(byte bandWidth)  {

      if (bandWidth < 0 || bandWidth > 6) bandWidth = 0;

      /* Al ser los 3 menos significativos y como FSYC no nos interesa(=0)
          Podemos meter directamente el byte
                     [--]  [FSYNC]  [  DLPF  ]
          REG 26 --  x  x  0  0  0  F2  F1  F0

         F2 F1 F0    Bandwidth [Hz]
         0  0  0
         0  0  1      184
         0  1  0      94
         0  1  1      44
         1  0  0      21
         1  0  1      10
         1  1  0      5 
      */
      write_one(MPU6050_RA_CONFIG, bandWidth);
    }; 
    //------------------------------------------------------------
    String calibrar()  {
      //LEER MUCHAS MEDIDAS
      int    num_readings = 100;
      long   xAcc = 0;
      long   yAcc = 0;
      long   zAcc = 0;
      long   xGyro  = 0;
      long   yGyro  = 0;
      long   zGyro  = 0;
  
      // Descartar la primera lectura
      getRaw();
 
      // Leer y hacer media de los valores raw del MPU6050
      for (int i = 0; i < num_readings; i++) {
        
        getRaw();
  
        xAcc   += accRaw[X];
        yAcc   += accRaw[Y];
        zAcc   += accRaw[Z];
        xGyro  += gyroRaw[X];
        yGyro  += gyroRaw[Y];
        zGyro  += gyroRaw[Z];
        delay(40);
      }
 
      xGyro  /= num_readings;
      yGyro  /= num_readings;
      zGyro  /= num_readings;

      xAcc /= num_readings;
      yAcc /= num_readings;
      zAcc /= num_readings;

      //OJO Aqui sumamos 1G, al estar en horizontal al calibrar
      //toda la gravedad(1g) se aplica sobre el eje Z y 
      // no interesa eliminarlo 
      zAcc = LSB_2_G - zAcc ;

      String info = "OFFSETS:" + SALTO_LINEA;
      
      info += TAB + ("Acc[") + (xAcc)   + COMA + (yAcc)  + COMA + (zAcc)  + ("]") + SALTO_LINEA;  
      info += TAB + ("Gyro[") + (xGyro) + COMA + (yGyro) + COMA + (zGyro) + ("]") + SALTO_LINEA;  
 
      setOffsetsAcc(xAcc,yAcc,zAcc);
      setOffsetsGyro(xGyro,yGyro,zGyro);
      
      return info;
    }; 
    //------------------------------------------------------------
    void setOffsetsGyro(int16_t offX, int16_t offY, int16_t offZ) {
        gyroOffset[X] = offX;
        gyroOffset[Y] = offY;
        gyroOffset[Z] = offZ;

        floatToMemory(EEPROM_MPU_GYRO_OFFSET_X , gyroOffset[X]);
        floatToMemory(EEPROM_MPU_GYRO_OFFSET_Y , gyroOffset[Y]);
        floatToMemory(EEPROM_MPU_GYRO_OFFSET_Z , gyroOffset[Z]); 
    };
    //------------------------------------------------------------
    void setOffsetsAcc(int16_t offX, int16_t offY, int16_t offZ) {
        accOffset[X] = offX;
        accOffset[Y] = offY;
        accOffset[Z] = offZ;

        floatToMemory(EEPROM_MPU_ACC_OFFSET_X , accOffset[X]);
        floatToMemory(EEPROM_MPU_ACC_OFFSET_Y , accOffset[Y]);
        floatToMemory(EEPROM_MPU_ACC_OFFSET_Z , accOffset[Z]); 
    };
    //------------------------------------------------------------
    String valuesToString() {
  
      String values =""; 
      values += gyroGrados[X] + COMA;
      values += gyroGrados[Y] + COMA;
      values += gyroGrados[Z] + COMA;
      
      values += accFuerzaG[X] + COMA;
      values += accFuerzaG[Y] + COMA;
      values += accFuerzaG[Z] + COMA; 
  
      return values ;
    };  
    //------------------------------------------------------------
    String rawToString() {
  
      String raw =""; 
      
      raw+= gyroRaw[X] + COMA;
      raw+= gyroRaw[Y] + COMA;
      raw+= gyroRaw[Z] + COMA;
      
      raw+= accRaw[X] + COMA;
      raw+= accRaw[Y] + COMA;
      raw+= accRaw[Z] + COMA; 
  
      return raw;
    };  
    //************************************************************
    //                           I2C
    //------------------------------------------------------------ 
     byte read_one(byte reg) {
        i2c_start(MPU6050_DIR | I2C_WRITE );
        i2c_write(reg); 
        i2c_rep_start(MPU6050_DIR | I2C_READ);
        byte oneByte = i2c_read (true); //read one byte
        i2c_stop();

        return oneByte; 
    }; 
    //------------------------------------------------------------
    int16_t read_two(uint8_t regAddr)
    { 
      reg16Bit datos;

      i2c_start(MPU6050_DIR | I2C_WRITE );
      i2c_write(regAddr);
      i2c_rep_start(MPU6050_DIR | I2C_READ);
      datos.reg.hByte = i2c_read (false);
      datos.reg.lByte = i2c_read (true);
      i2c_stop();
      
      return datos.value;
    };
    //------------------------------------------------------------
    void write_one(int regAddr, int pData)
    {
      i2c_start(MPU6050_DIR | I2C_WRITE);
      i2c_write(regAddr);
      i2c_write(pData);
      i2c_stop();
    };
    //------------------------------------------------------------
};


/*
  void Setup_MPU6050()
  {
   //Sets sample rate to 8000/1+7 = 1000Hz
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_SMPLRT_DIV, 0x07);
   //Disable FSync, 256Hz DLPF
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_CONFIG, 0x00);
   //Disable gyro self tests, scale of 500 degrees/s
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_GYRO_CONFIG, 0b00001000);
   //Disable accel self tests, scale of +-2g, no DHPF
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG, 0x00);
   //Freefall threshold of |0mg|
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_FF_THR, 0x00);
   //Freefall duration limit of 0
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_FF_DUR, 0x00);
   //Motion threshold of 0mg
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_MOT_THR, 0x00);
   //Motion duration of 0s
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_MOT_DUR, 0x00);
   //Zero motion threshold
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_ZRMOT_THR, 0x00);
   //Zero motion duration threshold
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_ZRMOT_DUR, 0x00);
   //Disable sensor output to FIFO buffer
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_FIFO_EN, 0x00);

   //AUX I2C setup
   //Sets AUX I2C to single master control, plus other config
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_MST_CTRL, 0x00);
   //Setup AUX I2C slaves
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_ADDR, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_REG, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_CTRL, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_ADDR, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_REG, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_CTRL, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_ADDR, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_REG, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_CTRL, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_ADDR, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_REG, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_CTRL, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_ADDR, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_REG, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_DO, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_CTRL, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_DI, 0x00);

   //MPU6050_RA_I2C_MST_STATUS //Read-only
   //Setup INT pin and AUX I2C pass through
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_INT_PIN_CFG, 0x00);
   //Enable data ready interrupt
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE, 0x00);

   //MPU6050_RA_DMP_INT_STATUS        //Read-only
   //MPU6050_RA_INT_STATUS 3A        //Read-only
   //MPU6050_RA_ACCEL_XOUT_H         //Read-only
   //MPU6050_RA_ACCEL_XOUT_L         //Read-only
   //MPU6050_RA_ACCEL_YOUT_H         //Read-only
   //MPU6050_RA_ACCEL_YOUT_L         //Read-only
   //MPU6050_RA_ACCEL_ZOUT_H         //Read-only
   //MPU6050_RA_ACCEL_ZOUT_L         //Read-only
   //MPU6050_RA_TEMP_OUT_H         //Read-only
   //MPU6050_RA_TEMP_OUT_L         //Read-only
   //MPU6050_RA_GYRO_XOUT_H         //Read-only
   //MPU6050_RA_GYRO_XOUT_L         //Read-only
   //MPU6050_RA_GYRO_YOUT_H         //Read-only
   //MPU6050_RA_GYRO_YOUT_L         //Read-only
   //MPU6050_RA_GYRO_ZOUT_H         //Read-only
   //MPU6050_RA_GYRO_ZOUT_L         //Read-only
   //MPU6050_RA_EXT_SENS_DATA_00     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_01     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_02     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_03     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_04     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_05     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_06     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_07     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_08     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_09     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_10     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_11     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_12     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_13     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_14     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_15     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_16     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_17     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_18     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_19     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_20     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_21     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_22     //Read-only
   //MPU6050_RA_EXT_SENS_DATA_23     //Read-only
   //MPU6050_RA_MOT_DETECT_STATUS     //Read-only

   //Slave out, dont care
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_DO, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_DO, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_DO, 0x00);
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_DO, 0x00);
   //More slave config
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_I2C_MST_DELAY_CTRL, 0x00);
   //Reset sensor signal paths
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_SIGNAL_PATH_RESET, 0x00);
   //Motion detection control
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_MOT_DETECT_CTRL, 0x00);
   //Disables FIFO, AUX I2C, FIFO and I2C reset bits to 0
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_USER_CTRL, 0x00);
   //Sets clock source to gyro reference w/ PLL
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, 0b00000010);
   //Controls frequency of wakeups in accel low power mode plus the sensor standby modes
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_2, 0x00);
   //MPU6050_RA_BANK_SEL            //Not in datasheet
   //MPU6050_RA_MEM_START_ADDR        //Not in datasheet
   //MPU6050_RA_MEM_R_W            //Not in datasheet
   //MPU6050_RA_DMP_CFG_1            //Not in datasheet
   //MPU6050_RA_DMP_CFG_2            //Not in datasheet
   //MPU6050_RA_FIFO_COUNTH        //Read-only
   //MPU6050_RA_FIFO_COUNTL        //Read-only
   //Data transfer to and from the FIFO buffer
   LDByteWriteI2C(MPU6050_ADDRESS, MPU6050_RA_FIFO_R_W, 0x00);
   //MPU6050_RA_WHO_AM_I             //Read-only, I2C address

   printf("\nMPU6050 Setup Complete");
  }


*/











