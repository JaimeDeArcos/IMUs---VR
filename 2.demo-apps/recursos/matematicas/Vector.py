'''
Created on 25 de mar. de 2016

@author: jaime
'''
from math import sqrt , fsum


class Vector(tuple):
    
    def __new__(cls, *V):
        '''Para crear un nuevo vector. 
        - Por ejemplo: V = Vector() 
                       V = Vector(a,b)
                       V = Vector(v1, v2, ..., vn)'''
        if not V:
            V = (0, 0)
        elif len(V) == 1:
            raise ValueError('Un vector debe tener al menos 2 coordenadas".')
        return tuple.__new__(cls, V)
    #SUMA--------------------------------------------------------------
    def __add__(self, V):
        '''Sobrecarga del operador suma. Se pueden
        sumar vectores con la operacion  V + W.'''
        if len(self) != len(V):
            raise IndexError('Vectors must have same dimmensions.')
        else:
            added = tuple(a + b for a, b in zip(self, V))
            return Vector(*added)

    __radd__ = __add__  # radd y add : para a+b o b+a
    # RESTA------------------------------------------------------------
    def __sub__(self, V):
        '''Sobrecarga del operador resta(-). Se pueden
        restar vectores con la operacion  V - W.'''
        if len(self) != len(V):
            raise IndexError('Vectors must have same dimmensions.')
        else:
            subtracted = tuple(a - b for a, b in zip(self, V))
            return Vector(*subtracted)

    def __rsub__(self, V):
        '''Sobrecarga del operador resta (-). Se pueden
        restar vectores con la operacion  w - V.'''
        if len(self) != len(V):
            raise IndexError('Los vectores deben tener la misma dimension.')
        else:
            subtracted = tuple(b - a for a, b in zip(self, V))
            return Vector(*subtracted)     
           
    # MULTIPLICACION--------------------------------------------------
    def __mul__(self, V):
        '''Sobrecarga del operador multiplicacion (*). Multiplica
        2 vectores coordenada a coordenada'''
        if type(V) == type(self):
            if len(self) != len(V):
                raise IndexError('Los vectores deben tener la misma dimension.')
            else:
                
                # multiplied = tuple(a * b for a, b in zip(self, V))
                resultado = 0
                for i in range(len(self)):
                    resultado += (self[i] * V[i])
                return resultado         
        # Vector * Escalar
        elif isinstance(V, type(1)) or isinstance(V, type(1.0)):
            multiplied = tuple(a * V for a in self)
        return Vector(*multiplied)

    __rmul__ = __mul__
    
    # DIVISION --------------------------------------------------------
    def __truediv__(self, V):
        if type(V) == type(self):
            if len(self) != len(V):
                raise IndexError('Los vectores deben tener la misma dimension.')
            if 0 in V:
                raise ZeroDivisionError('Division por 0.')
            divided = tuple(a / b for a, b in zip(self, V))
        elif isinstance(V, int) or isinstance(V, float):
            divided = tuple(a / V for a in self)
        return Vector(*divided)

    __rtruediv__ = __truediv__
    
    # NEGACION --------------------------------------------------------
    def __neg__(self):
        '''El operador negacion sobrecargado para vectores.
         Cambia el signo de cada coordenada.'''
        opposite = tuple(-1 * a for a in self)
        return Vector(*opposite)
    
    # MODULO ----------------------------------------------------------
    def modulo(self):
        '''Devuelve el modulo, magnitud del vector'''
        return sqrt(fsum(comp ** 2 for comp in self))
        
    # NORMALIZADO ----------------------------------------------------- 
    def norm(self):
        '''Devuelve el vector normalizado (modulo = 1)'''
        modulo = self.modulo()
        try:
            normalizado = tuple(comp / modulo for comp in self)
            return Vector(*normalizado)    
        except ZeroDivisionError:
            return Vector(0.0, 0.0, 0.0)
    # PRODUCTO VECTORIAL ----------------------------------------------
    def cross(self, V):
        '''Producto vectorial (cross) entre 2 vectores ''' 
        if len(self) > 3 or len(V) > 3 or len(self) != len(V):
            raise IndexError('Los vectores deben ser 3d.')
        else:
            e1 = Vector(1, 0, 0)
            e2 = Vector(0, 1, 0)
            e3 = Vector(0, 0, 1)
            det1 = self[1] * V[2] - self[2] * V[1]
            det2 = self[0] * V[2] - self[2] * V[0]
            det3 = self[0] * V[1] - self[1] * V[0]
            prodv = det1 * e1 - det2 * e2 + det3 * e3
            return prodv
