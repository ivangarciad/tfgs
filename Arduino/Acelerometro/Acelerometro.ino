/*
The sensor outputs provided by the library are the raw 16-bit values
obtained by concatenating the 8-bit high and low accelerometer and
magnetometer data registers. They can be converted to units of g and
gauss using the conversion factors specified in the datasheet for your
particular device and full scale setting (gain).

Example: An LSM303D gives a magnetometer X axis reading of 1982 with
its default full scale setting of +/- 4 gauss. The M_GN specification
in the LSM303D datasheet (page 10) states a conversion factor of 0.160
mgauss/LSB (least significant bit) at this FS setting, so the raw
reading of -1982 corresponds to 1982 * 0.160 = 317.1 mgauss =
0.3171 gauss.

In the LSM303DLHC, LSM303DLM, and LSM303DLH, the acceleration data
registers actually contain a left-aligned 12-bit number, so the lowest
4 bits are always 0, and the values should be shifted right by 4 bits
(divided by 16) to be consistent with the conversion factors specified
in the datasheets.

Example: An LSM303DLH gives an accelerometer Z axis reading of -16144
with its default full scale setting of +/- 2 g. Dropping the lowest 4
bits gives a 12-bit raw value of -1009. The LA_So specification in the
LSM303DLH datasheet (page 11) states a conversion factor of 1 mg/digit
at this FS setting, so the value of -1009 corresponds to -1009 * 1 =
1009 mg = 1.009 g.
*/

#include <Wire.h>
#include <math.h>
#include <LSM303.h>


/*Los siguientes factores sirven para convertir los valores dados
por el acelerometro a g. Las unidades de estos factores 
son mg.  Este factor hay que multiplicarlo por la medida para obtener mg.
*/

#define FACTOR_CONVERSION_2G 0.000061 //Valor de la tabla/1000
#define FACTOR_CONVERSION_4G 0.000122
#define FACTOR_CONVERSION_6G 0.000183
#define FACTOR_CONVERSION_8G 0.000244
#define FACTOR_CONVERSION_16G 0.000732

#define RAD_TO_DEG 57.2958

#define OFFSET     0.06 //Este es el valor del acelerometro para una fuerza de 0 g (obtenida de la pagina 10 del datasheet) 

LSM303 compass;

unsigned long time_loop1, time_loop2, time;

int counter, signo_z;
int offseta_x = 0, offseta_y=0, offseta_z=0;
int sampleNum = 500;
double aceleracion_x, aceleracion_y, aceleracion_z;
double angulo_acelerometro_x, angulo_acelerometro_y;
double raiz_x, raiz_y;

int acx, acy, acz;


void setup()
{
  Serial.begin(115200);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  
  compass.writeReg(LSM303::CTRL2, 0x00);
  
  
   
/*La linea compass.writeReg(LSM303::CTRL2, 0x10) cambia el fondo de escala, 
que por defecto al llamar a enableDefault se configura a +-2 g. 
Los valores posibles son:
     
         FS    VALOR REGISTRO CTRL2 
       +-2 g: 0x00;
       +-4 g: 0x08;
       +-6 g: 0x10; 
       +-8 g: 0x18;
       +-16 g:0x20;
       
Los bits del registro son:

  X X FS2 FS1 FS0 X X X        
       
*/

aceleracion_x = 0;
aceleracion_y = 0;
aceleracion_z = 0;
angulo_acelerometro_x = 0; 
angulo_acelerometro_y = 0;

Calibracion();
}




void loop()
{
  time_loop1 = millis();
  counter++;
  Calculo_Angulos();
 
   if (counter == 1){ 
         //Imprimir_Angulos(angulo_acelerometro_x, angulo_acelerometro_y);
         Imprimir_Para_Graficas(angulo_acelerometro_y);
         counter = 0;
     }
   while(millis()-time_loop1 < 10) //Timing->hay que asegurarse de que cada iteracion dura 5 ms, comprobado previamente con time_loop1 y time_loop2
  {
    delay(1);
  } 
}

//FUNCIONES

void Calibracion(){
  
   //Calcular el offset
 
for(int n=0;n<sampleNum;n++){
  
  compass.read();
  offseta_x+=(int)compass.a.x;
  offseta_y+=(int)compass.a.y;
  offseta_z+=(int)compass.a.z;

}
offseta_x = offseta_x/sampleNum;
offseta_y = offseta_y/sampleNum;
offseta_z = offseta_z/sampleNum;


//Imprimir offset
Serial.println();
Serial.print(" Offset X: ");
Serial.print(offseta_x);
Serial.print(" /tOffset y: ");
Serial.print(offseta_y);
Serial.print(" /tOffset Z: ");
Serial.print(offseta_z);
delay(1000);  
  
}


void Calculo_Angulos()
{
  compass.read();
  
  aceleracion_x =  -((compass.a.x-offseta_x)*FACTOR_CONVERSION_2G);//El signo - es debido a que la IMU esta invertida 
  aceleracion_y =  -((compass.a.y-offseta_y)*FACTOR_CONVERSION_2G);
  aceleracion_z =  -((compass.a.z-offseta_z)*FACTOR_CONVERSION_2G);
  
  if (aceleracion_z > 0)
      signo_z = 1;
  else if (aceleracion_z < 0)
      signo_z = -1;
  
  //raiz_x = signo_z*sqrt(0.65*(aceleracion_x*aceleracion_x)+(aceleracion_z*aceleracion_z));
  
  raiz_x = sqrt(pow(aceleracion_x,2)+pow(aceleracion_z,2));
  raiz_y = sqrt((aceleracion_y*aceleracion_y)+(aceleracion_z*aceleracion_z));    
  
  //angulo_acelerometro_x = (atan2(aceleracion_y, aceleracion_z))*RAD_TO_DEG;  //Equations 25 and 26 of http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf
  //angulo_acelerometro_y = (atan2(-aceleracion_x,raiz_y))*RAD_TO_DEG;
  
  
  //angulo_acelerometro_x = (atan2(aceleracion_y,raiz_x))*RAD_TO_DEG;    //Equations 28 and 29
  //angulo_acelerometro_y = (atan2(-aceleracion_x, aceleracion_z))*RAD_TO_DEG;  
  
  
  angulo_acelerometro_x = (atan2(aceleracion_y,raiz_x))*RAD_TO_DEG;  //Equations 37 and 38
  angulo_acelerometro_y = (atan2(-aceleracion_x,raiz_y))*RAD_TO_DEG;
  
}



void Imprimir_Angulos(double x, double y)
  {
    Serial.print("Angulo X:");
   Serial.println((int) x);
   
   Serial.print("Angulo Y:");
   Serial.println((int) y);
   
   Serial.print("Acel Z:");
   Serial.println(aceleracion_z);
   
  }
  
    void Imprimir_Para_Graficas(double anguloy)
  {
  
   Serial.print((int) anguloy); 
   Serial.print("\t\n");
   
  }


/*
ACTUALMENTE LOS ANGULOS OSCILAN ENTRE -90 Y 90 EN AMBOS EJES



*/
