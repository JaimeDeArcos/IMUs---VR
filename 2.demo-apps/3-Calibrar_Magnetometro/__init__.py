from visual import *  # must import visual or vis first
from visual.graph import *  # import graphing features

from recursos.Configuracion      import *
from recursos.Arduino            import Arduino  

arduino = Arduino(PUERTO, BAUDRATE)
arduino.conectar()


# GRAFICA:
gd = gdisplay(x=100, y=0, width=500, height=500,
      title='Medidas Magnetometro', xtitle='X', ytitle='Y',
      foreground=color.white, background=color.black,
      xmax=1000, xmin=-1000, ymax=1000 , ymin=-1000,)


f1 = gdots(color=color.cyan)


while 1:
    
    rate(FPS)
    
    data = arduino.getData('g')

    mX = float(data[0]) 
    mY = float(data[1])
    mZ = float(data[2])
    
    mCX = float(data[3]) 
    mCY = float(data[4])
    mCZ = float(data[5])
    
    # M-XY Azul
    # f1.plot(pos=(mX, mY), color=color.blue)
    
    # XZ rojo
    # f1.plot(pos=(mX, mZ), color=color.red)
     
    
    # YZ amarillo 
    f1.plot(pos=(mY, mZ), color=color.yellow)
    f1.plot(pos=(mCY, mCZ), color=color.red)
