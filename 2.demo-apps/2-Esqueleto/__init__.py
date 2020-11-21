import sys, time
from visual import arrow, color, rate, display, frame, sphere, cylinder


from recursos.Configuracion      import *
from recursos.Arduino            import Arduino
from recursos.figuras.Ventana    import Ventana
from recursos.figuras.Esqueleto  import Esqueleto
from recursos.matematicas.Vector import Vector
from recursos.matematicas.DCM    import DCM

# VENTANA ----------------------------------------------------------
ventana = Ventana("ESQUELETO", centro=(0, 0, 2)) 
ventana.display.scale = (0.1, 0.1, 0.1)


# ARDUINO ----------------------------------------------------------
arduino = Arduino(PUERTO, BAUDRATE)
arduino.conectar()

# Figura -----------------------------------------
esqueleto = Esqueleto()

dcm = DCM()


while(1):
    
    rate(FPS)
        
    # BRAZOS    
    esqueleto.brazoDch.setData((arduino.getData(2)))
    esqueleto.anteBDch.setData((arduino.getData(6))) 
    esqueleto.brazoIzd.setData((arduino.getData(5)))
    esqueleto.anteBIzd.setData((arduino.getData(1))) 
        
    # TRONCO Y HOMBROS
    dcm.update(arduino.getData('a'))
    dcm = dcm.traspose()
    esqueleto.tronco.updateOrientacion2(dcm.I, dcm.K)
    esqueleto.cuello.updateOrientacion2(dcm.I, dcm.K)
    esqueleto.hombroDch.updateOrientacion2(dcm.J, dcm.K)
    esqueleto.hombroIzq.updateOrientacion2(-dcm.J , dcm.K)
    
    esqueleto.update()
