/* ****************************************************************************
 * --------------------------------DCM----------------------------------------*
 *                                                                            *
 * Autor: Jaime de Arcos Sánchez                                              *
 * Fecha: 1 Julio 2014                                                        *
 *                                                                            *
 * Librería para el calculo de la DCM a partir de un IMU formado por:         *
 *                                                                            *
 * FUNCIONES:                                                                 *
 *  - init()          - Configuracion inicial                                  *
 *  - update()        - actualiza la DCM con valores de una nueva medida       *
 *  - RAWtoString()   - Imprime la DCM actualizada (Para avion.py)             *
 *  - DCMtoString()   
 *  - calibrarMPU()
 *  - calibrarHMC()
 *  
 *  - printI()        - Imprime la estimacion de la posicion acc+Gyro(avion.py)*
 *  - printValores()  - Imprime los valores en bruto de los sensores para      *
 *                      el cálculo en python (DCM v0.6.py)                     *
 *                                                                            *
 ******************************************************************************/

//PESOS DE CADA SENSOR
#define Pacc  0.1
#define Pmag  0.1
#define Pgyro 0.8

//----------------------------------------------------------------------------
class IMU
{
public:

  MPU6050  mpu;
  HMC5883L compass;

  vector I0,J0,K0;
  vector I1,J1,K1;
  vector I ,J ,K;
  
  float dcm[9]; 

  vector dTheta , dThetaGyro , dThetaAcc, dThetaMag;

  unsigned long t0,t1;
  //--------------------------------------------------------------------------
  void init()
  {
    //Sensores
    mpu.init();
    compass.init();

    t0=micros();
    t1=t0;

    I0.set(1,0,0);
    J0.set(0,1,0);
    K0.set(0,0,1);

    I1.set(1,0,0);
    J1.set(0,1,0);
    K1.set(0,0,1);
    
    I.set(1,0,0);
    J.set(0,1,0);
    K.set(0,0,1);
  };
  //--------------------------------------------------------------------------
  void update()
  {
    //Incremento de Tiempo
    t0 = t1;
    t1 = micros();
    float dt = (t1-t0)*0.000001;

    //Sensores
    mpu.update();
    compass.update();
    
       
    //PARA SLAVE
    //vector acc  (       -mpu.accRaw[X] ,       -mpu.accRaw[Y] ,       -mpu.accRaw[Z]);
    //vector mag  (   -compass.mGauss[Y] ,   -compass.mGauss[X] ,    compass.mGauss[Z]);
    //vector gyro (-mpu.gyroVelRadian[X] ,-mpu.gyroVelRadian[Y] ,-mpu.gyroVelRadian[Z]);
    
    //PARA MASTER
    vector acc  (        mpu.accRaw[Y] ,        mpu.accRaw[X] ,        mpu.accRaw[Z]);
    vector mag  (    compass.mGauss[X] ,    compass.mGauss[Y] ,   -compass.mGauss[Z]);
    vector gyro ( mpu.gyroRadianes[Y] ,  mpu.gyroRadianes[X] , mpu.gyroRadianes[Z]);
    
    //Estimacion POSICION segun acc+mag
    //Vectores World en sistema body
    J =    acc.cross(mag).norm();  //ESTE
    I =    (J.cross(acc)).norm();  //NORTE
    K =    acc.norm().mult(-1);    //CIELO
    
    //Estimacion GIRO 
    //Segun gyro
    dThetaGyro = gyro.mult(dt);
    dThetaGyro = dThetaGyro.mult(Pgyro);
    //Segun acc
    dThetaAcc  = K0.cross(K.resta(K0));
    dThetaAcc  = dThetaAcc.mult(Pacc);
    //Segun mag
    dThetaMag  = I0.cross(I.resta(I0));
    dThetaMag  = dThetaMag.mult(Pmag);

    //Filtro Complementario
    dTheta = dThetaAcc.suma(dThetaGyro).suma(dThetaMag);
    dTheta = dTheta.mult(1/(Pacc+Pgyro+Pmag));

    //DCM
    I1 = (I0.suma(dTheta.cross(I0))).norm();
    J1 = (J0.suma(dTheta.cross(J0))).norm();
    K1 = (K0.suma(dTheta.cross(K0))).norm();

    //Correccion de Errores
    float Err   = (I1.dot(K1)) /2;
    vector I1p  = I1.resta(K1.mult(Err));
    vector Klp  = K1.resta(I1.mult(Err));
    vector I1pp = I1p.norm();
    vector K1pp = Klp.norm();
    vector J1pp = K1pp.cross(I1pp);  

    I1 = I1pp;
    J1 = J1pp;
    K1 = K1pp;

    I0 = I1;
    J0 = J1;
    K0 = K1;
    
    toArray(); 
  };
  //--------------------------------------------------------------------------
  String RAWtoString() {

    String raw =""; 
    raw+= mpu.gyroRadianes[X]; raw+= ",";
    raw+= mpu.gyroRadianes[Y]; raw+= ",";
    raw+= mpu.gyroRadianes[Z]; raw+= ",";
    
    raw+= mpu.accRaw[X]; raw+= ",";
    raw+= mpu.accRaw[Y]; raw+= ",";
    raw+= mpu.accRaw[Z]; raw+= ",";

    raw+= compass.mGauss[X]; raw+= ",";
    raw+= compass.mGauss[Y]; raw+= ",";
    raw+= compass.mGauss[Z]; raw+= ",";

    return raw;
  };
  //-------------------------------------------------------------------------- 
  String DCMtoString(){

    String dcmS ="";
    for (int i=0;i<=8;i++){
      dcmS += dcm[i]; 
      dcmS += ",";
    }  
    return dcmS; 
  };
  //--------------------------------------------------------------------------  
  void calibrarMPU(){
    //mpu.calibrar();
  };
  //--------------------------------------------------------------------------  
  void calibrarHMC(int sel){
    compass.calibrar(sel);
  };
  //--------------------------------------------------------------------------
  void toArray()
  {
    dcm[0] = I1.x;
    dcm[1] = I1.y;
    dcm[2] = I1.z;
    dcm[3] = J1.x;
    dcm[4] = J1.y;
    dcm[5] = J1.z;
    dcm[6] = K1.x;
    dcm[7] = K1.y;
    dcm[8] = K1.z;
  };

};

