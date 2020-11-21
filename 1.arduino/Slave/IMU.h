/* *******************************************************************************
 * --------------------------------DCM-------------------------------------------*
 *                                                                               *
 * Autor: Jaime de Arcos Sánchez                                                 *
 * Fecha: 1 Junio 2016                                                           *
 *                                                                               *
 * Librería para el calculo de la DCM a partir de un IMU formado por:            *
 *                                                                               *
 * FUNCIONES:                                                                    *
 *  - init()          - Configuracion inicial                                    *
 *  - update()        - actualiza la DCM con valores de una nueva medida         *
 *  - RAWtoString()   - Imprime la DCM actualizada (Para avion.py)               *
 *  - DCMtoString()                                                              *                                                              
 *  - calibrarMPU()                                                              *
 *  - calibrarHMC()                                                              *
 *                                                                               *
 *  - printI()        - Imprime la estimacion de la posicion acc+Gyro(avion.py)  *
 *  - printValores()  - Imprime los valores en bruto de los sensores para        *
 *                      el cálculo en python (DCM v0.6.py)                       *
 *                                                                               *
 *********************************************************************************/
 
//----------------------------------------------------------------------------
class IMU
{
public:

  MPU6050  mpu;
  HMC5883L compass;

  float pesoGyro, pesoAcc, pesoMag;

  vector I,J,K; 
  
  float dcm[9]; 
 
  unsigned long t0,t1;
  //--------------------------------------------------------------------------
  void init()
  {
    //Sensores
    mpu.init();
    compass.init();

    pesoGyro = 0.8;
    pesoAcc  = 0.1;
    pesoMag  = 0.1;

    t0=micros();
    t1=t0;

    I.set(1,0,0);
    J.set(0,1,0);
    K.set(0,0,1);
  };
  //-------------------------------------------------------------------------- 
  String setPesos(float pGyro,float pAcc,float pMag){
    pesoGyro = pGyro;
    pesoAcc  = pAcc;
    pesoMag  = pMag;
  
    return String("PESOS [") +  pesoGyro + COMA + pesoAcc + COMA + pesoMag + "]" + SALTO_LINEA;
  };
  //--------------------------------------------------------------------------
  void filtroComplementario()
  {
    //Incremento de Tiempo
    t0 = t1;
    t1 = micros();
    float dt = (t1-t0)*0.000001;

    //Sensores
    mpu.update();
    compass.update();
     
    //PARA SLAVE
    vector acc  (       -mpu.accRaw[X] ,       -mpu.accRaw[Y] ,       -mpu.accRaw[Z]);
    vector mag  (   -compass.mGauss[Y] ,   -compass.mGauss[X] ,    compass.mGauss[Z]); 
    vector gyro ( -mpu.gyroRadianes[X] , -mpu.gyroRadianes[Y] , -mpu.gyroRadianes[Z]);
    
    //PARA MASTER
    //vector acc  (        mpu.accRaw[Y] ,        mpu.accRaw[X] ,        mpu.accRaw[Z]);
    //vector mag  (    compass.mGauss[X] ,    compass.mGauss[Y] ,   -compass.mGauss[Z]);
    //vector gyro ( mpu.gyroRadianes[Y] ,  mpu.gyroRadianes[X] ,   mpu.gyroRadianes[Z]);
    
    //Estimacion POSICION segun acc+mag
    //Vectores World en sistema body
    vector J0 =    acc.cross(mag).norm();  //ESTE
    vector I0 =    (J0.cross(acc)).norm();  //NORTE
    vector K0 =    acc.norm().mult(-1);    //CIELO
    
    //Estimacion GIRO 
    vector dTheta , dThetaGyro , dThetaAcc, dThetaMag;
    //Segun gyro
    dThetaGyro = gyro.mult(dt);
    dThetaGyro = dThetaGyro.mult(pesoGyro);
    //Segun acc
    dThetaAcc  = K.cross(K0.resta(K));
    dThetaAcc  = dThetaAcc.mult(pesoAcc);
    //Segun mag
    dThetaMag  = I.cross(I0.resta(I));
    dThetaMag  = dThetaMag.mult(pesoMag);

    //Filtro Complementario
    dTheta = dThetaAcc.suma(dThetaGyro).suma(dThetaMag);
    dTheta = dTheta.mult(1/(pesoAcc+pesoGyro+pesoMag));

    //DCM 
    I = (I.suma(dTheta.cross(I))).norm();
    J = (J.suma(dTheta.cross(J))).norm();
    K = (K.suma(dTheta.cross(K))).norm();

    //Correccion de Errores
    float Err   = (I.dot(K)) /2;
    vector I1p  = I.resta(K.mult(Err));
    vector Klp  = K.resta(I.mult(Err));
    vector I1pp = I1p.norm();
    vector K1pp = Klp.norm();
    vector J1pp = K1pp.cross(I1pp);  

    I = I1pp;
    J = J1pp;
    K = K1pp;
    
    toArray();
  };
  //--------------------------------------------------------------------------
  String rawToString() { 
    return mpu.rawToString() + compass.rawToString();  
  }; 
  //--------------------------------------------------------------------------
  String valuesToString() { 
    return mpu.valuesToString() + compass.valuesToString();   
  }; 
  //--------------------------------------------------------------------------
  String dcmToString(){

    String dcmS ="";
    for (int i=0;i<=8;i++){
      dcmS += dcm[i]; 
      dcmS += ",";
    }  
    return dcmS; 
  }; 
  //--------------------------------------------------------------------------
  String calibrarMPU(){
    return mpu.calibrar();
  };
  //--------------------------------------------------------------------------
  String calibrarHMC(int sel){
    return compass.calibrar(sel);
  };
  //--------------------------------------------------------------------------
  void enviar(int i)
  { 
    union u_tag {
      byte b[4];
      float floatVal;
    } u;
    u.floatVal = dcm[i];
    Wire.write(u.b,4);
  };
  //--------------------------------------------------------------------------
  void toArray()
  {
    dcm[0] = I.x;
    dcm[1] = I.y;
    dcm[2] = I.z;
    dcm[3] = J.x;
    dcm[4] = J.y;
    dcm[5] = J.z;
    dcm[6] = K.x;
    dcm[7] = K.y;
    dcm[8] = K.z;
  };
  //--------------------------------------------------------------------------
};

