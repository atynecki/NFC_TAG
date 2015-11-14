
#ifndef __M24LR04_H
#define __M24LR04_H

#include "lib_config.h"

#define M24LR04E_I2C_ADDRESS    	 0xA6
#define M24LR04E_I2C_SPEED               20000
#define M24LR04E_I2C_TIMEOUT             (uint32_t)0x000FF

#define M24LR04E_CONFIG1_ADDRESS         0x0000
#define M24LR04E_MESSAGE_LEN_ADDRESS     0x0008
#define M24LR04E_CONFIG2_ADDRESS         0x0009
#define M24LR04E_TEXT_ADDRESS            0x000C  

#define M24LR04E_PAGE_SIZE                    4


void M24LR04E_Init(void);
void M24LR04E_DeInit(void);

void M24LR04E_ReadOneByte(uint16_t ReadAddr,uint8_t* pBuffer);
void M24LR04E_ReadBuffer(uint16_t ReadAddr, uint8_t NumByteToRead,uint8_t* pBuffer);
void M24LR04E_WriteOneByte(uint16_t WriteAddr,uint8_t pBuffer);
void M24LR04E_WritePage(uint16_t WriteAddr,uint8_t* pBuffer);

#endif


