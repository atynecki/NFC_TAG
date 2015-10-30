 
#ifndef __TSENSOR_H
#define __TSENSOR_H

#include "stm8l15x.h"
#include "stm8l15x_i2c.h"
#include "lib_config.h"

#define STTS751_I2C_ADDRESS 					0x72
#define STTS751_I2C_SPEED                                       20000
#define STTS751_I2C_TIMEOUT         					(uint32_t)0x000FF

#define STTS751_STOP				 		0x0340
#define STTS751_ONESHOTMODE 					0x0F00


void TS_Init(void);
void TS_Config(uint16_t ConfigBytes);
void TS_BufferRead(uint8_t* pBuffer, uint8_t Pointer_Byte, uint8_t NumByteToRead);
void TS_ReadOneByte(uint8_t *pBuffer, uint8_t Pointer_Byte);

#endif
