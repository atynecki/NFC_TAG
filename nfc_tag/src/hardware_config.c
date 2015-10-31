
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
  
void clock_all_deinit ()
{
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_USART1, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_BEEP, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_DAC, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_RTC, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_LCD, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_BOOTROM, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_AES, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM5, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_SPI2, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_USART2, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_USART3, DISABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_CSSLSE, DISABLE);
}

void GPIO_init () 
{ 
  
}

void GPIO_all_deinit ()
{
    //GPIO_Mode_In_PU_No_IT
    GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
    GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
    GPIO_Init( GPIOC, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |\
               GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7, GPIO_Mode_Out_OD_Low_Fast);
    GPIO_Init( GPIOD, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
    GPIO_Init( GPIOE, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
    // Set all the GPIO state to 1
    GPIOA->ODR = 0xFF;
    GPIOB->ODR = 0xFF;
    GPIOC->ODR = 0xFF;
    GPIOD->ODR = 0xFF;
    GPIOE->ODR = 0xFF;
}