#ifndef __HARDWARE_CONFIG_H
#define __HARDWARE_CONFIG_H

#include "lib_config.h"

void clock_init(void);
void GPIO_init(void);
void GPIO_all_deinit(void);
void LED_set(void);
void LED_reset(void);
void LED_toggle(void);

#endif