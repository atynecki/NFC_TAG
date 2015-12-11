
#include "STTS751_driver.h"
	
/**
  * @brief  Initializes the STTS751 thermometer
  * @param  None
  * @retval None
*/
void TS_Init () {

  CLK_PeripheralClockConfig(STTS751_I2C_CLK, ENABLE);

  /* I2C DeInit */
  I2C_DeInit(STTS751_I2C);

  /* I2C configuration */
  I2C_Init(STTS751_I2C, STTS751_I2C_SPEED, 0x00, I2C_Mode_I2C,
            I2C_DutyCycle_2, I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);

  /* I2C Init */
  I2C_Cmd(STTS751_I2C, ENABLE);
}

/**
  * @brief      Writes two bytes to the STTS751. The first is the EEPROM address and the seond the data
  * @param[in]  ConfigBytes unit 16 containing the data to be written to the STTS751 for configure it
  * @retval     None
  */
void TS_Config (uint16_t ConfigBytes ) {
  int32_t I2C_TimeOut = STTS751_I2C_TIMEOUT;
	
  I2C_AcknowledgeConfig(STTS751_I2C, ENABLE);	
  
  /* Send START condition */
  I2C_GenerateSTART(STTS751_I2C, ENABLE);
	
  /* Test on EV5 and clear it I2C_EVENT_MASTER_MODE_SELECT */
  while( !I2C_CheckEvent(STTS751_I2C,I2C_EVENT_MASTER_MODE_SELECT)&& I2C_TimeOut-->0); 
	
  /* Send STLM75 slave address for write */
  I2C_Send7bitAddress(STTS751_I2C, STTS751_I2C_ADDRESS, I2C_Direction_Transmitter);
	
  /* Test on EV6 and clear it */
  while( !I2C_CheckEvent(STTS751_I2C,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)&& I2C_TimeOut-->0);
	
  I2C_GetFlagStatus(STTS751_I2C,I2C_FLAG_ADDR);
  (void)(STTS751_I2C->SR3);
			
  /* Send Address (on 2 bytes) of first byte to be written & wait event detection */
  I2C_SendData(STTS751_I2C,(uint8_t)((ConfigBytes & 0xFF00) >> 8)); /* MSB */
	
  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(STTS751_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING)&& I2C_TimeOut-->0);
	
  /* Send Data */
  I2C_SendData(STTS751_I2C,(uint8_t)((ConfigBytes & 0x00FF))); /* LSB */
	
  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(STTS751_I2C,I2C_EVENT_MASTER_BYTE_TRANSMITTING)&& I2C_TimeOut-->0);
	
  I2C_GenerateSTOP(STTS751_I2C,ENABLE);
}

/**
  * @brief Reads a block of data from the EEPROM
  * @param[in] pBuffer pointer to the buffer that receives the data read from the EEPROM
  * @param[in] WriteAddr EEPROM's internal address to read from
  * @param[in] NumByteToWrite EEPROM's number of bytes to read from the EEPROM  
  * @retval None
  */
void TS_ReadOneByte(uint8_t *pBuffer, uint8_t Pointer_Byte)
{  
  int32_t I2C_TimeOut = STTS751_I2C_TIMEOUT;;

  I2C_AcknowledgeConfig(STTS751_I2C, ENABLE);
  
  /* Send START condition */
  I2C_GenerateSTART(STTS751_I2C, ENABLE);
	
  /* Test on EV5 and clear it I2C_EVENT_MASTER_MODE_SELECT */ 
  while (!I2C_CheckEvent(STTS751_I2C, I2C_EVENT_MASTER_MODE_SELECT)&& I2C_TimeOut-->0);
	  
  /* Send slave Address in write direction & wait detection event */
  I2C_Send7bitAddress(STTS751_I2C, STTS751_I2C_ADDRESS, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  while ( !I2C_CheckEvent(STTS751_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)&& I2C_TimeOut-->0);
  
  I2C_GetFlagStatus(STTS751_I2C, I2C_FLAG_ADDR);
  (void)(STTS751_I2C->SR3);
    
  I2C_SendData(STTS751_I2C, (Pointer_Byte));
  
  /* Test on EV8 and clear it */
  while (!I2C_CheckEvent(STTS751_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)&& I2C_TimeOut-->0);
	
  /* Send START condition a second time */  
  I2C_GenerateSTART(STTS751_I2C, ENABLE);
	
  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(STTS751_I2C, I2C_EVENT_MASTER_MODE_SELECT)&& I2C_TimeOut-->0);
	
  /* Send slave Address in read direction & wait event */
  I2C_Send7bitAddress(STTS751_I2C, STTS751_I2C_ADDRESS, I2C_Direction_Receiver);

  // Test on EV6 and clear it 
  while (!I2C_CheckEvent(STTS751_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)&& I2C_TimeOut-->0);

  // Test on EV7 and clear it 
  while (!I2C_CheckEvent(STTS751_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)&& I2C_TimeOut-->0);

  // Received data 
  *pBuffer = I2C_ReceiveData(STTS751_I2C);

  // Disable acknowledgement 
  I2C_AcknowledgeConfig(STTS751_I2C, DISABLE);

  // Send STOP Condition 
  I2C_GenerateSTOP(STTS751_I2C, ENABLE);

  // Test on RXNE flag 
  while (I2C_GetFlagStatus(STTS751_I2C, I2C_FLAG_RXNE) == RESET && I2C_TimeOut-->0);
}
