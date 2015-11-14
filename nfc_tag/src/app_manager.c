
#include "app_manager.h"

app_config_t app_config;
const uint8_t ProgramMessage[PROGRAM_TEXT_LEN]={'P','R','O','G','R','A','M','M','I','N','G',' ','S','T','A','R','T'};

uint8_t header[HEADER_TEXT_LEN] = {0};
uint8_t sign_counter;
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
  CLK_PeripheralClockConfig(BOARD_I2C_CLK, ENABLE);

  /* I2C DeInit */
  I2C_DeInit(BOARD_I2C);

  /* I2C configuration */
   I2C_Init(BOARD_I2C, 100000, BOARD_I2C_ADDRESS<<1, I2C_Mode_SMBusDevice,
            I2C_DutyCycle_2, I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);

  /* I2C Init */
  I2C_Cmd(BOARD_I2C, ENABLE);
  
  I2C_ITConfig(BOARD_I2C, (I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF), ENABLE);
}

static void header_buff_clear ()
{
  for(counter = 0; counter<4; counter++){
    header[counter] = 0;
  }
}

static void clear_text_message_buff ()
{
  uint8_t counter;
  for(counter = 0; counter<MAX_TEXT_LEN; counter++){
    get_app_config()->text_message[counter] = 0;
  }
}
void programming_start ()
{
  get_app_config()->header_received = 0;
  get_app_config()->text_message_length = 0;
  get_app_config()->text_received = 0;
  header_buff_clear();
  clear_text_message_buff();
  LED_set();
  I2C_reconfig();
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
  if(get_app_config()->text_received == 0)
    display_message((uint8_t*)(ProgramMessage), PROGRAM_TEXT_LEN);
  else {
    get_app_config()->text_received = 0;
    get_app_config()->app_mode = PROGRAM_FINISH;
    get_app_config()->start_flag = TRUE;
  }
}

void text_message_received (uint8_t sign) 
{
  for(counter = 0; counter<HEADER_TEXT_LEN-1; counter++){
    header[counter] = header[counter+1];
  }
  
  header[3] = sign;
  
  if(header[0] == 'A' && header[1] == 'x' && header[2] == 'A' && header[3] == 'x'){
    if(get_app_config()->header_received == 0){
      get_app_config()->header_received = 1;
      sign_counter = 1;
    }
    else if (get_app_config()->header_received == 1){
       get_app_config()->header_received = 0;
       get_app_config()->text_message[sign_counter++] = 0xFE;
       get_app_config()->text_message_length += sign_counter;
       get_app_config()->text_received = 1;
    }
  }
  
  if(get_app_config()->header_received == 1)
    get_app_config()->text_message[sign_counter++] = sign;
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

uint8_t iteration_num = 0;
uint8_t text_buff[10] = {0};
ErrorStatus send_text_to_nfc ()
{
  uint8_t text_length =  get_app_config()->text_message_length;
  uint8_t *data_pointer = get_app_config()->text_message;
  uint16_t text_address = M24LR04E_TEXT_ADDRESS;
  
  
  iteration_num = text_length/4;
  if(iteration_num%4 !=0)
    iteration_num+=1;
  
  M24LR04E_Init();
  if(nfc_ready() == SUCCESS){
    for(counter=0; counter<iteration_num; counter++){
      M24LR04E_WritePage(text_address, data_pointer);
      delayLFO_ms (2);
      text_address+=4;
      data_pointer+=4;
    }
  }
  else 
    return ERROR;

  M24LR04E_WriteOneByte(M24LR04E_MESSAGE_LEN_ADDRESS, text_length+1);
  delayLFO_ms (2);
  
  M24LR04E_DeInit();
  
  return SUCCESS;
}

void wait_for_button () 
{
  display_message(get_app_config()->text_message, get_app_config()->text_message_length);
}