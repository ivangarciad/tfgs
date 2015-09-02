
#define MAXIMO_INTEGRAL 200

void PIP(){
  
    error_pi = ANGULO_REFERENCIA - angulo_y; 
    integral_pi+=(error_pi*DT);
    valor_PI = Kpi_p*error_pi + Kpi_i*integral_pi;
    error_p = valor_PI - dps_y;
    
    if(integral_pi > MAXIMO_INTEGRAL)       //Hay que limitar el valor de integral para que no se acumule demasiado y despues tarde mucho en corregir
        integral_pi = MAXIMO_INTEGRAL;
    else if(integral_pi < -MAXIMO_INTEGRAL)
        integral_pi = -MAXIMO_INTEGRAL;
        
    
     
    PIP_angulo = Kp_p*error_p;
  
  
}
