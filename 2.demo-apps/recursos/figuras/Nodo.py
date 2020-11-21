from recursos.matematicas.Vector import Vector
from visual import color, sphere, cylinder
from recursos.figuras.Ejes import Ejes
from recursos.matematicas.DCM import DCM


class Nodo():
            
    def __init__(self, largo , padre=None, dcm=DCM()): 
        
        self.dcm = dcm
        
        self.color = color.white
        self.largo = largo
        self.posicion = (0, 0, 0)
        self.axis = Vector(1, 0, 0) * self.largo
        self.up = Vector(0, 0, 1)
        self.padre = padre
        
        self.cilindro = cylinder(pos=self.posicion , axis=self.axis , radius=0.1 , color=self.color)
        self.junta = sphere  (pos=self.posicion , radius=0.2 , color=color.red)
        self.ejes = Ejes(color.green, color.yellow , color.red, 0.5)
        self.ejes.boxes(False)
                
    def updatePosicion(self): 
        # Posicion
        self.posicion = self.padre.cilindro.axis + self.padre.posicion
        self.junta.pos = self.posicion
        self.cilindro.pos = self.posicion 
        self.ejes.pos(self.posicion) 
        
    def updateOrientacion(self):
        # Orientacion  
        self.cilindro.axis = self.dcm.I
        self.ejes.axis(self.dcm.I)
        self.cilindro.up = self.dcm.K
        self.ejes.axis(self.dcm.K)
        self.cilindro.length = self.largo
     
    def updateOrientacion2(self,vI ,vK):
        # Orientacion  
        self.cilindro.axis = vI
        self.ejes.axis(vI)
        self.cilindro.up = vK
        self.ejes.axis(vK)
        self.cilindro.length = self.largo
          
    def setPadre(self, nodoPadre):
        self.padre = nodoPadre
    
    def setData(self, data):
        
        self.dcm.update(data)
        self.dcm = self.dcm.traspose()
        self.updateOrientacion()
