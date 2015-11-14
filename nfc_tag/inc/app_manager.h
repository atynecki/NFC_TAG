#ifndef __APP_MANAGER_H
#define __APP_MANAGER_H

#include "hardware_config.h"
#include "STTS751_driver.h"
#include "LCD_driver.h"
#include "M24Lr04E-R_driver.h"
#include "delay.h"

typedef struct {
  uint8_t app_mode;
  uint8_t temperature;
  uint8_t start_flag;
  uint8_t text_message[MAX_TEXT_LEN+2];
  uint8_t text_message_length;
  uint8_t header_received;
  uint8_t text_received;
} app_config_t, *app_config_p;

typedef enum {
  IDLE = 0, 
  PROGRAM_START,
  PROGRAM_FINISH
} mode;


void LED_display_init(void);
void termometer_init(void);
void temperature_get_display(void);

void programming_start(void);
void wait_for_text(void); 

void text_message_received(uint8_t sign);

ErrorStatus send_text_to_nfc(void);
void wait_for_button(void); 

app_config_p get_app_config(void); 

#endif
