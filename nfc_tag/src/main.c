/**
 * @name STM8 NFC TAG
 * @author Artur Tynecki
 * @brief programmable NFC TAG Board base on M24LR-BOARD (STM8) with temperature measurement
 * @version 1.0
 * 
 * @section DESCRIPTION
 * It is simple application for evaluation board M24LR-BOARD ST company
 * Application allows:
 * 	1. temperature measurement and display,
 * 	2. programinng NFC module from external programmer,
 * 	3. read NFC message from module and display.
 */

#include "app_manager.h"

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
            app_error_handler();
            break;
          }
          else
            read_text_message();
        }
        wait_for_button();
      break;

      default:
         app_error_handler();
       break;
    }
  }
}
