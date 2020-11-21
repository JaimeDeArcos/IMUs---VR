'''
Created on 25 de mar. de 2016

@author: jaime
'''
from visual import frame, cone, box, ellipsoid, color
from recursos.matematicas.DCM    import DCM
from recursos.matematicas.Vector import Vector


class Avion():
    
    def __init__(self):
       
        self.obj = frame(make_trail=True)
        self.body = cone     (frame=self.obj, pos=(0.25, 0, 0), axis=(-0.6, 0, 0), radius=0.04)
        self.body2 = cone     (frame=self.obj, pos=(0.25, 0, 0), axis=(0.1, 0, 0), radius=0.04)
        self.cabin = ellipsoid(frame=self.obj, pos=(0.17, 0.0125, 0), axis=(0.5, 0, 0), size=(0.13, 0.08, 0.05))
        self.wing = box      (frame=self.obj, pos=(0.175, 0, 0), size=(0.1, 0.01, 0.75))
        self.tail = box      (frame=self.obj, pos=(-0.2, 0, 0), size=(0.1, 0.01, 0.3))
        self.aleron = box      (frame=self.obj, pos=(-0.2, 0.05, 0), size=(0.1, 0.1, 0.02))
           
        # Pintura
        for obj in self.obj.objects:
            obj.color = color.red

        self.body.color = color.white
        self.cabin.color = (0.5, 0.5, 0.5)
        self.cabin.opacity = 0.8
        
        # Orientacion
        self.obj.axis = (1, 0, 0)
        self.obj.up = (0, 0, 1)
                
    def size(self, multiplicador):
        self.body.pos *= multiplicador
        self.body.axis *= multiplicador
        self.body.radius *= multiplicador
        self.body2.pos *= multiplicador
        self.body2.axis *= multiplicador
        self.body2.radius *= multiplicador
        self.wing.pos *= multiplicador
        self.wing.size *= multiplicador
        self.tail.pos *= multiplicador
        self.tail.size *= multiplicador
        self.aleron.pos *= multiplicador
        self.aleron.size *= multiplicador
        self.cabin.pos *= multiplicador
        self.cabin.size *= multiplicador
        
    def visibles(self, v):
        self.obj.visible = bool(v) 
    
    def axis(self, v):
        self.obj.axis = v
        
    def up(self, v):
        self.obj.up = v
        
    def pos(self, v):
        self.obj.pos = v
        