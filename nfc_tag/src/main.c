
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
   
  while(1) {
    switch(get_app_config()->app_mode) {
      case IDLE:
        if(get_app_config()->start_flag == TRUE){
          get_app_config()->start_flag = FALSE;
          termometer_init();
          LED_reset();
        }
        temperature_get_display();
        delayLFO_ms (1);
      break;
     
      case PROGRAM_START:
        if(get_app_config()->start_flag == TRUE){
          get_app_config()->start_flag = FALSE;
          programming_start();
        }
        wait_for_button();
      break;
      
      case PROGRAM_FINISH:
        if(get_app_config()->start_flag == TRUE){
          get_app_config()->start_flag = FALSE;
          ErrorStatus err = read_text_from_nfc();
          if(err == ERROR){
            get_app_config()->app_mode = 0xFF;
            break;
          }
          else{
            read_text_message();
            delayLFO_ms (10);
          }
        }
        wait_for_button();
      break;

      default:
         wait_for_button();
       break;
    }
  }
}
