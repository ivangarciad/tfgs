/*
The sensor outputs provided by the library are the raw 16-bit values
obtained by concatenating the 8-bit high and low gyro data registers.
They can be converted to units of dps (degrees per second) using the
conversion factors specified in the datasheet for your particular
device and full scale setting (gain).

Example: An L3GD20H gives a gyro X axis reading of 345 with its
default full scale setting of +/- 245 dps. The So specification
in the L3GD20H datasheet (page 10) states a conversion factor of 8.75
mdps/LSB (least significant bit) at this FS setting, so the raw
reading of 345 corresponds to 345 * 8.75 = 3020 mdps = 3.02 dps.
*/

#include <Wire.h>
#include <L3G.h>


/*Los siguientes factores sirven para convertir los valores dados
por el giroscopio a grados por segundo. Las unidades de estos factores 
son mdps(miligradosporsegundo). Este factor hay que multiplicarlo 
por la medida del giroscopio para obtener mdps.

*/

#define FACTOR_CONVERSION_245DPS 0.00875
#define FACTOR_CONVERSION_500DPS 0.0175
#define FACTOR_CONVERSION_2000DPS 0.07  //Valor de la tabla/1000

#define DT 0.005

L3G gyro;

int16_t dps_x, dps_y, dps_z; //El tipo de dato es int16_t porque la salida del giroscopio es de ese tipo

int counter;

unsigned long time_loop1, time_loop2, time;

double angulo_giroscopio_x, angulo_giroscopio_y, angulo_giroscopio_z;


int sampleNum = 500;
int offsetg_x = 0, offsetg_y=0, offsetg_z=0;

void setup() {
  
 
  Serial.begin(115200);
  Wire.begin();

  if (!gyro.init())
  {
    Serial.println("Failed to autodetect gyro type!");
    while (1);
  }

  gyro.enableDefault();

  Configuracion_Registros();
  Calibracion();
  
  angulo_giroscopio_x = 0; 
  angulo_giroscopio_y = 0; 
  angulo_giroscopio_z = 0; 
}





void loop() {
  
  
  time_loop1 = millis();
  
  counter++;

  gyro.read();

  dps_x =  (gyro.g.x-offsetg_x)*FACTOR_CONVERSION_2000DPS;
  dps_y =  (gyro.g.y-offsetg_y)*FACTOR_CONVERSION_2000DPS;
  dps_z =  (gyro.g.z-offsetg_z)*FACTOR_CONVERSION_2000DPS;
 
    
  angulo_giroscopio_x+= (double) dps_x*DT; 
  angulo_giroscopio_y+= (double) dps_y*DT; 
  angulo_giroscopio_z+= (double) dps_z*DT;
 
  //Angulos entre 0-360º
  
  /*if (angulo_giroscopio_x < 0)
      angulo_giroscopio_x += 360;
  
  else if(angulo_giroscopio_x >= 360)
      angulo_giroscopio_x -= 360;
      
  if (angulo_giroscopio_y < 0)
      angulo_giroscopio_y += 360;
  
  else if(angulo_giroscopio_y >= 360)
      angulo_giroscopio_y -= 360;
  
  
  if (angulo_giroscopio_z < 0)
      angulo_giroscopio_z += 360;
  
  else if(angulo_giroscopio_z >= 360)
      angulo_giroscopio_z -= 360;
  
  */
  
  //Mostrar en el monitor serie cada 100 ms
 
  
  if (counter == 1)
 { 
   
   //Imprimir_Angulos(angulo_giroscopio_x, angulo_giroscopio_y, angulo_giroscopio_z);
   Imprimir_Para_Graficas(dps_y, angulo_giroscopio_y);
   counter = 0;
 }

  time_loop2 = millis(); //Tiempo_ejecucion = time_loop2-time_loop1 = 100 ms = DT = 0.1 s
  
   /*Serial.print("Tiempo de bucle:");
   time = time_loop2-time_loop1;
   Serial.println(time);
 */
 
  while(millis()-time_loop1 < (1000*DT)) //Timing->hay que asegurarse de que cada iteracion dura 5 ms, comprobado previamente con time_loop1 y time_loop2
  {
    delay(1);
  }
  
}
  
  
  //FUNCIONES
  
  void Configuracion_Registros()
  {
  
  //gyro.writeReg(L3G::CTRL_REG1, 0b010000111); //configuracion output data rate y ancho de banda
  
  gyro.writeReg(L3G::CTRL_REG2, 0b00100000); //Filtro paso alto
  
  //gyro.writeReg(L3G::CTRL_REG4, 0x20); //Modificacion fondo de escala

   
  /*La linea gyro.writeReg(L3G::CTRL_REG4, 0x10); cambia el fondo de escala, que por defecto al llamar a
  enableDefault se configura a +-250 dps. Los valores posibles son:
     
       +-245 dps: 0x00 o 0b00000000;
       +-500 dps: 0x10 o 0b00010000;
       +-2000 dps: 0x20 o 0x30 o 0b00100000; 
       
  Los bits del registro correspondientes a FS1 y FS0 son X X FS1 FS0 X X X X
  
  */
    
  }
  
  void Imprimir_Angulos(double x, double y, double z)
  {
    Serial.print("Angulo X:");
   Serial.println((int) x);
   
   Serial.print("Angulo Y:");
   Serial.println((int) y);
   
   Serial.print("Angulo Z:");
   Serial.println((int) z);
    
  }
  
  void Imprimir_Para_Graficas(double velocidady, double anguloy)
  {
   Serial.print((int) velocidady); 
   Serial.print("\t");
   Serial.print((int) anguloy); 
   Serial.print("\t\n");
   
  }
  
  
  
  void Calibracion()
  {
     //Calcular el offset
 
for(int n=0;n<sampleNum;n++){
  
  gyro.read();
  offsetg_x+=(int)gyro.g.x;
  offsetg_y+=(int)gyro.g.y;
  offsetg_z+=(int)gyro.g.z;

}
offsetg_x = offsetg_x/sampleNum;
offsetg_y = offsetg_y/sampleNum;
offsetg_z = offsetg_z/sampleNum;


//Imprimir offset
Serial.println();
Serial.print(" Offset X: ");
Serial.print(offsetg_x);
Serial.print(" /tOffset y: ");
Serial.print(offsetg_y);
Serial.print(" /tOffset Z: ");
Serial.print(offsetg_z);
delay(1000);  
  }
  
