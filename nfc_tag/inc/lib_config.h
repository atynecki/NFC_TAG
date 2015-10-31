
#ifndef __LIB_CONFIG_H
#define __LIB_CONFIG_H

/* MACROs for SET, RESET or TOGGLE Output port */

#define GPIO_HIGH(a,b) 		a->ODR|=b
#define GPIO_LOW(a,b)		a->ODR&=~b
#define GPIO_TOGGLE(a,b) 	a->ODR^=b

#define LED_GPIO_PORT		GPIOE
#define LED_GPIO_PIN		GPIO_Pin_6

#define BUTTON_GPIO_PORT	GPIOC
#define BUTTON_GPIO_PIN		GPIO_Pin_7

/*
#define CTN_GPIO_PORT           GPIOC
#define CTN_CNTEN_GPIO_PIN      GPIO_Pin_4
#define WAKEUP_GPIO_PORT        GPIOE
#define ICC_WAKEUP_GPIO_PIN     GPIO_Pin_6
#define ICC_WAKEUP_EXTI_PIN     EXTI_Pin_3
*/

/**
  * @brief  M24LR16E-R I2C Interface pins
  */
#define M24LR04E_I2C                         I2C1
#define M24LR04E_I2C_CLK                     CLK_Peripheral_I2C1
#define M24LR04E_I2C_SCL_PIN                 GPIO_Pin_1                  /* PC.01 */
#define M24LR04E_I2C_SCL_GPIO_PORT           GPIOC                       /* GPIOC */
#define M24LR04E_I2C_SDA_PIN                 GPIO_Pin_0                  /* PC.00 */
#define M24LR04E_I2C_SDA_GPIO_PORT           GPIOC                       /* GPIOC */

/**
  * @brief  temperature sensor STTS751 I2C Interface pins
  */
#define STTS751_I2C                         I2C1
#define STTS751_I2C_CLK                     CLK_Peripheral_I2C1
#define STTS751_I2C_SCL_PIN                 GPIO_Pin_1                  /* PC.01 */
#define STTS751_I2C_SCL_GPIO_PORT           GPIOC                       /* GPIOC */
#define STTS751_I2C_SDA_PIN                 GPIO_Pin_0                  /* PC.00 */
#define STTS751_I2C_SDA_GPIO_PORT           GPIOC                       /* GPIOC */

#endif
