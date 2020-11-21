from visual import frame, cone, box, ellipsoid, color, curve, sphere, text, cylinder
from recursos.matematicas.DCM import DCM

class Ejes():
    
    def __init__(self, colorX=color.blue, colorY=color.blue, colorZ=color.blue, longitud=5):
        
        self.objeto = frame()
        self.ejeX = curve (frame=self.objeto, pos=[(0, 0, 0) , (longitud, 0, 0)] , radius=0.02, color=colorX)
        self.ejeY = curve (frame=self.objeto, pos=[(0, 0, 0) , (0, longitud, 0)] , radius=0.02, color=colorY)
        self.ejeZ = curve (frame=self.objeto, pos=[(0, 0, 0) , (0, 0, longitud)] , radius=0.02, color=colorZ)
        self.centro = sphere(frame=self.objeto, pos=(0, 0, 0) , radius=0.04, color=color.blue)
        
        # LETRAS X
        self.boxX = box(frame=self.objeto, pos=(3, 0, 0), length=0.2, height=0.2, width=0.2, color=color.blue)
        self.letraX1 = text(frame=self.objeto, text='X', align='left', height=0.1, depth=0.12, color=color.green, pos=(2.95, -0.05, 0))
        self.letraX2 = text(frame=self.objeto, text='X', align='left', height=0.1, depth=0.12, color=color.green, pos=(2.95, 0.12, -0.05), up=(0, 0, 1))
        # LETRAS Y
        self.boxY = box (frame=self.objeto, pos=(0, 3, 0), length=0.2, height=0.2, width=0.2, color=color.blue)
        self.letraY1 = text(frame=self.objeto, text='Y', align='left', height=0.1, depth=0.12, color=color.green, pos=(0.05, 2.95, 0), axis=(0, 1, 0))
        self.letraY2 = text(frame=self.objeto, text='Y', align='left', height=0.1, depth=0.12, color=color.green, pos=(0.00, 2.95, -0.05), axis=(0, 1, 0), up=(0, 0, 1))
        # LETRAS Z
        self.boxZ = box (frame=self.objeto, pos=(0, 0, 3), length=0.2, height=0.2, width=0.2, color=color.blue)
        self.letraZ1 = text(frame=self.objeto, text='Z', align='left', height=0.1, depth=0.12, color=color.green, pos=(-0.05, 0.0, 3.05), up=(0, 0, -1))
        self.letraZ2 = text(frame=self.objeto, text='Z', align='left', height=0.1, depth=0.12, color=color.green, pos=(0.00, -0.05, 2.95), axis=(0, 1, 0), up=(0, 0, 1))
        
    def visible(self, valor):
        if(valor == True):
            self.objeto.visible = True
        if(valor == False):
            self.objeto.visible = False  
    
    def color(self, colorX, colorY, colorZ):
            self.ejeX.color = colorX
            self.ejeY.color = colorY
            self.ejeZ.color = colorZ  

    def boxes(self, visible):
        self.boxX.visible = visible
        self.letraX1.visible = visible
        self.letraX2.visible = visible
        # LETRAS Y
        self.boxY.visible = visible
        self.letraY1.visible = visible
        self.letraY2.visible = visible
        # LETRAS Z
        self.boxZ.visible = visible
        self.letraZ1.visible = visible
        self.letraZ2.visible = visible
               
        
    def longitud(self, longitud):
        self.ejeX.pos = [(0, 0, 0) , (longitud, 0, 0)]
        self.ejeZ.pos = [(0, 0, 0) , (0, longitud, 0)]
        self.ejeY.pos = [(0, 0, 0) , (0, 0, longitud)]
    
    def axis(self, v):
        self.objeto.axis = v
        
    def up(self, v):
        self.objeto.up = v
    
    def pos(self, v):
        self.objeto.pos = v
  
