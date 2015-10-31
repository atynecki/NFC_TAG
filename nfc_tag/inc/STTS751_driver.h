 
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

#define STTS751_TEMP_H_REG                                      0x00
#define STTS751_STATUS_REG                                      0x01
#define STTS751_TEMP_L_REG                                      0x02
#define STTS751_CONFIG_REG                                      0x03
#define STTS751_CONV_RATE_REG                                   0x04
#define STTS751_TEMP_HIGH_LIMIT_H_REG                           0x05
#define STTS751_TEMP_HIGH_LIMIT_L_REG                           0x06
#define STTS751_TEMP_LOW_LIMIT_H_REG                            0x07
#define STTS751_TEMP_LOW_LIMIT_L_REG                            0x08
#define STTS751_ONE_SHOT_REG                                    0x0F
#define STTS751_ID_REG                                          0xFE

void TS_Init(void);
void TS_Config(uint16_t ConfigBytes);
void TS_BufferRead(uint8_t* pBuffer, uint8_t Pointer_Byte, uint8_t NumByteToRead);
void TS_ReadOneByte(uint8_t *pBuffer, uint8_t Pointer_Byte);

#endif
