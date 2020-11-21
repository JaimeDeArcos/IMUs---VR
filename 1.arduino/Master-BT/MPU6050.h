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


//VALORES DE CALIBRACION(cada sensor deberia de llevar los suyos)
#define ACC_BASE_X        650
#define ACC_BASE_Y        -177
#define ACC_BASE_Z        15679
#define GYRO_BASE_X        -1
#define GYRO_BASE_Y        -76
#define GYRO_BASE_Z        -54

const float GRADOS_A_RADIANES = 3.14159 / 180;


#define X 0
#define Y 1
#define Z 2

typedef union reg16Bit
{
  struct {
    uint8_t lByte;
    uint8_t hByte;
  } reg;

  int16_t value;

};
 
class MPU6050 {

  public:

    int16_t   accRaw[3];
    int16_t   gyroRaw[3];
    int16_t   tempRaw;
 
    int   accBase[3];     //Valor de calibracion
    int   gyroBase[3];    //Valor de calibracion

    float gyroGrados[3];
    float gyroRadianes[3];
  
    //------------------------------------------------------------
    String init() {

      String ret = "";

      //TODO: REFINAR a ver que es necesario
      //Test conexion I2C del sensor
      byte b = getRegister(MPU6050_WHO_AM_I);   //Lee MPU6050_WHO_AM_I (deberia ser 0x68)
      ret += ("MPU testI2c = ");
      ret += (b, HEX);
      ret += "\n";
      
      delay(100);
      
      //Test Power ON
      b = getRegister(MPU6050_PWR_MGMT_1);   //Lee MPU6050_PWR_MGMT_1
      ret += ("MPU PWR_MGMT_1 = ");
      ret += (b, BIN);
      delay(100);

      //PWR_MGMT_1    -- DEVICE_RESET 1
      I2C_writeRegister(MPU6050_DIR, MPU6050_PWR_MGMT_1, B10000000);
      delay(100);

      //Test Power ON
      b = getRegister(MPU6050_PWR_MGMT_1);   //Lee MPU6050_PWR_MGMT_1
      ret += ("MPU PWR_MGMT_1 = ");
      ret += (b, BIN);
      delay(100);

      //PWR_MGMT_1    -- Salir del sleep
      I2C_writeRegister(MPU6050_DIR, MPU6050_PWR_MGMT_1, B00000000);
      delay(100);

      //Test Power ON (OTRA VEZ?)
      b = getRegister(MPU6050_PWR_MGMT_1);   
      ret += ("MPU PWR_MGMT_1 = ");
      ret += (b, BIN);
      delay(100);

      getOffsets();

      return ret;
    };
    //------------------------------------------------------------
    String getRaw() { 
      //Acelerometro
      accRaw[0] = read_two (MPU6050_ACCEL_XOUT_H);
      accRaw[1] = read_two (MPU6050_ACCEL_YOUT_H);
      accRaw[2] = read_two (MPU6050_ACCEL_ZOUT_H);
      //Giroscopio
      gyroRaw[0] = read_two (MPU6050_GYRO_XOUT_H);
      gyroRaw[1] = read_two (MPU6050_GYRO_YOUT_H);
      gyroRaw[2] = read_two (MPU6050_GYRO_ZOUT_H);

      String ret = "";
      
      #ifdef DEBUG_MPU
      ret += ("MPU.getRaw:");
      ret += ("Acc["); 
      ret += (accRaw[0]); ret += (",");
      ret += (accRaw[1]); ret += (",");
      ret += (accRaw[2]); ret += ("]");

      ret += ("Gyro["); 
      ret += (gyroRaw[0]); ret += (",");
      ret += (gyroRaw[1]); ret += (",");
      ret += (gyroRaw[2]); ret += ("]");
      #endif

      return ret;
    };
    //------------------------------------------------------------
    void getTemp() {
      tempRaw = read_two  (MPU6050_TEMP_OUT_H);
    }; 
    //------------------------------------------------------------
    void update()
    { 
      getRaw();

      // Convertir los valores del gyro a grados/segundo
      float FS_SEL = 131;
      gyroGrados[X] = (gyroRaw[X] - gyroBase[X]) / FS_SEL;
      gyroGrados[Y] = (gyroRaw[Y] - gyroBase[Y]) / FS_SEL;
      gyroGrados[Z] = (gyroRaw[Z] - gyroBase[Z]) / FS_SEL;
 
      gyroRadianes[X] = gyroGrados[X] * GRADOS_A_RADIANES;
      gyroRadianes[Y] = gyroGrados[Y] * GRADOS_A_RADIANES;
      gyroRadianes[Z] = gyroGrados[Z] * GRADOS_A_RADIANES;
    };
    //------------------------------------------------------------
    void getOffsets() {

      accBase[X]  = getFloat(EEPROM_MPU_ACC_BASE_X);
      accBase[Y]  = getFloat(EEPROM_MPU_ACC_BASE_Y);
      accBase[Z]  = getFloat(EEPROM_MPU_ACC_BASE_Z);
      gyroBase[X] = getFloat(EEPROM_MPU_GYRO_BASE_X);
      gyroBase[Y] = getFloat(EEPROM_MPU_GYRO_BASE_Y);
      gyroBase[Z] = getFloat(EEPROM_MPU_GYRO_BASE_Z);
    };
    //************************************************************
    //                           I2C
    //------------------------------------------------------------ 
     byte getRegister(byte reg) {
      return I2C_getRegister (MPU6050_DIR, reg);
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











