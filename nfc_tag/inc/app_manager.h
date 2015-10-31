#ifndef __APP_MANAGER_H
#define __APP_MANAGER_H

#include "stm8l15x.h"
#include "lib_config.h"
#include "hardware_config.h"
#include "STTS751_driver.h"
#include "LCD_driver.h"
#include "delay.h"

#define USE_HSI


typedef struct {
  uint8_t app_mode;
  uint8_t temperature;
  
} app_config_t, *app_config_p;

#endif