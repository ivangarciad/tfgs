



void Imprimir_Matlab(){


   Serial.print((int) angulo_y); 
   Serial.print("\t");

   Serial.print((int) ANGULO_REFERENCIA); 
   Serial.print("\t");

   Serial.print((int) velocidad_motor_izquierdo); 
   Serial.print("\t");

   Serial.print((int) velocidad_motor_derecho); 
   Serial.print("\t");

   Serial.print((int) Kp); 
   Serial.print("\t");

   Serial.print((int) Ki); 
   Serial.print("\t");

   Serial.print((int) Kd); 
   Serial.print("\t");

   Serial.print((int) derivativo); 
   Serial.print("\t");

   Serial.print((int) integral); 
   Serial.print("\t");
   
   Serial.print(tiempo_matlab); 
   Serial.print("\t\n");

   


  
}

