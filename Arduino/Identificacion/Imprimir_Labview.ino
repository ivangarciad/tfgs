
void Imprimir_LabView(){
  
   Serial.print((int) angulo_y); 
   Serial.print("\t");

   Serial.print((int) velocidad_motor_izquierdo); 
   Serial.print("\t");

   Serial.print((int) velocidad_motor_derecho); 
   Serial.print("\t\n");

  
}
