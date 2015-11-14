#ifndef __HARDWARE_CONFIG_H
#define __HARDWARE_CONFIG_H

#include "stm8l15x.h"
#include "lib_config.h"

void clock_init(void);
void clock_all_deinit(void);
void GPIO_init(void);
void GPIO_all_deinit(void);
void LED_set(void);
void LED_reset(void);

#endif