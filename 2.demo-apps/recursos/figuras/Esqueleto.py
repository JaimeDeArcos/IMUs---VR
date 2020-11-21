from visual import frame
from recursos.figuras.Nodo import Nodo 
from recursos.matematicas.DCM import DCM

class Esqueleto():
    
    def __init__(self, ejesVisibles=False):
        
        # CUERPO
        self.nodoRaiz = Nodo(0) 
          
        self.cuerpo = frame(make_trail=True)
        self.tronco = Nodo(2.5, self.nodoRaiz, dcm=DCM(I=(0, 0, 1), K=(0, 1, 0))) 
        self.hombroIzq = Nodo(0.7, self.tronco, dcm=DCM(I=(1, 0, 0), K=(0, 1, 0))) 
        self.hombroDch = Nodo(0.7, self.tronco, dcm=DCM(I=(-1, 0, 1), K=(0, 1, 0))) 
        
        
        self.cuello = Nodo(0.7, self.tronco, dcm=DCM(I=(0, 0, 1), K=(0, 1, 0))) 
        self.cabeza = Nodo(0  , self.cuello, dcm=DCM(I=(0, 0, 1), K=(0, 1, 0)))
        # EXTREMIDADES
        self.brazoDch = Nodo(1.5 , self.hombroDch, dcm=DCM(I=(0, 0, -1), K=(0, 1, 0)))
        self.anteBDch = Nodo(1.5, self.brazoDch, dcm=DCM(I=(0, 0, -1), K=(0, 1, 0)))
        self.brazoIzd = Nodo(1.5, self.hombroIzq, dcm=DCM(I=(0, 0, -1), K=(0, 1, 0)))
        self.anteBIzd = Nodo(1.5, self.brazoIzd, dcm=DCM(I=(0, 0, -1), K=(0, 1, 0)))
        self.manoIzd = Nodo(0, self.anteBIzd, dcm=DCM(I=(0, 0, -1), K=(0, 1, 0)))
        self.manoDch = Nodo(0, self.anteBDch, dcm=DCM(I=(0, 0, -1), K=(0, 1, 0)))
        
        
         
        self.cabeza.junta.radius = 0.4
        
        self.tronco.updateOrientacion()
        self.cuello.updateOrientacion()
        self.cabeza.updateOrientacion()
        self.hombroDch.updateOrientacion()
        self.hombroIzq.updateOrientacion()
        self.brazoDch.updateOrientacion()
        self.anteBDch.updateOrientacion()
        
        self.brazoIzd.updateOrientacion()
        self.anteBIzd.updateOrientacion()
        self.manoIzd.updateOrientacion()
        self.manoDch.updateOrientacion()
        
        self.ejes(ejesVisibles)
        
        self.update()
        
    def update(self):
            
        self.tronco.updatePosicion()
        self.hombroIzq.updatePosicion()
        self.hombroDch.updatePosicion()
        self.cuello.updatePosicion()
        
        
        
        
        self.cabeza.updatePosicion()
        self.brazoDch.updatePosicion()
        self.anteBDch.updatePosicion()
        self.brazoIzd.updatePosicion()
        self.anteBIzd.updatePosicion()
        self.manoIzd.updatePosicion()
        self.manoDch.updatePosicion()
    
    def printAxis(self):
        print ("Tronco = " + str(self.tronco.cilindro.axis))
        print ("Cuello = " + str(self.cuello.cilindro.axis))
        print ("HombroIzq = " + str(self.hombroIzq.cilindro.axis))
        print ("HombroDch = " + str(self.hombroDch.cilindro.axis))
    
    def ejes(self, visible):
        self.nodoRaiz.ejes.visible(visible)
        self.tronco.ejes.visible(visible)
        self.cuello.ejes.visible(visible)
        self.cabeza.ejes.visible(visible)
        self.hombroIzq.ejes.visible(visible)
        self.hombroDch.ejes.visible(visible)
        self.brazoDch.ejes.visible(visible)
        self.anteBDch.ejes.visible(visible)
        self.brazoIzd.ejes.visible(visible)
        self.anteBIzd.ejes.visible(visible)
        self.manoIzd.ejes.visible(visible) 
        self.manoDch.ejes.visible(visible)

    
