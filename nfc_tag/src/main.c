
#include "delay.h"
#include "lib_config.h"
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
  get_app_config()->start_flag == TRUE;
   
  while(1) {
    switch(get_app_config()->app_mode) {
      case IDLE:
        if(get_app_config()->start_flag == TRUE){
          get_app_config()->start_flag == FALSE;
          termometer_init();
          LED_reset();
        }
        temperature_get_display();
        delayLFO_ms (2);
      break;
     
      case PROGRAM_START:
        if(get_app_config()->start_flag == TRUE){
          get_app_config()->start_flag == FALSE;
          programming_start();
        }
        wait_for_text();
      break;
      
      case SEND_TEXT:
        
      break;
      
      case PROGRAM_FINISH:
      break;
      
      case WAIT:
      break;
      
      default:
          LCD_GLASS_Clear();
          LCD_GLASS_DisplayString("Error");
       break;
    }
  }
}	


