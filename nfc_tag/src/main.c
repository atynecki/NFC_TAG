
#include "app_manager.h"

#define text "Artur"
void main(void)
{
  clock_init();
  GPIO_init();
  
  LED_display_init();
  termometer_init();
  
  enableInterrupts();
  
  get_app_config()->app_mode = IDLE;
  get_app_config()->start_flag = TRUE;
  
  get_app_config()->text_message[0] = 0;
  get_app_config()->text_message[1] = 'T';
  get_app_config()->text_message[2] = 'E';
  get_app_config()->text_message[3] = 'S';
  get_app_config()->text_message[4] = 'T';
  get_app_config()->text_message[5] = ' ';
  get_app_config()->text_message[6] = 'T';
  get_app_config()->text_message[7] = 'E';
  get_app_config()->text_message[8] = 'K';
  get_app_config()->text_message[9] = 'S';
  get_app_config()->text_message[10] = 'T';
  get_app_config()->text_message[11] = 0xFE;
  get_app_config()->text_message_length = 12;
   
  while(1) {
    switch(get_app_config()->app_mode) {
      case IDLE:
        if(get_app_config()->start_flag == TRUE){
          get_app_config()->start_flag = FALSE;
          termometer_init();
          LED_reset();
        }
        temperature_get_display();
        delayLFO_ms (2);
      break;
     
      case PROGRAM_START:
        if(get_app_config()->start_flag == TRUE){
          get_app_config()->start_flag = FALSE;
          programming_start();
        }
        wait_for_text();
      break;
      
      case PROGRAM_FINISH:
        if(get_app_config()->start_flag == TRUE){
          get_app_config()->start_flag = FALSE;
          ErrorStatus err = send_text_to_nfc();
          if(err == ERROR){
            get_app_config()->app_mode = 0xFF;
            break;
          }
        }
        wait_for_button();
      break;

      default:
          LCD_GLASS_Clear();
          LCD_GLASS_DisplayString("Error");
       break;
    }
  }
}
