#include <Arduino.h>

float slaveDCM[9][9];

float recibirFloat(int x,byte dir){
    byte data[4];
    //Peticion
     Wire.beginTransmission(dir); 
     Wire.write(byte(x));          
     Wire.endTransmission(); 
     //lectura
     Wire.requestFrom(dir, 4);    
     
     int i=0;
     while(Wire.available())    
     { 
       data[i] = Wire.read();
       i++;       
     }
     
     union u_tag {
       byte b[4];
       float floatVal;
     } u;
    
     u.b[0] = data[0];
     u.b[1] = data[1];
     u.b[2] = data[2];
     u.b[3] = data[3];
     
     return u.floatVal; 
};
 
void Slave_update(byte dir){ 
    for (int i=0;i<9;i++)  slaveDCM[dir][i] = recibirFloat(i,dir);
};

String Slave_toString(byte dir){ 
     String dcmS ="";
    for (int i=0;i<=8;i++){
      dcmS += slaveDCM[dir][i]; 
      dcmS += ",";
    }  
    return dcmS; 
};

