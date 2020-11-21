/*********************************************************
 *                     I2C Functions
 *********************************************************/ 
 
 byte I2C_getRegister(byte address, byte registro) {
    i2c_start(address | I2C_WRITE );
    i2c_write(registro);
    
    i2c_rep_start(address | I2C_READ);
    byte oneByte = i2c_read (true); //read one byte
    i2c_stop();

    return oneByte;  // return byte value
};

bool I2C_writeRegister(byte address,byte registro, byte valor) {
    if(!i2c_start(address | I2C_WRITE))   return false;
    if(!i2c_write(registro))              return false;
    if (!i2c_write(valor))                return false;
    i2c_stop();
    return true;
};
