
#include "app_manager.h"

app_config_t app_config;
const uint8_t ProgramMessage[PROGRAM_TEXT_LEN]={'P','R','O','G','R','A','M','M','I','N','G',' ','S','T','A','R','T',' ',' '};
const uint8_t ReadMessage[READ_TEXT_LEN]={'N','F','C',' ','R','E','A','D',' ','T','E','X','T',' ',' ',' '};
uint8_t counter;

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
    /* I2C Init */
  I2C_Cmd(BOARD_I2C, DISABLE);
  
    /* I2C DeInit */
  I2C_DeInit(BOARD_I2C);
  
  CLK_PeripheralClockConfig(BOARD_I2C_CLK, DISABLE);
}

void programming_start ()
{
  I2C_reconfig();
  LED_set();
}

static void display_message (uint8_t message[], uint8_t length, bool ones)
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
            
    if(ones == TRUE)
       LCD_GLASS_ScrollSentenceOnes(message,60,length);
    else
      LCD_GLASS_ScrollSentenceNbCar(message,50,length);		
            
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

void read_text_message ()
{
  LCD_GLASS_Clear();
  delayLFO_ms(2);
  get_app_config()->text_message_stop = 0;
  display_message((uint8_t*)(ReadMessage), READ_TEXT_LEN, TRUE);
  LCD_GLASS_Clear();
  delayLFO_ms(3);
}

void wait_for_button () 
{
  get_app_config()->text_message_stop = 0;
  if(get_app_config()->app_mode == PROGRAM_START)
    display_message((uint8_t*)(ProgramMessage),PROGRAM_TEXT_LEN,FALSE);
  else if(get_app_config()->app_mode == PROGRAM_FINISH){
    LED_toggle();
    display_message(get_app_config()->text_message, get_app_config()->text_message_length,FALSE);
    LED_toggle();
    delayLFO_ms(2);
  }
  else {
     LCD_GLASS_Clear();
     LCD_GLASS_DisplayString("Error");
     while(get_app_config()->app_mode == 0xFF) {
     }
  }  
}

static void clear_text_message_buff ()
{
  uint8_t counter;
  for(counter = 0; counter<MAX_TEXT_LEN; counter++){
    get_app_config()->text_message[counter] = 0;
  }
}

static ErrorStatus nfc_ready ()
{
  uint8_t read_value = 0x00;
	
  // check the E1 at CONFIG1
  M24LR04E_ReadOneByte (M24LR04E_CONFIG1_ADDRESS, &read_value);	
  if (read_value != 0xE1)
          return ERROR;
  
  // text or URL flag at CONFIG2
  M24LR04E_ReadOneByte (M24LR04E_CONFIG2_ADDRESS, &read_value);	
  if (read_value != 0x54)
      return ERROR;

  return SUCCESS;	
}

uint8_t tmp;
static ErrorStatus get_message_length ()
{
  M24LR04E_ReadOneByte(M24LR04E_MESSAGE_LEN_ADDRESS, &get_app_config()->text_message_length);
  if (get_app_config()->text_message_length == 0x00)
      return ERROR;
		
  return SUCCESS;	
}

static void ToUpperCase (uint8_t buffer_length ,uint8_t *buffer)
{
    uint8_t i;
  for (i=0;i<buffer_length;i++) {
    if (buffer[i] >= 0x61 && buffer[i] <= 0x7A){
          buffer[i] -=32;
    }
  }
}

ErrorStatus read_text_from_nfc ()
{ 
  get_app_config()->text_message_length = 0;
  clear_text_message_buff();
  
  M24LR04E_Init();
  
  if(nfc_ready() == SUCCESS){
    if(get_message_length () == SUCCESS) {
      M24LR04E_ReadBuffer(M24LR04E_TEXT_ADDRESS,get_app_config()->text_message_length, get_app_config()->text_message);
    }
    else {
      M24LR04E_DeInit();
      return ERROR;
    }
  }
  else {
    M24LR04E_DeInit();
    return ERROR;
  }
  
  M24LR04E_DeInit();
  
  ToUpperCase(get_app_config()->text_message_length, get_app_config()->text_message);
  
  return SUCCESS;
}
