
#define MAXIMO_INTEGRAL 200

void PID(){
  
    error = ANGULO_REFERENCIA - angulo_y; 
    integral+=(error*DT);
    
    if(integral > MAXIMO_INTEGRAL)       //Hay que limitar el valor de integral para que no se acumule demasiado y despues tarde mucho en corregir
        integral = MAXIMO_INTEGRAL;
    else if(integral < -MAXIMO_INTEGRAL)
        integral = -MAXIMO_INTEGRAL;
        
    derivativo = (error-error_pasado)/DT;
    
     
    PID_angulo = Kp*error + Ki*integral + Kd*derivativo;
  
  
}
