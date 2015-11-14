#ifndef __APP_MANAGER_H
#define __APP_MANAGER_H

#include "stm8l15x.h"
#include "lib_config.h"
#include "hardware_config.h"
#include "STTS751_driver.h"
#include "LCD_driver.h"
#include "delay.h"

#define USE_HSI

#define PROGRAM_TEXT_LEN               17
#define BOARD_I2C_ADDRESS              0x53

typedef struct {
  uint8_t app_mode;
  uint8_t temperature;
  uint8_t start_flag;
  uint8_t text_message[40];
  uint8_t text_message_length;
  uint8_t header_received;
} app_config_t, *app_config_p;

typedef enum {
  IDLE = 0, 
  PROGRAM_START,
  SEND_TEXT,
  PROGRAM_FINISH,
  WAIT
} mode;


void LED_display_init(void);
void termometer_init(void);
void temperature_get_display(void);

void programming_start(void);
void wait_for_text(void); 

bool text_message_received (uint8_t sign);

app_config_p get_app_config(void); 
#endif