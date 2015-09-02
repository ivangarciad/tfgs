#include <Servo.h> 
#include <Wire.h>
#include <LSM303.h>
#include <L3G.h>

#define ANGULO_REFERENCIA 0


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


//Defines MOTORES

#define MAX_SIGNAL                 2000
#define MIN_SIGNAL                 700
#define MOTOR_IZQUIERDO_PIN        9     //El que queda mas alejado del arduino
#define MOTOR_DERECHO_PIN          10

#define Throttle_Motor_Izquierdo   1500   //
#define Throttle_Motor_Derecho     1500
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


//Defines PID

#define Kp 9  
#define Ki 1
#define Kd 4




#define DT 0.01 //Este valor es la duracion del bucle principal en segundos

#define RAD_TO_DEG 57.2958

    double angulo_y;
    int counter;
    unsigned long time_loop1, time_loop2, time;
    int sampleNum = 500;


  //VARIABLES MOTORES

    Servo motorderecho, motorizquierdo;
    int velocidad_motor_izquierdo, velocidad_motor_derecho;

  //VARIABLES ACELEROMETRO

    LSM303 compass;
    double fuerza_x, fuerza_y,fuerza_z;
    double angulo_acelerometro_x, angulo_acelerometro_y;
    double raiz_x, raiz_y;
    int offseta_x = 0, offseta_y=0, offseta_z=0;

  //VARIABLES GIROSCOPIO

    L3G gyro;
    double dps_x, dps_y, dps_z;
    double angulo_giroscopio_x, angulo_giroscopio_y, angulo_giroscopio_z;
    int offsetg_x = 0, offsetg_y=0, offsetg_z=0;

//VARIABLES PID

    float error, error_pasado;
    float derivativo, integral;
    float PID_angulo;


void setup()
{
     Serial.begin(115200);
     Wire.begin(); //Comunicacion I2C con la IMU
   
   //MOTORES
     motorderecho.attach(MOTOR_DERECHO_PIN);
     motorizquierdo.attach(MOTOR_IZQUIERDO_PIN);
     Calibracion_Motores();
   
   //ACELEROMETRO
    Configuracion_Acelerometro();
  
  //GIROSCOPIO
    Configuracion_Giroscopio();
 
    angulo_y = 0;
    angulo_giroscopio_x = 0; 
    angulo_giroscopio_y = 0; 
    angulo_giroscopio_z = 0;
    angulo_acelerometro_x = 0;
    angulo_acelerometro_y = 0;
    counter = 0;
    error = 0;
    error_pasado = 0;
  
  
    Calibracion_IMU();
 
    delay(2000);
  
}


void loop()
{
    time_loop1 = millis();
    counter++;
  
  //ACELEROMETRO
  
    Calculo_Angulos_Acelerometro();

  //GIROSCOPIO
  
    Angulos_Giroscopio();
       
  //FILTRO COMPLEMENTARIO
  
    angulo_y = 0.95*(angulo_y+dps_y*DT)+0.05*angulo_acelerometro_y;  //PITCH
  
  
  //Mostrar Valor actual del angulo
     
   //PID  
    
    error = ANGULO_REFERENCIA - angulo_y; 
    integral+=(error*DT);
    derivativo = (error-error_pasado)/DT;
     
    PID_angulo = Kp*error + Ki*integral + Kd*derivativo;
     
   //MOTORES
     
     velocidad_motor_izquierdo = Throttle_Motor_Izquierdo + PID_angulo;
     velocidad_motor_derecho   = Throttle_Motor_Derecho - PID_angulo;
     
   //Limitar velocidad para que este dentro de los limites
   
     velocidad_motor_izquierdo = (velocidad_motor_izquierdo > 2000) ? 2000:velocidad_motor_izquierdo;
     
     velocidad_motor_izquierdo = (velocidad_motor_izquierdo < 1000) ? 1000:velocidad_motor_izquierdo; //Velocidad minima para que no se pare el motor izquierdo
     
     velocidad_motor_derecho = (velocidad_motor_derecho > 2000) ? 2000:velocidad_motor_derecho;
     
     velocidad_motor_derecho = (velocidad_motor_derecho < 1000) ? 1000:velocidad_motor_derecho;//Velocidad minima para que no se pare el motor derecho
     
     motorizquierdo.writeMicroseconds(velocidad_motor_izquierdo);
     motorderecho.writeMicroseconds(velocidad_motor_derecho);
     
      if(counter == 20){
  
      Imprimir_Datos(angulo_y);
      Serial.print("Velocidad Motores\n");
      Serial.print("____________________\n");
      Serial.print("Izquierdo\tDerecho \n");
      Serial.print("____________________\n");
      Serial.print(velocidad_motor_izquierdo);
      Serial.print(" \t \t ");
      Serial.print(velocidad_motor_derecho);
      Serial.print("\n");
      counter = 0;
     }
     
    time_loop2 = millis(); 
   //Imprimir_tiempo_ejecucion_bucle();

 
    if(time_loop2-time_loop1 > 10){ //Timing->hay que asegurarse de que cada iteracion dura 10 ms
        Serial.print("\n EL TIEMPO DE EJECUCION HA SUPERADO EL LIMITE \n");
    }
    else
      delay(10 - (time_loop2-time_loop1));
    
    error_pasado = error;
  
}

//FUNCIONES

void Calibracion_Motores(){
  
      Serial.println("CALIBRACION DEL ESC.\n");
      Serial.println("Escribiendo valor maximo.\n");
      Serial.println("Conecta la bateria (LED verde encendido), espera 3 segundos al segundo sonido del ESC, \n "); 
      Serial.println("el primer sonido indica que se ha encendido el ESC, y pulsa cualquier tecla para salir del modo programacion.\n");
      motorderecho.writeMicroseconds(MAX_SIGNAL);
      motorizquierdo.writeMicroseconds(MAX_SIGNAL);

    // Esperando a que se pulse una tecla
      while (!Serial.available());
      Serial.read();

    // Enviar valor minimo 
      Serial.println("Enviando valor minimo, que inicia el modo throttle de los motores\n");
      motorderecho.writeMicroseconds(MIN_SIGNAL);
      motorizquierdo.writeMicroseconds(MIN_SIGNAL);
  
 }
 
 void Configuracion_Acelerometro()
{
  
  
  //INICIALIZACION
    compass.init();
    compass.enableDefault();  
  
  /*
  compass.enableDefault() enables the LSM303's accelerometer and magnetometer. Also:

        -Sets sensor full scales (gain) to default power-on values, which are+/- 2 g for accelerometer and +/- 4 gauss for magnetometer

        - Selects 50 Hz ODR (output data rate) for accelerometer and 7.5 Hz ODR for magnetometer (6.25 Hz on LSM303D). (These are the ODR

              settings for which the electrical characteristics are specified in the datasheets.)

        - Enables high resolution modes (if available).

  Note that this function will also reset other settings controlled by

  the registers it writes to.  
  
  */
  
  //CONFIGURAR REGISTROS
  
    compass.writeReg(LSM303::CTRL1, 0x37);//ODR = 37 Hz
  
    compass.writeReg(LSM303::CTRL2, 0x00);
  
  /*
  La linea compass.writeReg(LSM303::CTRL2, 0x10) cambia el fondo de escala, 
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
}


void Configuracion_Giroscopio()
{
  
  //INICIALIZACION
     if (!gyro.init()){
       
      Serial.println("Failed to autodetect gyro type!");
      while (1);
    }

    gyro.enableDefault();
  
  /*
  gyro.enableDefault() enables the L3G's gyro. Also:
      -Sets gyro full scale (gain) to default power-on value of +/- 245 dps for L3GD20H).
      -Selects 189.4 Hz ODR (output data rate).
  Note that this function will also reset other settings controlled by
  the registers it writes to.
*/

  
  
  //CONFIGURAR REGISTROS
    gyro.writeReg(L3G::CTRL1, 0x0F); //Configuracion ODR = 12.5 Hz y BW
  
    gyro.writeReg(L3G::CTRL2, 0b00100000); //Filtro paso alto
  
    gyro.writeReg(L3G::CTRL4, 0b00100000); //Modificacion fondo de escala

   
  /*La linea gyro.writeReg(L3G::CTRL_REG4, 0x10); cambia el fondo de escala, que por defecto al llamar a
  enableDefault se configura a +-250 dps. Los valores posibles son:
     
       +-245 dps: 0x00 o 0b00000000;
       +-500 dps: 0x10 o 0b00010000;
       +-2000 dps: 0x20 o 0x30 o 0b00100000; 
       
  Los bits del registro correspondientes a FS1 y FS0 son X X FS1 FS0 X X X X
  
  */
  
}

void Calibracion_IMU()
{
     //Calcular el offset del giroscopio y del acelerometro
 
    for(int n=0;n<sampleNum;n++){
      
        gyro.read();
        offsetg_x+=(int)gyro.g.x;
        offsetg_y+=(int)gyro.g.y;
        offsetg_z+=(int)gyro.g.z;
  
        compass.read();
        offseta_x+=(int)compass.a.x;
        offseta_y+=(int)compass.a.y;
        offseta_z+=(int)compass.a.z;
    }
    
    offsetg_x = offsetg_x/sampleNum;
    offsetg_y = offsetg_y/sampleNum;
    offsetg_z = offsetg_z/sampleNum;

    offseta_x = offseta_x/sampleNum;
    offseta_y = offseta_y/sampleNum;
    offseta_z = offseta_z/sampleNum;
  
}

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

void Imprimir_Datos(double anguloy)
  {
    Serial.print("Angulo Y: ");
    Serial.print((int) anguloy);  
    Serial.print("\n\n");
  }
  

void Imprimir_tiempo_ejecucion_bucle()
{
   Serial.print("\n");
   Serial.print("Tiempo de bucle:");
   time = time_loop2-time_loop1;
   Serial.println(time);
 }

