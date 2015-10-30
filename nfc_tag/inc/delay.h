
#ifndef __DELAY_H
#define __DELAY_H
#include "stm8l15x_clk.h"

//------------------------------------------------------------------------------
// Function Name : delay_ms
// Description   : delay for some time in ms unit
// Input         : n_ms is how many ms of time to delay
//------------------------------------------------------------------------------
void delay_ms(u16 n_ms) ;
void delayLFO_ms (u16 n_ms);

//------------------------------------------------------------------------------
// Function Name : delay_10us
// Description   : delay for some time in 10us unit(partial accurate)
// Input         : n_10us is how many 10us of time to delay
//------------------------------------------------------------------------------
void delay_10us(u16 n_10us);

#endif