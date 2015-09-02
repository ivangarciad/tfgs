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

    float Kp1 = 8, Kp2 = 8;  
    float Ki1 = 4, Ki2 = 5;
    float Kd1 = 3, Kd2 = 1;


    float error_PID1, error_pasado_PID1, error_PID2, error_pasado_PID2;
    float derivativo_PID1, integral_PID1, derivativo_PID2, integral_PID2;
    
    float PID1, PIDPID_angulo;


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
    counter = 0;
    error_PID1 = 0;
    error_pasado_PID1 = 0;
    error_PID2 = 0;
    error_pasado_PID2 = 0;
  
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
  
      angulo_y = 0.95*(angulo_y + dps_y*DT) + 0.05*angulo_acelerometro_y;  //PITCH

  //PID  
    
      PIDPID();
      
   //MOTORES
     
     Velocidad_Motores();
     
     error_pasado_PID1 = error_PID1;
     error_pasado_PID2 = error_PID2;
     
   //Mostrar Valor actual del angulo
     if(counter == 1){

        tiempo_matlab += DT;
        Imprimir_LabView();
        
        counter = 0;
      
     }
     
   //TIMING
     
     time_loop2 = millis(); 
   //Imprimir_tiempo_ejecucion_bucle();

     if(time_loop2-time_loop1 > (1000*DT)){ //Timing->hay que asegurarse de que cada iteracion dura 10 ms
         Serial.print("\n EL TIEMPO DE EJECUCION HA SUPERADO EL LIMITE \n");
      }
      else
         delay((1000*DT) - (time_loop2-time_loop1));
    
      
      
   
  
}









