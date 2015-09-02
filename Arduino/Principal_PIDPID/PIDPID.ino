
#define MAXIMO_INTEGRAL 200

void PIDPID(){

    //PID del angulo
    error_PID1 = ANGULO_REFERENCIA - angulo_y; 
    integral_PID1+=(error_PID1*DT);
    
    if(integral_PID1 > MAXIMO_INTEGRAL)       //Hay que limitar el valor de integral para que no se acumule demasiado y despues tarde mucho en corregir
        integral_PID1 = MAXIMO_INTEGRAL;
    else if(integral_PID1 < -MAXIMO_INTEGRAL)
        integral_PID1 = -MAXIMO_INTEGRAL;
        
    derivativo_PID1 = (error_PID1-error_pasado_PID1)/DT;

    PID1 = Kp1*error_PID1 + Ki1*integral_PID1 + Kd1*derivativo_PID1;

    
    //PID de la velocidad angular
    error_PID2 = PID1 - dps_y; 
    integral_PID2+=(error_PID2*DT);
    
    if(integral_PID2 > MAXIMO_INTEGRAL)       //Hay que limitar el valor de integral para que no se acumule demasiado y despues tarde mucho en corregir
        integral_PID2 = MAXIMO_INTEGRAL;
    else if(integral_PID2 < -MAXIMO_INTEGRAL)
        integral_PID2 = -MAXIMO_INTEGRAL;
        
    derivativo_PID2 = (error_PID2-error_pasado_PID2)/DT;
    
     
    PIDPID_angulo = Kp2*error_PID2 + Ki2*integral_PID2 + Kd2*derivativo_PID2;
  
  
}
