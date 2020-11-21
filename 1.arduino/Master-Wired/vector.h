/* **********************************************************************
 * -----------------------VECTOR----------------------------------------*
 * Autor: Jaime de Arcos Sánchez                                        *
 * Fecha: 1 Julio 2014                                                  *
 *                                                                      *
 * Libreria para la manipulacion de vectores                            *
 *                                                                      *
 * FUNCIONES:                                                           *
 *  - norm()      - Devuelve el vector normalizado                      *
 *  - modulo()    - Devuelve el modulo del vector                       *
 *  - mult()      - Multiplica el vector por un escalar                 *
 *  - dot()       - Producto Escalar                                    *
 *  - cross()     - Producto Vectorial                                  *
 *  - suma()      - Suma de vectores                                    *
 *  - resta()     - Resta de vectores                                   *
 *  - set()       - Define el vector                                    *
 *  - print()     - Imprime el vector en el formato (x,y,z)             *
 ************************************************************************/
 
 class vector
{
  public:
  
     float x;
     float y;
     float z;
     
     //--------------------------------------------------------------------
     vector()
     {
        x = 0;
        y = 0;
        z = 0; 
     }
     //--------------------------------------------------------------------
     vector(float a, float b, float c)
     {
        x = a;
        y = b;
        z = c;
     };
     //--------------------------------------------------------------------
     vector norm()
     {
        float mod = modulo();
        vector vOut(x/mod,y/mod,z/mod);
        return vOut;
     };
     //--------------------------------------------------------------------
     float modulo()
     {
        return sqrt(sq(x) + sq(y) + sq(z)); 
     };
     //--------------------------------------------------------------------
     vector mult(float escalar)
     {
        vector vOut(x*escalar,y*escalar,z*escalar);
        return vOut; 
     };
     //--------------------------------------------------------------------
     float dot(vector v)
     {
         return x*v.x + y*v.y + z*v.z;
     }
     //--------------------------------------------------------------------
     vector cross(vector v2)
     {
        vector vOut(0,0,0);
        vOut.x = y*v2.z - z*v2.y;
        vOut.y = z*v2.x - x*v2.z;
        vOut.z = x*v2.y - y*v2.x;
        return vOut;
     };
     //--------------------------------------------------------------------
     vector suma(vector v2)
     {
        vector vOut(0,0,0);
        vOut.x = x + v2.x;
        vOut.y = y + v2.y;
        vOut.z = z + v2.z;
        return vOut;
     };
     //--------------------------------------------------------------------
     vector resta(vector v2)
     {
        vector vOut(0,0,0);
        vOut.x = x - v2.x;
        vOut.y = y - v2.y;
        vOut.z = z - v2.z;
        return vOut;
     };
     //--------------------------------------------------------------------
     void set(float a,float b,float c)
     {
       x = a;
       y = b;
       z = c;
     };
     //--------------------------------------------------------------------
     void print()
     {
        Serial.print("(");
        Serial.print(x);
        Serial.print(",");
        Serial.print(y);
        Serial.print(",");
        Serial.print(z);
        Serial.print(")"); 
     };
     //--------------------------------------------------------------------
};
