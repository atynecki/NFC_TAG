
#include "app_manager.h"

app_config_t app_config;
const uint8_t ProgramMessage[PROGRAM_TEXT_LEN]={'P','R','O','G','R','A','M','M','I','N','G',' ','S','T','A','R','T'};

app_config_p get_app_config () 
{
  return &app_config;
}

void LED_display_init () 
{
  LCD_GLASS_Init();
  LCD_GLASS_Clear();
}

void termometer_init () 
{
    /* config the TS in OneShotMode */
    TS_Init();
    TS_Config(STTS751_STOP);
}

double temp_value;
uint8_t msb_value;
uint8_t lsb_value;
double temperature_measurement ()
{    
     TS_Config(STTS751_ONESHOTMODE);
     delayLFO_ms (1);
     
     TS_ReadOneByte(&msb_value, STTS751_TEMP_H_REG);
     delayLFO_ms(1);
     TS_ReadOneByte(&lsb_value, STTS751_TEMP_L_REG);
     
     if(msb_value & 0x80){
	msb_value = msb_value & 0x3F;
	temp_value = 256 - ((double)(msb_value) + (double)(lsb_value)/256);
     }
     else {
	temp_value = ((double)(msb_value) + (double)(lsb_value)/256);
     }
     
     return temp_value;
}


void temperature_display (double temp)
{
    uint16_t TempChar16[6];
    
    TempChar16[5] = 'C';
    TempChar16[4] = ' ';
    // check if the temperature is negative
    if (temp < 0)
            TempChar16[0] = '-';
    else 
            TempChar16[0] = ' ';
    uint16_t temp_int = (uint16_t) (temp);
    uint16_t temp_frag = (uint16_t)(temp*10 - temp_int*10);
    
    TempChar16[1] = (temp_int /10) + 0x30;
    TempChar16[2] = (temp_int %10) + 0x30 | DOT;
    TempChar16[3] = temp_frag + 0x30;

    LCD_GLASS_DisplayStrDeci(TempChar16);		
}

void temperature_get_display ()
{
  double temp_value;
  
  temp_value = temperature_measurement();
  temperature_display(temp_value);
}


static void I2C_reconfig ()
{
  CLK_PeripheralClockConfig(BOARD_I2C_CLK, ENABLE);

  /* I2C DeInit */
  I2C_DeInit(BOARD_I2C);

  /* I2C configuration */
   I2C_Init(BOARD_I2C, 100000, BOARD_I2C_ADDRESS, I2C_Mode_SMBusDevice,
            I2C_DutyCycle_2, I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);

  /* I2C Init */
  I2C_Cmd(BOARD_I2C, ENABLE);
  
  I2C_ITConfig(BOARD_I2C, (I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF), ENABLE);
}

void programming_start ()
{
  LED_set();
  I2C_reconfig();
  get_app_config()->header_received = 0;
}

static void display_message (uint8_t message[], uint8_t length)
{
        //Switch the clock to LSI
	#ifdef USE_LSE
		CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
		CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSE);	
		CLK_SYSCLKSourceSwitchCmd(ENABLE);
		while (((CLK->SWCR)& 0x01)==0x01);
		CLK_HSICmd(DISABLE);
		CLK->ECKCR &= ~0x01; 
	#else
		CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
		CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);
		CLK_SYSCLKSourceSwitchCmd(ENABLE);
		while (((CLK->SWCR)& 0x01)==0x01);
		CLK_HSICmd(DISABLE);
		CLK->ECKCR &= ~0x01; 
	#endif		
		
		LCD_GLASS_ScrollSentenceNbCar(message,30,length+6);		
		
	#ifdef USE_HSI
                //Switch the clock to HSI
                CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_16);
                CLK_HSICmd(ENABLE);
                while (((CLK->ICKCR)& 0x02)!=0x02);			
                CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
                CLK_SYSCLKSourceSwitchCmd(ENABLE);
                while (((CLK->SWCR)& 0x01)==0x01);
        #else
                CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_2);
                CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
                while (((CLK->SWCR)& 0x01)==0x01);
                CLK_SYSCLKSourceSwitchCmd(ENABLE);
        #endif
}

void wait_for_text () 
{
  display_message((uint8_t*)(ProgramMessage), PROGRAM_TEXT_LEN);
}

uint8_t header [4] = {0};
uint8_t counter;
uint8_t sign_counter;
bool text_message_received (uint8_t sign) 
{
  for(counter = 0; counter<3; counter++){
    header[counter] = header[counter+1];
  }
  
  header[3] = sign;
  
  if(header[0] == 'A' && header[1] == 'x' && header[2] == 'A' && header[3] == 'z'){
    if(get_app_config()->header_received == 0){
      get_app_config()->header_received = 1;
      sign_counter = 0;
    }
    else if (get_app_config()->header_received == 1){
       get_app_config()->header_received = 0;
       get_app_config()-> text_message_length = sign_counter;
       return TRUE;
    }
  }
  
  if(get_app_config()->header_received == 1)
    get_app_config()->text_message[sign_counter++] = sign;
  
  return FALSE;
}
 