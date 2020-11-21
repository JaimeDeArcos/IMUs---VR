//***********************************************
//DIRECCION I2C PARA EL
byte getI2CADDRESS()            { return EEPROM.read(EEPROM_I2C_ADDRESS);  }
void setI2CADDRESS(byte address){ EEPROM.write(EEPROM_I2C_ADDRESS , address);}
//***********************************************

void changeAdress(){
  
  Serial.print("Dir Actual: 0x");
  Serial.println(getI2CADDRESS());
  
  Serial.println("Nueva DIR?: [0-255]");
  while (!Serial.available()){ delay(100); }
  int dato = Serial.parseInt();
  
  if ((dato > 255) || (dato < 1)){
      Serial.println("ERROR: rango [0-255]");  
      Serial.print("La Dir I2C sigue siendo: 0x");
      Serial.print(getI2CADDRESS());
  }else{
      Serial.print("DIR cambiada");
      byte nuevaDir = (byte) dato;
      Serial.println(nuevaDir);
      setI2CADDRESS(nuevaDir);
  } 
  Serial.print("Dir Actual: 0x");
  Serial.println(getI2CADDRESS());
  Serial.flush();
}

