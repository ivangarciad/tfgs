#define ANGULO_REF_MAX  70
#define ANGULO_REF_MIN -70

float INCREMENTO_ANGULO_POSITIVO = 1;
float INCREMENTO_ANGULO_NEGATIVO = -1;
char val;

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */

void serialEvent(){      
  
  if( Serial.available() )    // si hay dato y el puerto lo lee
   {
      val = Serial.read();    // lee y almacena el dato en 'val'
   }
   
    switch (val){
      
      
      case '+':
        ANGULO_REFERENCIA += INCREMENTO_ANGULO_POSITIVO;
        if (ANGULO_REFERENCIA > ANGULO_REF_MAX)
              ANGULO_REFERENCIA -= INCREMENTO_ANGULO_POSITIVO;
      break;
      
      case '-':
        ANGULO_REFERENCIA += INCREMENTO_ANGULO_NEGATIVO;
        if (ANGULO_REFERENCIA < ANGULO_REF_MIN)
              ANGULO_REFERENCIA -= INCREMENTO_ANGULO_NEGATIVO;
      break;

      case 'R':
        INCREMENTO_ANGULO_POSITIVO+=1;
        INCREMENTO_ANGULO_NEGATIVO-=1;
      break;
      
      case 'r':
        INCREMENTO_ANGULO_POSITIVO-=1;
        INCREMENTO_ANGULO_NEGATIVO+=1;
      break;
      
      case 'd':
        Kp_p-=1;
      break;
      
      case 'D':
        Kp_p+=1;
      break;
      
      case 'i':
        Kpi_i-=1;
      break;
      
      case 'I':
        Kpi_i+=1;
      break;
      
      case 'p':
        Kpi_p-=1;
      break;
      
      case 'P':
        Kpi_p+=1;
      break;
      
    }  
}
