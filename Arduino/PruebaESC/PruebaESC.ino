#include <Servo.h>

#define MAX_SIGNAL 2000
#define MIN_SIGNAL 700
#define MOTOR1_PIN 9   //A la izquierda del arduino, el mas alejado
#define MOTOR2_PIN 10

Servo motor1, motor2;

void setup() {
  Serial.begin(115200);
  Serial.println("PROGRAMA DE CALIBRACION DEL ESC.");

  motor1.attach(MOTOR1_PIN);
  motor2.attach(MOTOR2_PIN);

  Serial.println("Escribiendo valor maximo.\n");
  Serial.println("Conecta la bateria (LED verde encendido), espera 3 segundos al segundo sonido del ESC, \n "); 
  Serial.println("el primero indica que se ha encendido,  y pulsa cualquier tecla para salir del modo programacion.\n");
  motor1.writeMicroseconds(MAX_SIGNAL);
  motor2.writeMicroseconds(MAX_SIGNAL);

  // Esperando a que se pulse una tecla
  while (!Serial.available());
  Serial.read();

  // Enviar valor minimo 
  Serial.println("Enviando valor minimo\n");
  //motor1.writeMicroseconds(MIN_SIGNAL);
  motor2.writeMicroseconds(MIN_SIGNAL);
delay(4000);

}

void loop() {  

   Serial.println("Pulsa una tecla para continuar");
  while (!Serial.available());
  Serial.read();
  
  int speed;

 // sweep up from 0 to to maximum speed in 20 seconds
 for(speed = 0; speed <= 60; speed += 5) {
   setSpeed(speed);
   
   Serial.print("\n");
   if(speed == 0)
         Serial.print("Primer bucle for de 0 a 100 en intervalos de 10 cada segundo");
   
   Serial.print("\n");
   Serial.print("\n");
   Serial.print("Velocidad del motor en %: ");
   Serial.print(speed);
   delay(1000);
 }
 Serial.print("\n");
 Serial.print("\n");
 Serial.print("Esperando el paso del primer bucle for al segundo bucle for");
 delay(2000);
 Serial.print("\n");
 
 // sweep back down to 0 speed.
 for(speed = 60; speed > 0; speed -= 5) {
   setSpeed(speed);
  
   Serial.print("\n");
   if(speed == 20)
       Serial.print("Segundo bucle for, desde 95 hasta 0 de bajando un 5% cada segundo");
   Serial.print("\n");
   Serial.print("\n");
   Serial.print("Velocidad del motor en %: ");
   Serial.print(speed);

   delay(1000);
 }
  
 setSpeed(0);  
 Serial.print("\n");
 delay(5000); // stop the motor for 5 seconds
  
}


void setSpeed(int speed){
 // speed is from 0 to 100 where 0 is off and 100 is maximum speed
 //the following maps speed values of 0-100 to angles from 0-180,
 // some speed controllers may need different values, see the ESC instructions
 int angle = map(speed, 0, 100, MIN_SIGNAL, MAX_SIGNAL);
  motor1.writeMicroseconds(angle); 
  motor2.writeMicroseconds(angle); 
}
