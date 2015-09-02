#include <Servo.h> 
#include <Wire.h>
#include <LSM303.h>
#include <L3G.h>

#define DT 0.03 //Este valor es la duracion del bucle principal en segundos

#define RAD_TO_DEG 57.2958

    double angulo_y, ANGULO_REFERENCIA = 0;
    int counter;
    unsigned long time_loop1, time_loop2, tiempo; //Variables para el timing del programa, hay que asegurarse que el bucle principal siempre dura DT
    float tiempo_matlab = 0;
    int sampleNum = 500;
    double matriz_angulo[80];


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




void setup()
{
     Serial.begin(115200);
     Wire.begin(); //Comunicacion I2C con la IMU
   
     Serial.print(" \n\n                      INSTRUCCIONES\n");
     Serial.print("______________________________________________________\n");
     Serial.print("Pulsa p para decrementar 1 Kp y P para incrementar 1\n");
     Serial.print("Pulsa d para decrementar 1 Kd y D para incrementar 1\n");
     Serial.print("Pulsa i para decrementar 0.1 Ki y I para incrementar 0.1\n");
     Serial.print("Pulsa - para decrementar 2 grados el angulo de referencia y + para incrementar 2\n\n");
   
    
   //MOTORES
     Calibracion_Motores();
   
   //ACELEROMETRO
    Configuracion_Acelerometro();
  
  //GIROSCOPIO
    Configuracion_Giroscopio();
 
    //angulo_y = 0;
    angulo_giroscopio_x = 0; 
    //angulo_giroscopio_y = 0; 
    angulo_giroscopio_z = 0;
    angulo_acelerometro_x = 0;
    //angulo_acelerometro_y = 0;
    velocidad_motor_derecho = 0;
    counter = 0;
   
    Calibracion_IMU();
       
    delay(2000);
}


void loop()
{
    time_loop1 = millis();
  
  //ACELEROMETRO
  
      Calculo_Angulos_Acelerometro();

  //GIROSCOPIO
  
       Angulos_Giroscopio();
       
  //FILTRO COMPLEMENTARIO
  
      angulo_y = 0.95*(angulo_y + dps_y*DT) + 0.05*angulo_acelerometro_y;  //PITCH

    Imprimir_LabView();
      
    
   //Mostrar Valor actual del angulo
     if(counter < 25){

     velocidad_motor_izquierdo = 1250;
     motorizquierdo.writeMicroseconds(velocidad_motor_izquierdo);
     
     }


    if ((counter>25)&&(counter<50))
    {
        velocidad_motor_izquierdo = 750;
        motorizquierdo.writeMicroseconds(velocidad_motor_izquierdo);

        velocidad_motor_derecho = 1250;
        motorderecho.writeMicroseconds(velocidad_motor_derecho);
      
    }

    if ((counter>50)&&(counter<75))
    {
        velocidad_motor_izquierdo = 1250;
        motorizquierdo.writeMicroseconds(velocidad_motor_izquierdo);

        velocidad_motor_derecho = 1250;
        motorderecho.writeMicroseconds(velocidad_motor_derecho);
      
    }
    
    if (counter>75){

        velocidad_motor_izquierdo = 750;
        motorizquierdo.writeMicroseconds(velocidad_motor_izquierdo);
        velocidad_motor_derecho = 750;
        motorderecho.writeMicroseconds(velocidad_motor_derecho);
     }

    counter++;
   //TIMING
     
     time_loop2 = millis(); 
   //Imprimir_tiempo_ejecucion_bucle();

     if(time_loop2-time_loop1 > (1000*DT)){ //Timing->hay que asegurarse de que cada iteracion dura 10 ms
         Serial.print("\n EL TIEMPO DE EJECUCION HA SUPERADO EL LIMITE \n");
      }
      else
         delay((1000*DT) - (time_loop2-time_loop1)); 
}


void Imprimir_tiempo_ejecucion_bucle()
{
   Serial.print("\n");
   Serial.print("Tiempo de bucle:");
   tiempo = time_loop2-time_loop1;
   Serial.println(tiempo);
 }






