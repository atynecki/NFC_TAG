
#include "M24LR04E-R_driver.h"

/**
  * @brief  Initializes the M24LR04E
  * @param  None
  * @retval None
  */
void M24LR04E_Init () {

  CLK_PeripheralClockConfig(M24LR04E_I2C_CLK, ENABLE);

  /* I2C DeInit */
  I2C_DeInit(M24LR04E_I2C);

  /* I2C configuration */
  I2C_Init(M24LR04E_I2C, M24LR04E_I2C_SPEED, 0x00, I2C_Mode_I2C,
           I2C_DutyCycle_2, I2C_Ack_Enable, I2C_AcknowledgedAddress_7bit);

  /* I2C Init */
  I2C_Cmd(M24LR04E_I2C, ENABLE);
}

/**
  * @brief  DeInitializes the M24LR04E_I2C
  * @param  None
  * @retval None
  */
void M24LR04E_DeInit () {
  /* Disable M24LR04E_I2C */
  I2C_Cmd(M24LR04E_I2C, DISABLE);
  /* DeInitializes the M24LR04E_I2C */
  I2C_DeInit(M24LR04E_I2C);

  /* M24LR04E_I2C Periph clock disable */
  CLK_PeripheralClockConfig(M24LR04E_I2C_CLK, DISABLE);

  /* Configure M24LR04E_I2C pins: SCL */
  GPIO_Init(M24LR04E_I2C_SCL_GPIO_PORT, M24LR04E_I2C_SCL_PIN, GPIO_Mode_In_PU_No_IT);

  /* Configure M24LR04E_I2C pins: SDA */
  GPIO_Init(M24LR04E_I2C_SDA_GPIO_PORT, M24LR04E_I2C_SDA_PIN, GPIO_Mode_In_PU_No_IT);
}

/**
  * @brief  Checks the M24LR16E status
  * @param  None
  * @retval ErrorStatus: M24LR16E Status (ERROR or SUCCESS).
  */
ErrorStatus M24LR04E_GetStatus(void)
{
  uint32_t I2C_TimeOut = M24LR04E_I2C_TIMEOUT;

  /* Clear the M24LR04E_I2C AF flag */
  I2C_ClearFlag(M24LR04E_I2C, I2C_FLAG_AF);

  /* Enable M24LR04E_I2C acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(M24LR04E_I2C, ENABLE);

  /* Send M24LR04E_I2C START condition */
  I2C_GenerateSTART(M24LR04E_I2C, ENABLE);

  /* Test on M24LR04E_I2C EV5 and clear it */
  while ((!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_MODE_SELECT)) && I2C_TimeOut) {
    I2C_TimeOut--;
  }
  if (I2C_TimeOut == 0) {
    return ERROR;
  }
  I2C_TimeOut = M24LR04E_I2C_TIMEOUT;

  /* Send M24LR04E-R slave address for write */
  I2C_Send7bitAddress(M24LR04E_I2C, M24LR04E_I2C_ADDRESS, I2C_Direction_Transmitter);

  while ((!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && I2C_TimeOut) {
    I2C_TimeOut--;
  }

  if ((I2C_GetFlagStatus(M24LR04E_I2C, I2C_FLAG_AF) != 0x00) || (I2C_TimeOut == 0)){
    return ERROR;
  }
  else {
    return SUCCESS;
  }
}

/**
  * @brief this function reads one byte of data from the M24LR16E EEPROM
  * @param[in] pBuffer pointer to the buffer that receives the data read from the EEPROM
  * @param[in] WriteAddr EEPROM's internal address to read from
  * @param[in] NumByteToWrite EEPROM's number of bytes to read from the EEPROM  
  * @retval None
  */
void M24LR04E_ReadOneByte (uint8_t EE_address, uint16_t ReadAddr,uint8_t* pBuffer) {
  int32_t I2C_TimeOut = M24LR04E_I2C_TIMEOUT;

  // Enable M24LR04E_I2C acknowledgement if it is already disabled by other function 
  I2C_AcknowledgeConfig(M24LR04E_I2C, ENABLE);

  // Send M24LR04E_I2C START condition 
  I2C_GenerateSTART(M24LR04E_I2C, ENABLE);

  // Test on M24LR04E_I2C EV5 and clear it 
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_MODE_SELECT) && I2C_TimeOut-->0);

  // Send M24LR04E-R slave address for write 
  I2C_Send7bitAddress(M24LR04E_I2C, EE_address , I2C_Direction_Transmitter);

  // Test on EV6 and clear it 
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)&& I2C_TimeOut-->0);

  // Send Address of first byte to be read & wait event detection 
  I2C_SendData(M24LR04E_I2C,(uint8_t)(ReadAddr >> 8));

  // Test on M24LR04E_I2C EV8 and clear it 
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)&& I2C_TimeOut-->0);
	
  // Send Address of first byte to be read & wait event detection 
  I2C_SendData(M24LR04E_I2C,(uint8_t)ReadAddr);
	
  // Test on M24LR04E_I2C EV8 and clear it 
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)&& I2C_TimeOut-->0);

  // Send Re-START condition 
  I2C_GenerateSTART(M24LR04E_I2C, ENABLE);

  // Test on EV5 and clear it 
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_MODE_SELECT)&& I2C_TimeOut-->0);

  // Send STLM75 slave address for read 
  I2C_Send7bitAddress(M24LR04E_I2C, EE_address, I2C_Direction_Receiver);

  // Test on EV6 and clear it 
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)&& I2C_TimeOut-->0);

  // Test on EV7 and clear it 
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)&& I2C_TimeOut-->0);

  // Store M24LR04E_I2C received data 
  *pBuffer = I2C_ReceiveData(M24LR04E_I2C);

  // Disable M24LR04E_I2C acknowledgement 
  I2C_AcknowledgeConfig(M24LR04E_I2C, DISABLE);

  // Send M24LR04E_I2C STOP Condition 
  I2C_GenerateSTOP(M24LR04E_I2C, ENABLE);

  // Test on RXNE flag 
  while (I2C_GetFlagStatus(M24LR04E_I2C, I2C_FLAG_RXNE) == RESET && I2C_TimeOut-->0);
}

/**
  * @brief this function reads a block of data from the M24LR16E EEPROM
  * @param[in] pBuffer pointer to the buffer that receives the data read from the EEPROM
  * @param[in] WriteAddr EEPROM's internal address to read from
  * @param[in] NumByteToWrite EEPROM's number of bytes to read from the EEPROM
  * @retval None
  */
void M24LR04E_ReadBuffer (uint8_t EE_address, uint16_t ReadAddr, uint8_t NumByteToRead,uint8_t* pBuffer) {
  int32_t I2C_TimeOut = M24LR04E_I2C_TIMEOUT;

  // Enable M24LR04E_I2C acknowledgement if it is already disabled by other function 
  I2C_AcknowledgeConfig(M24LR04E_I2C, ENABLE);
  
  // Send M24LR04E_I2C START condition 
  I2C_GenerateSTART(M24LR04E_I2C, ENABLE);

  // Test on M24LR04E_I2C EV5 and clear it 
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_MODE_SELECT)&& I2C_TimeOut-->0);

  // Send STLM75 slave address for write 
  I2C_Send7bitAddress(M24LR04E_I2C, EE_address , I2C_Direction_Transmitter);

  // Test on M24LR04E_I2C EV6 and clear it 
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)&& I2C_TimeOut-->0);

  // Send Address of first byte to be read & wait event detection 
  I2C_SendData(M24LR04E_I2C,(uint8_t)(ReadAddr >> 8));
	
  // Test on M24LR04E_I2C EV8 and clear it 
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)&& I2C_TimeOut-->0);
	
  // Send Address of first byte to be read & wait event detection 
  I2C_SendData(M24LR04E_I2C,(uint8_t)ReadAddr); 
	
  // Test on M24LR04E_I2C EV8 and clear it 
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)&& I2C_TimeOut-->0);

  // Send Re-STRAT condition 
  I2C_GenerateSTART(M24LR04E_I2C, ENABLE);

  // Test on EV5 and clear it 
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_MODE_SELECT)&& I2C_TimeOut-->0);

  // Send M24LR16E slave address for read 
  I2C_Send7bitAddress(M24LR04E_I2C, EE_address, I2C_Direction_Receiver);

  // Test on EV6 and clear it 
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)&& I2C_TimeOut-->0);

  // While there is data to be read 
  while(NumByteToRead ) {
    // Test on EV7 and clear it 
    while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)&& I2C_TimeOut-->0);
    
    // Store M24LR04E_I2C received data 
    *pBuffer = I2C_ReceiveData(M24LR04E_I2C);
    
    if(NumByteToRead == 1) {
        // Disable M24LR04E_I2C acknowledgement 
        I2C_AcknowledgeConfig(M24LR04E_I2C, DISABLE);
    }
		
    // Point to the next location where the byte read will be saved 
    pBuffer++; 
    // Decrement the read bytes counter 
    NumByteToRead--; 
		
    if(NumByteToRead == 0) {
        // Send M24LR04E_I2C STOP Condition 
        I2C_GenerateSTOP(M24LR04E_I2C, ENABLE);
    }	
  }
  
  // Store M24LR04E_I2C received data 
  *pBuffer = I2C_ReceiveData(M24LR04E_I2C);
  // Test on RXNE flag 
  while (I2C_GetFlagStatus(M24LR04E_I2C, I2C_FLAG_RXNE) == RESET&& I2C_TimeOut-->0);
}

/**
  * @brief  Write to the configuration register of the M24LR16E
  * @param  RegValue: sepecifies the value to be written to M24LR16E configuration register
  * @retval None
  */
void M24LR04E_WriteOneByte (uint8_t EE_address, uint16_t ReadAddr,uint8_t pBuffer) {
  int32_t I2C_TimeOut = M24LR04E_I2C_TIMEOUT;
  
  /* Send M24LR04E_I2C START condition */
  I2C_GenerateSTART(M24LR04E_I2C, ENABLE);

  /* Test on M24LR04E_I2C EV5 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_MODE_SELECT)&& I2C_TimeOut-->0);

  /* Send STLM75 slave address for write */
  I2C_Send7bitAddress(M24LR04E_I2C, EE_address, I2C_Direction_Transmitter);

  /* Test on M24LR04E_I2C EV6 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)&& I2C_TimeOut-->0);
	
  // Send Address of first byte to be read & wait event detection 
  I2C_SendData(M24LR04E_I2C,(uint8_t)(ReadAddr >> 8));

  // Test on M24LR04E_I2C EV8 and clear it 
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)&& I2C_TimeOut-->0);

  // Send Address of first byte to be read & wait event detection 
  I2C_SendData(M24LR04E_I2C,(uint8_t)ReadAddr);
  
  // Test on M24LR04E_I2C EV8 and clear it 
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)&& I2C_TimeOut-->0);
	
  /* Send the configuration register data pointer */
  I2C_SendData(M24LR04E_I2C, pBuffer);

  /* Test on M24LR04E_I2C EV8 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)&& I2C_TimeOut-->0);

  /* Send M24LR04E_I2C STOP Condition */
  I2C_GenerateSTOP(M24LR04E_I2C, ENABLE);
}








/**
  * @brief  Read the specified register from the M24LR16E.
  * @param  RegName: specifies the M24LR16E register to be read.
  *              This member can be one of the following values:
  *                  - M24LR04E_REG_TEMP: temperature register
  *                  - M24LR04E_REG_TOS: Over-limit temperature register
  *                  - M24LR04E_REG_THYS: Hysteresis temperature register
  * @retval M24LR16E register value.
  */
uint16_t M24LR04E_ReadReg(uint8_t RegName)
{
  __IO uint16_t RegValue = 0;

  /* Enable M24LR04E_I2C acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(M24LR04E_I2C, ENABLE);

  /*--------------------------- Transmission Phase ----------------------------*/
  /* Send M24LR04E_I2C START condition */
  I2C_GenerateSTART(M24LR04E_I2C, ENABLE);

  /* Test on M24LR04E_I2C EV5 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }

  /* Send STLM75 slave address for write */
  I2C_Send7bitAddress(M24LR04E_I2C, M24LR04E_I2C_ADDRESS, I2C_Direction_Transmitter);

  /* Test on M24LR04E_I2C EV6 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* EV6 */
  {
  }

  /* Send the specified register data pointer */
  I2C_SendData(M24LR04E_I2C, RegName);

  /* Test on M24LR04E_I2C EV8 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /*------------------------------ Reception Phase ----------------------------*/
  /* Send Re-STRAT condition */
  I2C_GenerateSTART(M24LR04E_I2C, ENABLE);

  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }

  /* Send STLM75 slave address for read */
  I2C_Send7bitAddress(M24LR04E_I2C, M24LR04E_I2C_ADDRESS, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))  /* EV6 */
  {
  }

  /* Test on EV7 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))  /* EV7 */
  {
  }

  /* Store M24LR04E_I2C received data */
  RegValue = (uint16_t)(I2C_ReceiveData(M24LR04E_I2C) << 8);

  /* Disable M24LR04E_I2C acknowledgement */
  I2C_AcknowledgeConfig(M24LR04E_I2C, DISABLE);

  /* Send M24LR04E_I2C STOP Condition */
  I2C_GenerateSTOP(M24LR04E_I2C, ENABLE);

  /* Test on RXNE flag */
  while (I2C_GetFlagStatus(M24LR04E_I2C, I2C_FLAG_RXNE) == RESET)
  {}

  /* Store M24LR04E_I2C received data */
  RegValue |= I2C_ReceiveData(M24LR04E_I2C);

  /* Return register value */
  return (RegValue);
}

/**
  * @brief  Write to the specified register of the M24LR16E.
  * @param  RegName: specifies the M24LR16E register to be written.
  *              This member can be one of the following values:
  *                  - M24LR04E_REG_TOS: Over-limit temperature register
  *                  - M24LR04E_REG_THYS: Hysteresis temperature register
  * @param  RegValue: value to be written to M24LR16E register.
  * @retval None
  */
void M24LR04E_WriteReg(uint8_t RegName, uint16_t RegValue)
{
  /*-------------------------------- Transmission Phase -----------------------*/
  /* Send M24LR04E_I2C START condition */
  I2C_GenerateSTART(M24LR04E_I2C, ENABLE);

  /* Test on M24LR04E_I2C EV5 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }

  /* Send STLM75 slave address for write */
  I2C_Send7bitAddress(M24LR04E_I2C, M24LR04E_I2C_ADDRESS, I2C_Direction_Transmitter);

  /* Test on M24LR04E_I2C EV6 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* EV6 */
  {
  }

  /* Send the specified register data pointer */
  I2C_SendData(M24LR04E_I2C, RegName);

  /* Test on M24LR04E_I2C EV8 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /* Send M24LR04E_I2C data */
  I2C_SendData(M24LR04E_I2C, (uint8_t)(RegValue >> 8));

  /* Test on M24LR04E_I2C EV8 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /* Send M24LR04E_I2C data */
  I2C_SendData(M24LR04E_I2C, (uint8_t)RegValue);

  /* Test on M24LR04E_I2C EV8 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /* Send M24LR04E_I2C STOP Condition */
  I2C_GenerateSTOP(M24LR04E_I2C, ENABLE);
}





/**
  * @brief  Read Temperature register of M24LR16E: double temperature value.
  * @param  None
  * @retval M24LR16E measured temperature value.
  */
uint16_t M24LR04E_ReadTemp(void)
{
  __IO uint16_t RegValue = 0;

  /* Enable M24LR04E_I2C acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(M24LR04E_I2C, ENABLE);

  /*------------------------------------- Transmission Phase ------------------*/
  /* Send M24LR04E_I2C START condition */
  I2C_GenerateSTART(M24LR04E_I2C, ENABLE);

  /* Test on M24LR04E_I2C EV5 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }

  /* Send STLM75 slave address for write */
  I2C_Send7bitAddress(M24LR04E_I2C, M24LR04E_I2C_ADDRESS, I2C_Direction_Transmitter);

  /* Test on M24LR04E_I2C EV6 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* EV6 */
  {
  }

  /* Send the temperature register data pointer */
  I2C_SendData(M24LR04E_I2C, M24LR04E_REG_TEMP);

  /* Test on M24LR04E_I2C EV8 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /*-------------------------------- Reception Phase --------------------------*/
  /* Send Re-STRAT condition */
  I2C_GenerateSTART(M24LR04E_I2C, ENABLE);

  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }

  /* Send STLM75 slave address for read */
  I2C_Send7bitAddress(M24LR04E_I2C, M24LR04E_I2C_ADDRESS, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))  /* EV6 */
  {
  }

  /* Test on EV7 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))  /* EV7 */
  {
  }

  /* Store M24LR04E_I2C received data */
  RegValue = I2C_ReceiveData(M24LR04E_I2C) << 8;

  /* Disable M24LR04E_I2C acknowledgement */
  I2C_AcknowledgeConfig(M24LR04E_I2C, DISABLE);

  /* Send M24LR04E_I2C STOP Condition */
  I2C_GenerateSTOP(M24LR04E_I2C, ENABLE);

  /* Test on RXNE flag */
  while (I2C_GetFlagStatus(M24LR04E_I2C, I2C_FLAG_RXNE) == RESET)
  {}

  /* Store M24LR04E_I2C received data */
  RegValue |= I2C_ReceiveData(M24LR04E_I2C);

  /* Return Temperature value */
  return (RegValue >> 7);
}

/**
  * @brief  Read the configuration register from the M24LR16E.
  * @param  None
  * @retval M24LR16E configuration register value.
  */
uint8_t M24LR04E_ReadConfReg(void)
{
  __IO uint8_t RegValue = 0;

  /* Enable M24LR04E_I2C acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(M24LR04E_I2C, ENABLE);
  /*----------------------------- Transmission Phase --------------------------*/
  /* Send M24LR04E_I2C START condition */
  I2C_GenerateSTART(M24LR04E_I2C, ENABLE);

  /* Test on M24LR04E_I2C EV5 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }

  /* Send STLM75 slave address for write */
  I2C_Send7bitAddress(M24LR04E_I2C, M24LR04E_I2C_ADDRESS, I2C_Direction_Transmitter);

  /* Test on M24LR04E_I2C EV6 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* EV6 */
  {
  }

  /* Send the configuration register data pointer */
  I2C_SendData(M24LR04E_I2C, M24LR04E_REG_CONF);

  /* Test on M24LR04E_I2C EV8 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /*----------------------------- Reception Phase -----------------------------*/
  /* Send Re-STRAT condition */
  I2C_GenerateSTART(M24LR04E_I2C, ENABLE);

  /* Test on EV5 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }

  /* Send STLM75 slave address for read */
  I2C_Send7bitAddress(M24LR04E_I2C, M24LR04E_I2C_ADDRESS, I2C_Direction_Receiver);

  /* Test on EV6 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))  /* EV6 */
  {
  }

  /* Disable M24LR04E_I2C acknowledgement */
  I2C_AcknowledgeConfig(M24LR04E_I2C, DISABLE);

  /* Send M24LR04E_I2C STOP Condition */
  I2C_GenerateSTOP(M24LR04E_I2C, ENABLE);

  /* Test on RXNE flag */
  while (I2C_GetFlagStatus(M24LR04E_I2C, I2C_FLAG_RXNE) == RESET);

  /* Store M24LR04E_I2C received data */
  RegValue = I2C_ReceiveData(M24LR04E_I2C);

  /* Return configuration register value */
  return (RegValue);
}

/**
  * @brief  Write to the configuration register of the M24LR16E.
  * @param  RegValue: sepecifies the value to be written to M24LR16E configuration
  *         register.
  * @retval None
  */
void M24LR04E_WriteConfReg(uint8_t RegValue)
{
  /*-------------------------------- Transmission Phase -----------------------*/
  /* Send M24LR04E_I2C START condition */
  I2C_GenerateSTART(M24LR04E_I2C, ENABLE);

  /* Test on M24LR04E_I2C EV5 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_MODE_SELECT))  /* EV5 */
  {
  }

  /* Send STLM75 slave address for write */
  I2C_Send7bitAddress(M24LR04E_I2C, M24LR04E_I2C_ADDRESS, I2C_Direction_Transmitter);

  /* Test on M24LR04E_I2C EV6 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) /* EV6 */
  {
  }

  /* Send the configuration register data pointer */
  I2C_SendData(M24LR04E_I2C, M24LR04E_REG_CONF);

  /* Test on M24LR04E_I2C EV8 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /* Send M24LR04E_I2C data */
  I2C_SendData(M24LR04E_I2C, RegValue);

  /* Test on M24LR04E_I2C EV8 and clear it */
  while (!I2C_CheckEvent(M24LR04E_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) /* EV8 */
  {
  }

  /* Send M24LR04E_I2C STOP Condition */
  I2C_GenerateSTOP(M24LR04E_I2C, ENABLE);
}


