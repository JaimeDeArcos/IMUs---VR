/* *************************************************************************
 * -----------------------------HMC5883L-----------------------------------*
 *                                                                         *
 * Autor: Jaime de Arcos Sánchez                                           *
 * Fecha: 1 Julio 2014                                                     *
 *                                                                         *
 * Libreria para la adquisicion de datos de un sensor HMC5883L             *
 *                                                                         *
 * FUNCIONES:                                                              *
 *  - init()      - Configuracion inicial                                  *
 *  - update()    - Calcula mGauss desde los Raw                           *
 *  - setGain()   - Seleccionar la escala: ± 0.8/1.3/1.9/2.5/4/4.7/5.6/8.1 *
 *  - calibrar()  - Calibracion del sensor (OJO anotar los valores a mano) *
 *  - i2c_write_one() - I2C: escribir un registro                              *
 *  -  i2c_read_one()  - I2C: leer un registro                                  *
 *  - getRAW()    - I2C: Lee los 6 registros de la medida del la brujula   *  
 *                                                                         *
 ***************************************************************************/

#include <Arduino.h> 

// Definiciones------------------------------------------------------------
#define HMC5883L_DIR_8        0x3C      // Direccion I2C de 8-bit del HMC5883L
#define HMC5883L_DIR_7        0x1E      // Direccion I2C de 8-bit del HMC5883L

#define compass_XY_excitation 1160      // Excitacion del Campo magnetico 
#define compass_Z_excitation  1080      // durante el Auto-Test (Calibracion)

#define RAD_A_GRADOS          57.29577

#define GAIN_OFFSET_X         1.11253
#define GAIN_OFFSET_Y         1.15630
#define GAIN_OFFSET_Z         1.02261

#define OFFSET_X             -5.11765      //-27.12338
#define OFFSET_Y              146.27194    //56.38119
#define OFFSET_Z              -5.17441     //47.51050

#define HMC5883L_RETARDO      100  //En microsegundos 
//REGISTROS-----------------------------------------------------------------
#define CONFIG_A              0x00 //R/W
#define CONFIG_B              0x01 //R/W
#define MODE                  0x02 //R/W
#define MODE_MEDIDA_UNICA     0x01 //W
#define MODE_MEDIDA_CONTINUAS 0x00 //W

#define X_MSB                 0x03    //R
#define Y_MSB                 0x07    //R
#define Z_MSB                 0x05    //R
//--------------------------------------------------------------------------
class HMC5883L
{
private:

  float gainError[3]; // Desviacion de la ganacia
  float offset[3];    // Desviacion en las lecturas
  float LSB_2_Gauss;  // Ganancia en miliGauss/LSb 

public:

  int   raw[3];       // Valores entre (-500,+500)
  float mGauss[3];    // miliGauss
 
  //------------------------------------------------------------------------
  void init(){

    //Config Register A: 
    //(8-muestras, 15 Hz , normal measurement)
    i2c_write_one(CONFIG_A, B01110000);
   
    //Config Register B:
    //(Gain=001, ±1,3 Gauss, 0.92 miliGaus/LSB)
    setRange(1.3);
  
    for (int i=0; i<3 ; i++){
      offset[i]    = 0;
      gainError[i] = 1;
    }

    getOffsets();
    
  };
  //------------------------------------------------------------------------
  void getOffsets(){
    
    offset[X]    = getFloat(EEPROM_HMC_OFFSET_X);   
    offset[Y]    = getFloat(EEPROM_HMC_OFFSET_Y);
    offset[Z]    = getFloat(EEPROM_HMC_OFFSET_Z);
    gainError[X] = getFloat(EEPROM_HMC_GAIN_OFFSET_X);
    gainError[Y] = getFloat(EEPROM_HMC_GAIN_OFFSET_Y);
    gainError[Z] = getFloat(EEPROM_HMC_GAIN_OFFSET_Z);

    //Si la Memoria esta vacia o corrupta devuelve NaN
    for (int i=0;i<=2;i++){
      if (isnan(offset[i])    ==1)  offset[i]=0;
      if (isnan(gainError[i]) ==1)  gainError[i]=1; 
    }  
    
  };
  //------------------------------------------------------------------------
  void update(){

    //Modo unica medida
    i2c_write_one(MODE,MODE_MEDIDA_UNICA);
    
    delayMicroseconds(HMC5883L_RETARDO);

    getRaw();
    
    mGauss[X] = raw[X] * LSB_2_Gauss * gainError[X] + offset[X];
    mGauss[Y] = raw[Y] * LSB_2_Gauss * gainError[Y] + offset[Y];
    mGauss[Z] = raw[Z] * LSB_2_Gauss * gainError[Z] + offset[Z];
  };
  //------------------------------------------------------------------------
  void setRange(float rangeSEL){

    uint8_t regValue = 0x00;
    if     (rangeSEL == 0.88){  regValue = 0x00;   LSB_2_Gauss = 0.73;    }
    else if(rangeSEL == 1.3) {  regValue = 0x01;   LSB_2_Gauss = 0.92;    }
    else if(rangeSEL == 1.9) {  regValue = 0x02;   LSB_2_Gauss = 1.22;    }
    else if(rangeSEL == 2.5) {  regValue = 0x03;   LSB_2_Gauss = 1.52;    }
    else if(rangeSEL == 4.0) {  regValue = 0x04;   LSB_2_Gauss = 2.27;    }
    else if(rangeSEL == 4.7) {  regValue = 0x05;   LSB_2_Gauss = 2.56;    }
    else if(rangeSEL == 5.6) {  regValue = 0x06;   LSB_2_Gauss = 3.03;    }
    else if(rangeSEL == 8.1) {  regValue = 0x07;   LSB_2_Gauss = 4.35;    }
    else                     {  regValue = 0x01;   LSB_2_Gauss = 0.92;    }  

    // Configuracion 
    regValue = regValue << 5;
    i2c_write_one(CONFIG_B,regValue);
  };
  //------------------------------------------------------------------------
  String calibrar(int seleccion){
    // *********************************************************************
    // La funcion realiza 2 tareas, (Segun seleccion)
    // 1. Calcula la desviacion en la ganacia en cada uno de los ejes del
    //    magnetometro, usando la funcion de auto-excitacion del HMC5883L.
    // 2. Se calcula la media de cada uno de los ejes del campo magnético,
    //    cuando el magnetómetro se hace girar 360 grados.
    // 3. Realiza las dos tareas.
    // *********************************************************************  

    // *********************************************************************  
    // Calculo del Gain offset (desviacion de la ganacia) 
    // *********************************************************************   
    if (seleccion == 1){
      
        // Configurar el registro de Control para Positive Bias mode
        i2c_write_one(CONFIG_A, 0b01110001);
      
        // Desechar la primera lectura
        update();     

        // Leer solo datos positivos
        while(raw[X]<200 | raw[Y]<200 | raw[Z]<200){  
            update();
        }

        mGauss[X] = raw[X] * LSB_2_Gauss;
        mGauss[Y] = raw[Y] * LSB_2_Gauss;
        mGauss[Z] = raw[Z] * LSB_2_Gauss;

        // Offset = 1160 - Data_positive
        gainError[X] = (float)compass_XY_excitation / mGauss[X];
        gainError[Y] = (float)compass_XY_excitation / mGauss[Y];
        gainError[Z] = (float)compass_Z_excitation  / mGauss[Z];

        // Configurar el registro de Control para Negative Bias mode
        i2c_write_one(CONFIG_A, 0b01110010);

        update();  // Descartar la primera lectura

        // Leer solo datos negativos
        while(raw[X] > -200 | raw[Y]>-200 | raw[Z] > -200){
          update();
        }

        mGauss[X] = raw[X] * LSB_2_Gauss;
        mGauss[Y] = raw[Y] * LSB_2_Gauss;
        mGauss[Z] = raw[Z] * LSB_2_Gauss;

        // Calcular la media de las desviaciones (offsets)
        gainError[X] = (float)((compass_XY_excitation / abs(mGauss[X])) + gainError[X]) /2;
        gainError[Y] = (float)((compass_XY_excitation / abs(mGauss[Y])) + gainError[Y]) /2;
        gainError[Z] = (float)((compass_Z_excitation  / abs(mGauss[Z])) + gainError[Z]) /2;

        // Guardar en memoria
        floatToMemory(EEPROM_HMC_GAIN_OFFSET_X , gainError[X]);
        floatToMemory(EEPROM_HMC_GAIN_OFFSET_Y , gainError[Y]);
        floatToMemory(EEPROM_HMC_GAIN_OFFSET_Z , gainError[Z]);

        // Mostrar por Pantalla
        return String("Gain Offset: ") + gainError[X] + COMA + gainError[Y] +COMA+  gainError[Z]; 
    }

    // Configurando el registro Control para el modo normal
    i2c_write_one(CONFIG_A, 0b01111000);

    // *********************************************************************  
    // Calculo del Offset 
   // *********************************************************************  
    if (seleccion == 2){
 
       // Descartar las primeras 10 lecturas
       for(byte i=0;i<10;i++)    update();

      float x_max = -4000 , y_max =-4000 , z_max = -4000; 
      float x_min =  4000 , y_min = 4000 , z_min =  4000;  

      unsigned long t = millis();
      while(millis()-t <= 30000){       // Un minuto
         update();

         mGauss[X] = (float)raw[X] * LSB_2_Gauss * gainError[X];
         mGauss[Y] = (float)raw[Y] * LSB_2_Gauss * gainError[Y];
         mGauss[Z] = (float)raw[Z] * LSB_2_Gauss * gainError[Z];

         x_max = max(x_max,mGauss[X]);
         y_max = max(y_max,mGauss[Y]);
         z_max = max(z_max,mGauss[Z]);

         x_min = min(x_min,mGauss[X]);
         y_min = min(y_min,mGauss[Y]);
         z_min = min(z_min,mGauss[Z]);
      }

      offset[X] = ((x_max - x_min)/2) - x_max;
      offset[Y] = ((y_max - y_min)/2) - y_max;
      offset[Z] = ((z_max - z_min)/2) - z_max;
 
      floatToMemory(EEPROM_HMC_OFFSET_X , offset[X]);
      floatToMemory(EEPROM_HMC_OFFSET_Y , offset[Y]);
      floatToMemory(EEPROM_HMC_OFFSET_Z , offset[Z]);
      
      return String("Offset: ") + offset[X] + COMA + offset[Y] + COMA +  offset[Z]; 
    } 
    return "ERROR"; 
  }; 
  //------------------------------------------------------------------------
  void getRaw(){
    //Peticion
    i2c_start(HMC5883L_DIR_8 | I2C_WRITE );
    i2c_write(X_MSB);
    
     //Lectura de los 6 registros
    i2c_rep_start(HMC5883L_DIR_8 | I2C_READ);
    raw[X]  = i2c_read (false)<<8;
    raw[X] |= i2c_read (false);
    raw[Z]  = i2c_read (false)<<8;
    raw[Z] |= i2c_read (false);
    raw[Y]  = i2c_read (false)<<8;
    raw[Y] |= i2c_read (true);
    
    i2c_stop(); 
  };
  //------------------------------------------------------------------------
  String rawToString() {
    return  raw[X] + COMA +  raw[Y] + COMA +  raw[Z];
  }; 
   //------------------------------------------------------------------------
  String valuesToString() { 
    return  mGauss[X] + COMA +  mGauss[Y] + COMA + mGauss[Z];
  };  
  //------------------------------------------------------------------------ 
  //                      I2C Methods
  //------------------------------------------------------------------------ 
  void i2c_write_one(int regAddr, int pData)
  {
    i2c_start(HMC5883L_DIR_8 | I2C_WRITE);
    i2c_write(regAddr);
    i2c_write(pData);
    i2c_stop();
  };
  //------------------------------------------------------------------------
  int  i2c_read_one(uint8_t regAddr)
  {
    int data;
    //Peticion
    i2c_start(HMC5883L_DIR_8 | I2C_WRITE );
    i2c_write(regAddr);
    //Lectura
    i2c_rep_start(HMC5883L_DIR_8 | I2C_READ);
    data = i2c_read (true); //read one byte
    i2c_stop();

    return data;  // return byte value
  };
  //------------------------------------------------------------------------ 
};





