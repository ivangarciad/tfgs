// this uses the Arduino servo library included with version 0012 

// caution, this code sweeps the motor up to maximum speed !
// make sure the motor is mounted securily before running.

#include <Servo.h> 

#define MAX_SIGNAL 2000
#define MIN_SIGNAL 700
#define MOTOR_IZQUIERDO_PIN 9 //El que queda mas alejado del arduino
#define MOTOR_DERECHO_PIN 10

Servo motorderecho, motorizquierdo;

void setup()
{
   Serial.begin(115200);
   Serial.println("CALIBRACION DEL ESC.\n");
 
   motorderecho.attach(MOTOR_DERECHO_PIN);
   motorizquierdo.attach(MOTOR_IZQUIERDO_PIN);

   Calibracion_Motores();
 
   delay(2000);
  
}


void loop()
{
   int velocidad;

   //Auemnta la velocidad de 0 al 50 % en intervalos en 10 segundos
   for(velocidad = 900; velocidad <= 1500; velocidad += 50) {
     FijarVelocidad(velocidad);
     Serial.print("\n");
   
     if(velocidad == 1000)
         Serial.print("Primer bucle for de 0 a 50 incrementando 5 cada segundo");
   
     Serial.print("\n");
     Serial.print("\n");
     Serial.print("Velocidad del motor en %: ");
     Serial.print(velocidad);
     delay(1000);
   }
   
   Serial.print("\n");
   Serial.print("\n");
   Serial.print("Esperando el paso del primer bucle for al segundo bucle for");
   delay(1000);
   Serial.print("\n");
 
   // sweep back down to 0 speed.
   for(velocidad = 1500; velocidad > 1000; velocidad -= 50) {
     FijarVelocidad(velocidad);
     Serial.print("\n");
   
     if(velocidad == 1500)
         Serial.print("Segundo bucle for, desde 95 hasta 0 de bajando un 5% cada segundo");
   
     Serial.print("\n");
     Serial.print("\n");
     Serial.print("Velocidad del motor en %: ");
     Serial.print(velocidad);

     delay(1000);
   }
  
   FijarVelocidad(0);  
   Serial.print("\n");
   delay(2000); 
}




//FUNCIONES

 void Calibracion_Motores(){
  
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
  
void FijarVelocidad(int velocidad){
 // velocidad varia entre 0 y 100, cada motor arranca a un valor distinto, que hay que comprobar previamente
 //La funcion "map" mapea los valores de velocidad y los transfroma a valores
 //comprensibles por los motores en velocidad_motores, que es entre 700 y 2000 us
 //int velocidad_motores = map(velocidad, 0, 100, 1000, 2000);
  int velocidad_motores = velocidad;
  motorizquierdo.writeMicroseconds(velocidad_motores); 
  motorderecho.writeMicroseconds(velocidad_motores); 
}
