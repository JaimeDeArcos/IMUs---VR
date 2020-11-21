
import sys
import glob
import serial
import winsound

class Arduino(): 
    
    #PUERTO
    #BAUDRATE
    #IMU
                          
    def __init__(self, puerto, baudrate):
        
        self.PUERTO = puerto
        self.BAUDRATE = baudrate
        #self.TIMEOUT = timeout
         
    def conectar(self):
        try:
            self.conexion = serial.Serial(self.PUERTO, self.BAUDRATE,timeout=0.10)
            #self.conexion = serial.Serial(self.PUERTO, self.BAUDRATE, timeout=self.TIMEOUT)
        except:
            print("ERROR: NO SE PUEDE ABRIR EL PUERTO: " + self.PUERTO)
            Freq = 2500 # Set Frequency To 2500 Hertz
            Dur = 100 # Set Duration To 1000 ms == 1 second
            winsound.Beep(Freq,Dur)
            sys.exit(0)
            
    def close(self):
        # close serial
        self.conexion.flush()
        self.conexion.close()    
    
    def listaPuertos(self):
        """Devuelve los puertos Serie disponibles
        """
        if sys.platform.startswith('win'):
            result = []
            for i in range(256):
                try:
                    s = serial.Serial(i)
                    s.close()
                    result.append('COM' + str(i + 1))
                except serial.SerialException:
                    pass
            return result

        elif sys.platform.startswith('linux'):
            return glob.glob('/dev/tty*')

        elif sys.platform.startswith('darwin'):
            return glob.glob('/dev/tty.*')
    
    def getData(self,code):
        
        self.conexion.write(str(code))
        linea = self.conexion.readline()
        data = linea.split(',')
        
        return data
        
