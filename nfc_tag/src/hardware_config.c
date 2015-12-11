
#include "hardware_config.h"

void clock_init () 
{
  /* Select HSI as system clock source */
  #ifdef USE_HSI
          CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
          CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_16);	
   #else
          CLK_SYSCLKSourceSwitchCmd(ENABLE);
          /* Select 2MHz HSE as system clock source */
          CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
          CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_4);	
          CLK_HSICmd(DISABLE);
  #endif
}

void GPIO_init () 
{ 
  /* LED init */
  GPIO_Init(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_Mode_Out_PP_Low_Fast);
  // set to 0 
  GPIO_WriteBit(LED_GPIO_PORT, LED_GPIO_PIN, RESET);
	
  /* USER button init */
  GPIO_Init(BUTTON_GPIO_PORT, BUTTON_GPIO_PIN, GPIO_Mode_In_FL_IT);
  EXTI_SetPinSensitivity(EXTI_Pin_7, EXTI_Trigger_Falling);
}

void GPIO_all_deinit ()
{
    GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
    GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
    GPIO_Init( GPIOC, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |\
               GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7, GPIO_Mode_Out_OD_Low_Fast);
    GPIO_Init( GPIOD, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
    GPIO_Init( GPIOE, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
    GPIOA->ODR = 0xFF;
    GPIOB->ODR = 0xFF;
    GPIOC->ODR = 0xFF;
    GPIOD->ODR = 0xFF;
    GPIOE->ODR = 0xFF;
}

void LED_set ()
{
   GPIO_WriteBit(LED_GPIO_PORT, LED_GPIO_PIN, SET);
}

void LED_reset ()
{
   GPIO_WriteBit(LED_GPIO_PORT, LED_GPIO_PIN, RESET);
}

void LED_toggle ()
{
  GPIO_ToggleBits(LED_GPIO_PORT, LED_GPIO_PIN);
}