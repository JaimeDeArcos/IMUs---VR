//VERSION DEL SOFTWARE
#define VERSION 1
//***********************************************
// SOFTWARE I2C 
#define NO_INTERRUPT 1
#define I2C_TIMEOUT 500
#define I2C_FASTMODE 1

#define SDA_PORT PORTB
#define SDA_PIN 1        //DIGITAL PIN 9
#define SCL_PORT PORTB
#define SCL_PIN 0        //DIGITAL PIN 8
#define FAC 1
#define I2C_CPUFREQ (F_CPU/FAC) 
//***********************************************
// EJES (Axes)
#define X  0  
#define Y  1  
#define Z  2  
//***********************************************
#define PIN_LED 13
boolean ledStatus;

void blink(){
   ledStatus = !ledStatus;
   digitalWrite(PIN_LED,ledStatus);
} 
//************************************************
String TAB = "\t";
String SALTO_LINEA =  "\n";
String COMA = ","; 


