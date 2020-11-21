'''
Created on 25 de mar. de 2016

@author: jaime
'''

from Vector import Vector


class DCM():
    
    def __init__(self, I=Vector(1, 0, 0), J=Vector(0, 1, 0), K=Vector(0, 0, 1)):
        
        #[Ix,Iy,Iz]
        #[Jx,Jy,Jz]
        #[Kx,Ky,Kz]
        
        self.I = I
        self.J = J
        self.K = K
    
    
    def traspose(self):
        iG = Vector(self.I[0],self.J[0],self.K[0])
        jG = Vector(self.I[1],self.J[1],self.K[1]) 
        kG = Vector(self.I[2],self.J[2],self.K[2]) 
        return DCM(iG,jG,kG)
    
    def update(self, data):
        
        if (data == 0):
            return
        else:
            try:
                data[8]
                Ix = float(data[0])
                Iy = float(data[1])
                Iz = float(data[2])
                Jx = float(data[3])
                Jy = float(data[4])
                Jz = float(data[5])
                Kx = float(data[6])
                Ky = float(data[7])
                Kz = float(data[8])
                
                self.I = Vector(Ix, Iy, Iz)
                self.J = Vector(Jx, Jy, Jz)
                self.K = Vector(Kx, Ky, Kz) 
                 
            except:
                pass
