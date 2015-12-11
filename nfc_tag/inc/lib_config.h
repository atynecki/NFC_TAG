
#ifndef __LIB_CONFIG_H
#define __LIB_CONFIG_H

#include "stm8l15x.h"
#include "stm8l15x_i2c.h"
#include "stm8l15x_wwdg.h"

#define USE_HSI

/* MACROs for SET, RESET or TOGGLE Output port */

#define GPIO_HIGH(a,b) 		a->ODR|=b
#define GPIO_LOW(a,b)		a->ODR&=~b
#define GPIO_TOGGLE(a,b) 	a->ODR^=b

/* LED PIN */
#define LED_GPIO_PORT		             GPIOE
#define LED_GPIO_PIN		             GPIO_Pin_6

/* BUTTON PIN */
#define BUTTON_GPIO_PORT	             GPIOC
#define BUTTON_GPIO_PIN		             GPIO_Pin_7

/** M24LR16E-R I2C CONFIG */
#define M24LR04E_I2C                         I2C1
#define M24LR04E_I2C_CLK                     CLK_Peripheral_I2C1
#define M24LR04E_I2C_SCL_PIN                 GPIO_Pin_1                  /* PC.01 */
#define M24LR04E_I2C_SCL_GPIO_PORT           GPIOC                       /* GPIOC */
#define M24LR04E_I2C_SDA_PIN                 GPIO_Pin_0                  /* PC.00 */
#define M24LR04E_I2C_SDA_GPIO_PORT           GPIOC                       /* GPIOC */

/** STTS751 I2C CONFIG */
#define STTS751_I2C                         I2C1
#define STTS751_I2C_CLK                     CLK_Peripheral_I2C1
#define STTS751_I2C_SCL_PIN                 GPIO_Pin_1                  /* PC.01 */
#define STTS751_I2C_SCL_GPIO_PORT           GPIOC                       /* GPIOC */
#define STTS751_I2C_SDA_PIN                 GPIO_Pin_0                  /* PC.00 */
#define STTS751_I2C_SDA_GPIO_PORT           GPIOC                       /* GPIOC */

#define BOARD_I2C                           I2C1
#define BOARD_I2C_CLK                       CLK_Peripheral_I2C1

/** Text message parameters */
#define PROGRAM_TEXT_LEN                    19
#define READ_TEXT_LEN                       16      
#define DISPLAY_TEXT_BREAK_LEN               3
#define MAX_TEXT_LEN                        40

#endif
