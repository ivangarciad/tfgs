// Uncomment the below line to use this axis definition: 
   // X axis pointing forward
   // Y axis pointing to the right 
   // and Z axis pointing down.
// Positive pitch : nose up
// Positive roll : right wing down
// Positive yaw : clockwise
int SENSOR_SIGN[9] = {1,1,1,-1,-1,-1}; //Correct directions x,y,z - gyro, accelerometer


// Uncomment the below line to use this axis definition: 
   // X axis pointing forward
   // Y axis pointing to the left 
   // and Z axis pointing up.
// Positive pitch : nose down
// Positive roll : right wing down
// Positive yaw : counterclockwise
//int SENSOR_SIGN[9] = {1,-1,-1,-1,1,1,1}; //Correct directions x,y,z - gyro, accelerometer



//Defines ACELEROMETRO

/*Los siguientes factores sirven para convertir los valores dados
por el acelerometro a aceleraciones medidas en g. Las unidades de estos factores 
son mg.  Este factor hay que multiplicarlo por la medida para obtener mg.
*/

#define FACTOR_CONVERSION_2G       0.000061 //Valor de la tabla en mg/1000
#define FACTOR_CONVERSION_4G       0.000122
#define FACTOR_CONVERSION_6G       0.000183
#define FACTOR_CONVERSION_8G       0.000244
#define FACTOR_CONVERSION_16G      0.000732

//Defines GIROSCOPIO

/*Los siguientes factores sirven para convertir los valores dados
por el giroscopio a grados por segundo. Las unidades de estos factores 
son mdps(miligradosporsegundo). Este factor hay que multiplicarlo 
por la medida para obtener mdps.

*/

#define FACTOR_CONVERSION_250DPS    0.00875
#define FACTOR_CONVERSION_500DPS    0.0175
#define FACTOR_CONVERSION_2000DPS   0.07  //Valor de la tabla en mdps/1000


void Calculo_Angulos_Acelerometro()
{
  
    compass.read();
   
    fuerza_x =  SENSOR_SIGN[3]*((compass.a.x-offseta_x)*FACTOR_CONVERSION_2G); 
    fuerza_y =  SENSOR_SIGN[4]*((compass.a.y-offseta_y)*FACTOR_CONVERSION_2G);
    fuerza_z =  SENSOR_SIGN[5]*((compass.a.z-offseta_z)*FACTOR_CONVERSION_2G);
  
    raiz_x = sqrt(pow(fuerza_x,2)+pow(fuerza_z,2));
    raiz_y = sqrt(pow(fuerza_y,2)+pow(fuerza_z,2));
  
    //angulo_acelerometro_x = (atan2(fuerza_y,raiz_x))*RAD_TO_DEG;  ////Equations 28 and 29 of http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf
    //angulo_acelerometro_y = (atan2(-fuerza_x, fuerza_z))*RAD_TO_DEG; 
  
    //angulo_acelerometro_x = (atan2(fuerza_y,raiz_x))*RAD_TO_DEG;  //Equations 37 and 38
    angulo_acelerometro_y = (atan2(-fuerza_x,raiz_y))*RAD_TO_DEG;
  
}

void Angulos_Giroscopio()
{
    gyro.read();
    
    dps_y =  SENSOR_SIGN[1]*(gyro.g.y-offsetg_y)*FACTOR_CONVERSION_2000DPS;
    angulo_giroscopio_y+= (double) dps_y*DT; 
   
}

