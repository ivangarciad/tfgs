

void Mostrar_Datos(double anguloy)
  {
    Serial.print("        Angulo Y\n");
    Serial.print("_________________________\n");
    Serial.print("  Medido\tReferencia \n");
    Serial.print("_________________________\n");
    Serial.print((int) anguloy);  
    Serial.print(" \t \t ");
    Serial.print((int) ANGULO_REFERENCIA);  
    Serial.print("\n\n");
    Serial.print("   Velocidad Motores\n");
    Serial.print("_________________________\n");
    Serial.print("Izquierdo\tDerecho \n");
    Serial.print("_________________________\n");
    Serial.print(velocidad_motor_izquierdo);
    Serial.print(" \t \t ");
    Serial.print(velocidad_motor_derecho);
    Serial.print("\n\n");
    Serial.print("   Constantes PID\n");
    Serial.print("_________________________\n");
    Serial.print("Kp\tKi\tKd \n");
    Serial.print("_________________________\n");
    Serial.print(Kp);  
    Serial.print(" \t ");
    Serial.print(Ki);
    Serial.print(" \t ");
    Serial.print(Kd);  
    Serial.print("\n\n\n\n\n\n");
    counter = 0;
  }
  

void Imprimir_tiempo_ejecucion_bucle()
{
   Serial.print("\n");
   Serial.print("Tiempo de bucle:");
   tiempo = time_loop2-time_loop1;
   Serial.println(tiempo);
 }

