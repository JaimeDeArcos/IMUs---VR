from visual import arrow, color, rate, display
 
from recursos.Configuracion      import *
from recursos.Arduino            import Arduino
from recursos.figuras.Ventana    import Ventana  
from recursos.figuras.Avion      import Avion
from recursos.matematicas.DCM    import  DCM

# VENTANA ----------------------------------------------------------
ventana = Ventana("Avion.py") 

# ARDUINO ----------------------------------------------------------
arduino = Arduino(PUERTO, BAUDRATE)
arduino.conectar()

# AVION ------------------------------------------------------------
avion = Avion()
avion.size(4)

dcm = DCM()
    
while 1:
    
    rate(FPS)
    
    data = arduino.getData(COD_DCM) 
    
    dcm.update(data)
    
    dcm = dcm.traspose();
   
    avion.axis(dcm.I)
    avion.up(dcm.K)
        
