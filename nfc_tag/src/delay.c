
#include "delay.h"

void delay_ms(u16 n_ms)
{
  /* Init TIMER 4 */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

  /* Init TIMER 4 prescaler: / (2^6) = /64 */
  TIM4->PSCR = 6;

  /* HSI div by 1 --> Auto-Reload value: 16M / 64 = 1/4M, 1/4M / 1k = 250*/
  TIM4->ARR = 250;
  
  /* Counter value: 2, to compensate the initialization of TIMER*/
  TIM4->CNTR = 2;

  /* clear update flag */
  TIM4->SR1 &= ~TIM4_SR1_UIF;

  /* Enable Counter */
  TIM4->CR1 |= TIM4_CR1_CEN;

  while(n_ms--)
  {
    while((TIM4->SR1 & TIM4_SR1_UIF) == 0) ;
    TIM4->SR1 &= ~TIM4_SR1_UIF;
  }

  /* Disable Counter */
  TIM4->CR1 &= ~TIM4_CR1_CEN;
}

void delay_10us(u16 n_10us)
{
  /* Init TIMER 4 */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);

  /* prescaler: / (2^0) = /1 */
  TIM4->PSCR = 0;

  /* SYS_CLK_HSI_DIV1 Auto-Reload value: 16M / 1 = 16M, 16M / 100k = 160 */
  TIM4->ARR = 160;

  /* Counter value: 10, to compensate the initialization of TIMER */
  TIM4->CNTR = 10;

  /* clear update flag */
  TIM4->SR1 &= ~TIM4_SR1_UIF;

  /* Enable Counter */
  TIM4->CR1 |= TIM4_CR1_CEN;

  while(n_10us--)
  {
    while((TIM4->SR1 & TIM4_SR1_UIF) == 0) ;
    TIM4->SR1 &= ~TIM4_SR1_UIF;
  }

  /* Disable Counter */
  TIM4->CR1 &= ~TIM4_CR1_CEN;
 CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);

}

void delayLFO_ms (u16 n_ms)
{
    //Switch the clock to LSE and disable HSI
    #ifdef USE_LSE
        CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
        CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSE);	
        CLK_SYSCLKSourceSwitchCmd(ENABLE);
        while (((CLK->SWCR)& 0x01)==0x01);
        CLK_HSICmd(DISABLE);
        CLK->ECKCR &= ~0x01; 
    #else
        CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
        CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);
        CLK_SYSCLKSourceSwitchCmd(ENABLE);
        while (((CLK->SWCR)& 0x01)==0x01);
        CLK_HSICmd(DISABLE);
        CLK->ECKCR &= ~0x01; 
    #endif	

    delay_ms(n_ms);

    #ifdef USE_HSI
        //Switch the clock to HSI
        CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_16);
        CLK_HSICmd(ENABLE);
        while (((CLK->ICKCR)& 0x02)!=0x02);			
        CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
        CLK_SYSCLKSourceSwitchCmd(ENABLE);
        while (((CLK->SWCR)& 0x01)==0x01);
    #else
        CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_2);
        // Select 2MHz HSE as system clock source 
        CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
        // wait until the target clock source is ready 
        while (((CLK->SWCR)& 0x01)==0x01);
        // wait until the target clock source is ready 
        CLK_SYSCLKSourceSwitchCmd(ENABLE);
    #endif
}
