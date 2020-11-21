import sys
from visual import arrow, color, rate, display


from recursos.Configuracion      import *
from recursos.Arduino            import Arduino
from recursos.figuras.Ventana    import Ventana
from recursos.matematicas.Vector import Vector

ventana = Ventana("Sensores en bruto") 

arduino = Arduino(PUERTO, BAUDRATE)
arduino.conectar()

accArrow = arrow(pos=(0, 0, 0), axis=(0, 0, -1), color=color.red)
gyroArrow = arrow(pos=(0, 0, 0), axis=(1, 0, 0) , color=color.green)
magArrow = arrow(pos=(0, 0, 0), axis=(0, 1, 0) , color=color.blue)


while 1:
    
    rate(FPS)
    
    data = arduino.getData(COD_VALORES)
    
    gX = float(data[0])  ; gY = float(data[1]) ; gZ = float(data[2])
    aX = float(data[3])  ; aY = float(data[4]) ; aZ = float(data[5])
    mX = float(data[6])  ; mY = float(data[7]) ; mZ = float(data[8])
    
    # PARA MASTER
    # acc  = Vector(-aX, -aY, -aZ).norm()
    # mag  = Vector(-mY, -mX,  mZ).norm()
    # gyro = Vector(-gX, -gY, -gZ)
    
    acc = Vector(aX, aY, aZ).norm()
    mag = Vector(mX, mY, mZ).norm()
    gyro = Vector(gX , gY, gZ) * 0.01  # Para que no sea muy grande el valor /100
    
     
    accArrow.axis = -acc  # Fuerza G
    gyroArrow.axis = gyro  # Grados/seg
    magArrow.axis = -mag  # mili Gauss

 
