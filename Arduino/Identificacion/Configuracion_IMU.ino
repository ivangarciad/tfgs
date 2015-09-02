 
 
 void Configuracion_Acelerometro()
{
  
  
  //INICIALIZACION
    compass.init();
    compass.enableDefault();  
  
  /*
  compass.enableDefault() enables the LSM303's accelerometer and magnetometer. Also:

        -Sets sensor full scales (gain) to default power-on values, which are+/- 2 g for accelerometer and +/- 4 gauss for magnetometer

        - Selects 50 Hz ODR (output data rate) for accelerometer and 7.5 Hz ODR for magnetometer (6.25 Hz on LSM303D). (These are the ODR

              settings for which the electrical characteristics are specified in the datasheets.)

        - Enables high resolution modes (if available).

  Note that this function will also reset other settings controlled by

  the registers it writes to.  
  
  */
  
  //CONFIGURAR REGISTROS
  
    compass.writeReg(LSM303::CTRL1, 0x37);//ODR = 37 Hz
  
    compass.writeReg(LSM303::CTRL2, 0x00);
  
  /*
  La linea compass.writeReg(LSM303::CTRL2, 0x10) cambia el fondo de escala, 
que por defecto al llamar a enableDefault se configura a +-2 g. 
Los valores posibles son:
     
         FS    VALOR REGISTRO CTRL2 
       +-2 g: 0x00;
       +-4 g: 0x08;
       +-6 g: 0x10; 
       +-8 g: 0x18;
       +-16 g:0x20;
       
Los bits del registro son:

  X X FS2 FS1 FS0 X X X        
       
*/
}


void Configuracion_Giroscopio()
{
  
  //INICIALIZACION
     if (!gyro.init()){
       
      Serial.println("Failed to autodetect gyro type!");
      while (1);
    }

    gyro.enableDefault();
  
  /*
  gyro.enableDefault() enables the L3G's gyro. Also:
      -Sets gyro full scale (gain) to default power-on value of +/- 245 dps for L3GD20H).
      -Selects 189.4 Hz ODR (output data rate).
  Note that this function will also reset other settings controlled by
  the registers it writes to.
*/

  
  
  //CONFIGURAR REGISTROS
    gyro.writeReg(L3G::CTRL1, 0x0F); //Configuracion ODR = 12.5 Hz y BW
  
    gyro.writeReg(L3G::CTRL2, 0b00100000); //Filtro paso alto
  
    gyro.writeReg(L3G::CTRL4, 0b00100000); //Modificacion fondo de escala

   
  /*La linea gyro.writeReg(L3G::CTRL_REG4, 0x10); cambia el fondo de escala, que por defecto al llamar a
  enableDefault se configura a +-250 dps. Los valores posibles son:
     
       +-245 dps: 0x00 o 0b00000000;
       +-500 dps: 0x10 o 0b00010000;
       +-2000 dps: 0x20 o 0x30 o 0b00100000; 
       
  Los bits del registro correspondientes a FS1 y FS0 son X X FS1 FS0 X X X X
  
  */
  
}

void Calibracion_IMU()
{
     //Calcular el offset del giroscopio y del acelerometro
 
    for(int n=0;n<sampleNum;n++){
      
        gyro.read();
        offsetg_x+=(int)gyro.g.x;
        offsetg_y+=(int)gyro.g.y;
        offsetg_z+=(int)gyro.g.z;
  
        compass.read();
        offseta_x+=(int)compass.a.x;
        offseta_y+=(int)compass.a.y;
        offseta_z+=(int)compass.a.z;
    }
    
    offsetg_x = offsetg_x/sampleNum;
    offsetg_y = offsetg_y/sampleNum;
    offsetg_z = offsetg_z/sampleNum;

    offseta_x = offseta_x/sampleNum;
    offseta_y = offseta_y/sampleNum;
    offseta_z = offseta_z/sampleNum;
  
}



