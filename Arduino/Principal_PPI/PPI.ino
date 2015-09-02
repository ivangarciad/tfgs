
#define MAXIMO_INTEGRAL 200

void PPI(){
  
    error_p = ANGULO_REFERENCIA - angulo_y; 
    error_pi = Kp*error_p - dps_y;
    integral+=(error_pi*DT);
    
    if(integral > MAXIMO_INTEGRAL)       //Hay que limitar el valor de integral para que no se acumule demasiado y despues tarde mucho en corregir
        integral = MAXIMO_INTEGRAL;
    else if(integral < -MAXIMO_INTEGRAL)
        integral = -MAXIMO_INTEGRAL;
        
    
    
     
    PPI_angulo = Kpi_p*error_pi + Kpi_i*integral;
  
  
}
