
#include "app_manager.h"

app_config_t app_config;


app_config_p get_app_config () 
{
  return &app_config;
}

void termometer_init () 
{
    /* config the TS in OneShotMode */
    TS_Init();
    TS_Config(STTS751_STOP);
    delayLFO_ms (1);
    TS_Config(STTS751_ONESHOTMODE);
}


void temperature_measurement (uint8_t *data_sensor)
{
     TS_ReadOneByte(data_sensor, STTS751_TEMP_H_REG);
}


void temperature_display (uint8_t temp)
{
    uint16_t TempChar16[6];
    
    TempChar16[5] = 'C';
    TempChar16[4] = ' ';
    // check if the temperature is negative
    if ((temp & 0x80) != 0)
    {
            temp = (~temp) +1;
            TempChar16[1] = '-';
    }
    else {
            TempChar16[1] = ' ';	
    }
    
    TempChar16[3] = (temp %10) + 0x30 ;
    TempChar16[2] = (temp /10) + 0x30;
    TempChar16[0] = ' ';
    LCD_GLASS_DisplayStrDeci(TempChar16);		
}