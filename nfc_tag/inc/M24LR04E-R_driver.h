
#ifndef __M24LR04_H
#define __M24LR04_H

#include "stm8l15x.h"
#include "stm8l15x_i2c.h"
#include "lib_config.h"

#define M24LR04E_I2C_ADDRESS            0x90
#define M24LR04E_I2C_SPEED              20000
#define M24LR04E_I2C_TIMEOUT            (uint32_t)0x000FF

#define M24LR04E_REG_TEMP        0x00  /* Temperature Register of LM75 */
#define M24LR04E_REG_CONF        0x01  /* Configuration Register of LM75 */
#define M24LR04E_REG_THYS        0x02  /* Temperature Register of LM75 */
#define M24LR04E_REG_TOS         0x03  /* Over-temp Shutdown threshold Register of LM75 */

void M24LR04E_Init(void);
void M24LR04E_DeInit(void);
ErrorStatus M24LR04E_GetStatus(void);

void M24LR04E_ReadOneByte(uint8_t EE_address, uint16_t ReadAddr,uint8_t* pBuffer);
void M24LR04E_ReadBuffer(uint8_t EE_address, uint16_t ReadAddr, uint8_t NumByteToRead,uint8_t* pBuffer);
void M24LR04E_WriteOneByte(uint8_t EE_address, uint16_t WriteAddr,uint8_t pBuffer);

uint16_t M24LR04E_ReadTemp(void);
uint16_t M24LR04E_ReadReg(uint8_t RegName);
void M24LR04E_WriteReg(uint8_t RegName, uint16_t RegValue);
uint8_t M24LR04E_ReadConfReg(void);
void M24LR04E_WriteConfReg(uint8_t RegValue);
void M24LR04E_ShutDown(FunctionalState NewState);  

#endif


