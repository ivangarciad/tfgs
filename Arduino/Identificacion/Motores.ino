
//Defines MOTORES
#define MAX_SIGNAL                 2000
#define MIN_SIGNAL                 700   //Este es el valor minimo que se le puede aplicar, no el valor al que arranca

#define MAX_VEL                    1500
#define MIN_VEL                    1000   
#define MOTOR_IZQUIERDO_PIN        9     //El que queda mas alejado del arduino
#define MOTOR_DERECHO_PIN          10

#define Throttle_Motor_Izquierdo   1250   //Esta es la velocidad a la que van a girar los motores si el error es 0
#define Throttle_Motor_Derecho     1300



void Calibracion_Motores(){
  
      motorderecho.attach(MOTOR_DERECHO_PIN);
      motorizquierdo.attach(MOTOR_IZQUIERDO_PIN); 
      Serial.println("                  CALIBRACION DEL ESC.\n");
      Serial.println("Escribiendo valor maximo.\n");
      Serial.println("Conecta la bateria (LED verde encendido), espera 3 segundos al segundo sonido del ESC, \n "); 
      Serial.println("el primer sonido indica que se ha encendido el ESC, y pulsa cualquier tecla para continuar.\n");
      motorderecho.writeMicroseconds(MAX_SIGNAL);
      motorizquierdo.writeMicroseconds(MAX_SIGNAL);

    // Esperando a que se pulse una tecla
      while (!Serial.available());
      Serial.read();

    // Enviar valor minimo 
      Serial.println("Enviando valor minimo, que inicia el modo throttle de los motores\n\n");
      motorderecho.writeMicroseconds(MIN_SIGNAL);
      motorizquierdo.writeMicroseconds(MIN_SIGNAL);
  
 }
 

void Velocidad_Motores(){
  
     velocidad_motor_izquierdo = Throttle_Motor_Izquierdo;
     velocidad_motor_derecho   = Throttle_Motor_Derecho;
     
   //Limitar velocidad para que este dentro de los limites
   
     velocidad_motor_izquierdo = (velocidad_motor_izquierdo > MAX_VEL) ? MAX_VEL:velocidad_motor_izquierdo;
     velocidad_motor_izquierdo = (velocidad_motor_izquierdo < MIN_VEL) ? MIN_VEL:velocidad_motor_izquierdo; //Velocidad minima para que no se pare el motor izquierdo
     
     velocidad_motor_derecho = (velocidad_motor_derecho > MAX_VEL) ? MAX_VEL:velocidad_motor_derecho;
     velocidad_motor_derecho = (velocidad_motor_derecho < MIN_VEL) ? MIN_VEL:velocidad_motor_derecho;//Velocidad minima para que no se pare el motor derecho
     
     
     
  //Aplicar velocidad a los motores   
     motorizquierdo.writeMicroseconds(velocidad_motor_izquierdo);
     motorderecho.writeMicroseconds(velocidad_motor_derecho);
  
  
  
}
