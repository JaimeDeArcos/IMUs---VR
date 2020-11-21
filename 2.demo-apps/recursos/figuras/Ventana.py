
from visual import display, color
from recursos.figuras.Ejes import Ejes


# http://vpython.org/contents/docs/display.html

class Ventana():
    
    def __init__(self, titulo, centro=(0, 0, 0), ejesVisibles=True):
        self.display = display(title=titulo, x=0, y=0, width=1250, height=1040, center=centro, background=color.black)
        self.display.forward = (-2, -2, -2)
        self.display.up = (0, 0, 2)
        self.ejes = Ejes()
        self.ejes.visible(ejesVisibles)
     
    def escala(self, val):    
        self.display.scale = (val, val, val)
        
    
