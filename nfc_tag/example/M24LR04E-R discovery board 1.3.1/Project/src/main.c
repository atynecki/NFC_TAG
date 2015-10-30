/**
  ******************************************************************************
  * @file    main.c
  * @author  MMY division
  * @version V1.3.0
  * @date    09/2012
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  */
 
/* Includes ------------------------------------------------------------------*/
#include "stm8l15x.h"
#include "stm8l_discovery_lcd.h"
#include "discover_board.h"
#include "vcc_measure.h"
#include "I2C_M24LR04E-R.h"
#include "I2C_STTS751.h"
#include "discover_functions.h"
#include "misc.h"
#include "delay.h"

// the HSI (High Speed Internal) oscillator is used 
#define USE_HSI

/* Machine status used by main for active function set by user button in interrupt handler */
uint8_t state_machine;


static ErrorStatus User_GetPayloadLength(uint8_t *PayloadLength);
static ErrorStatus User_CheckNDEFMessage(void);
static ErrorStatus User_GetNDEFMessage(uint8_t  PayloadLength,uint8_t *NDEFmessage);
static void ToUpperCase (uint8_t  NbCar ,uint8_t *StringToConvert);
static void DeInitClock ( void );
static void DeInitGPIO ( void );
static void User_DesactivateEnergyHarvesting ( void );
static void User_EnterIntoLowPowerMode(void);
static void InitializeBuffer (uint8_t *Buffer ,uint8_t NbCar);
static int8_t User_ReadNDEFMessage ( uint8_t *PayloadLength );	
static void User_DisplayMessage (uint8_t message[],uint8_t PayloadLength );
static void User_DisplayMessageActiveHaltMode ( uint8_t PayloadLength );
static void User_GetOneTemperature (uint8_t *data_sensor);
static void User_DisplayOneTemperature (uint8_t data_sensor);
static int8_t User_WriteFirmwareVersion ( void ) ; 		

/* LCD bar graph: used for display active function */
extern uint8_t t_bar[2];

uint8_t NDEFmessage[0x40];
const uint8_t ErrorMessage[]={'N','O','T',' ','A',' ','N','D','E','F',' ','T','E','X','T',' ','M','E','S','S','A','G','E',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};

volatile char FLASH_CR2     @0x5051;    
volatile char FLASH_NCR2    @0x17ff;    
volatile char FLASH_IAPSR   @0x5054;    


EEPROM uint8_t 	EEMenuState;
EEPROM uint8_t 	EEInitial = 0;
const uint8_t 	FirmwareVersion[2] = {0x13,0x00}; // 4 MSB bits major version number & 4 LSB bits minor version number

#define USE_HSI

/**
  * @brief main entry point.
  * @par Parameters None
  * @retval void None
  * @par Required preconditions: None
  */
void main(void)
{ 
uint8_t PayloadLength,
				data_sensor,
				*bufMessage;
		
	/* deinit I/O ports */
	DeInitClock();
	DeInitGPIO();
	
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

	// Initializes the LCD glass 
  LCD_GLASS_Init();

	
	/* LED button init: GPIO set in push pull */
	GPIO_Init( LED_GPIO_PORT, LED_GPIO_PIN, GPIO_Mode_Out_PP_Low_Fast);
	// set to 0 
	GPIOE->ODR &= ~LED_GPIO_PIN;
	
	/* USER button init: GPIO set in input interrupt active mode */
  GPIO_Init( BUTTON_GPIO_PORT, USER_GPIO_PIN, GPIO_Mode_In_FL_IT);
	EXTI_SetPinSensitivity(EXTI_Pin_7, EXTI_Trigger_Falling);

  //* Init Bar on LCD all are OFF
  BAR0_OFF;
  BAR1_OFF;
  BAR2_OFF;
  BAR3_OFF;	
	
	enableInterrupts();
	
  
  //* At power on VDD diplays 
	bufMessage = NDEFmessage;
	
	if (EEMenuState > STATE_TEMPMEAS) 
		EEMenuState = STATE_CHECKNDEFMESSAGE;
		
	FLASH_Unlock(FLASH_MemType_Data );
	
	state_machine = EEMenuState ; 
	
	delayLFO_ms (1);
	
	if (EEInitial == 0)
	{
			User_WriteFirmwareVersion ();
			EEInitial =1;
	}
	
  while (1)
  {
    
    switch (state_machine)
    {
			  
				case STATE_VREF:
					// measure the voltage available at the output of the M24LR04E-R
					Vref_measure();

					delayLFO_ms (2);
        break;
			
        case STATE_CHECKNDEFMESSAGE:
				
						// read the NDEF message from the M24LR04E-R EEPROM and display it if it is found 				
					if (User_ReadNDEFMessage (&PayloadLength) == SUCCESS)						
						User_DisplayMessage (bufMessage,PayloadLength);
	//					User_DisplayMessageActiveHaltMode (PayloadLength);
					else 
						User_DisplayMessage(ErrorMessage,20);		
		
					
	
        break;
				
				case STATE_TEMPMEAS:
						
						// read the ambiant tempserature from the STTS751
						User_GetOneTemperature (&data_sensor);
						// display the temperature
						User_DisplayOneTemperature (data_sensor);
			
						delayLFO_ms (2);
						
				break;
			
			break;
  
        /* for safe: normaly never reaches */ 			
        default:
					LCD_GLASS_Clear();
					LCD_GLASS_DisplayString("Error");
					state_machine = STATE_VREF;
        break;
      }
    
		
    }
}	


/**
  * @brief this functions returns a temperature measurement provided by 
	* @brief the STTS751 device.
	* @par data_sensor : temperature measurement
  * @retval none
  */
static void User_GetOneTemperature (uint8_t *data_sensor)
{
uint8_t 	Pointer_temperature = 0x00;					/* temperature access */
	
	
	data_sensor[0] = 0x00;
	/* Config the Temperature sensor in OneShotMode */
	I2C_SS_Init();
	I2C_SS_Config(STTS751_STOP);
//	delay_ms(2);
	delayLFO_ms (1);
	I2C_SS_Config(STTS751_ONESHOTMODE);
//	delay_ms(50);
	delayLFO_ms (1);
	/* read 2 bytes temperature and store them into data_sensor[] */
	I2C_SS_ReadOneByte(data_sensor,Pointer_temperature);
//	delay_ms(10);
	delayLFO_ms (1);
	
}

/**
  * @brief this functions displays a temperature measurement on the LCD screen.
	* @par data_sensor : temperature measurement
  * @retval none
  */
static void User_DisplayOneTemperature (uint8_t data_sensor)
{
uint16_t 	TempChar16[6];
	
	TempChar16[5] = 'C';
	TempChar16[4] = ' ';
	// check if the temperature is negative
	if ((data_sensor & 0x80) != 0)
	{
		data_sensor = (~data_sensor) +1;
		TempChar16[1] = '-';
	}
	else 
		TempChar16[1] = ' ';							
	
	TempChar16[3] = (data_sensor %10) + 0x30 ;
	TempChar16[2] = (data_sensor /10) + 0x30;
	TempChar16[0] = ' ';
	LCD_GLASS_DisplayStrDeci(TempChar16);		
}

/**
  * @brief this function checks if a NDEF message is available in the M24LR04E-R EEPROM
	* @par PayloadLength : the number of byte of the NDEF message
  * @retval SUCCESS : A NDEF message has been found
	* @retval ERROR :  a NDEF message doens't have been found
  */
static int8_t User_ReadNDEFMessage ( uint8_t *PayloadLength )			
{
uint8_t NthAttempt=0, 
				NbAttempt = 2;
				
	*PayloadLength = 0;
		
	for (NthAttempt = 0; NthAttempt < NbAttempt ; NthAttempt++)
	{
		M24LR04E_Init();
		// check if a NDEF message is available in the M24LR04 EEPROM					
		if (User_CheckNDEFMessage() == SUCCESS)
		{
			User_GetPayloadLength(PayloadLength);
			if (PayloadLength !=0x00)
			{
				(*PayloadLength) -=2;
				InitializeBuffer (NDEFmessage,(*PayloadLength)+10);
				User_GetNDEFMessage(*PayloadLength,NDEFmessage);
			
				I2C_Cmd(M24LR04E_I2C, DISABLE);			
				
				CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, DISABLE);	
	
				GPIO_HIGH(M24LR04E_I2C_SCL_GPIO_PORT,M24LR04E_I2C_SCL_PIN);	
				GPIO_HIGH(M24LR04E_I2C_SCL_GPIO_PORT,M24LR04E_I2C_SDA_PIN);	
		
				ToUpperCase (*PayloadLength,NDEFmessage);
				
				return SUCCESS;
			}
		}
		
		M24LR04E_DeInit();
		I2C_Cmd(M24LR04E_I2C, DISABLE);
	}
	
	return ERROR;
}

/**
  * @brief this function checks if a NDEF message is available in the M24LR04E-R EEPROM
	* @par PayloadLength : the number of byte of the NDEF message
  * @retval SUCCESS : A NDEF message has been found
	* @retval ERROR :  a NDEF message doens't have been found
  */
static int8_t User_WriteFirmwareVersion ( void )			
{
uint8_t *OneByte = 0x00;
uint16_t WriteAddr = 0x01FC;				
	
		
	M24LR04E_Init();
		
	M24LR04E_WriteOneByte (M24LR16_EEPROM_ADDRESS_USER, WriteAddr++, FirmwareVersion [0]);			

	
	I2C_Cmd(M24LR04E_I2C, DISABLE);			
		
	CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, DISABLE);	
	
	GPIO_HIGH(M24LR04E_I2C_SCL_GPIO_PORT,M24LR04E_I2C_SCL_PIN);	
	GPIO_HIGH(M24LR04E_I2C_SCL_GPIO_PORT,M24LR04E_I2C_SDA_PIN);	
		
		
	M24LR04E_DeInit();
	I2C_Cmd(M24LR04E_I2C, DISABLE);
	
	
	return SUCCESS;
}

/**
  * @brief this function checks if a NDEF message is available
	* @par Parameters None
  * @retval none
  */
static void User_DisplayMessage (uint8_t message[],uint8_t PayloadLength )
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
		
		LCD_GLASS_ScrollSentenceNbCar(message,30,PayloadLength+6);		
		
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

/**
  * @brief this function checks if a NDEF message is available
	* @par Parameters None
  * @retval none
  */
static void User_DisplayMessageActiveHaltMode ( uint8_t PayloadLength )
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
		
		// disable interupt				
		sim();
		// To copy function DISPLAYRAM in RAM section DISPLAY
		#ifdef _COSMIC_
			if (!(_fctcpy('D')))
				while(1);
		#endif
				
			Display_Ram (); // Call in RAM
			
		//	state_machine = STATE_VREF;
						
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
							
		// enable interupt				
		rim();
		

}

	
					
/**
  * @brief this functions reads some specific bytes from M24LR16E memory
	* @brief in order tocheck if the memory contains an NDEF message
	* @par Parameters None
  * @retval SUCCESS : the M24LR16E contains a NDEF message
	* @retval ERROR : the M24LR16E donesn't contain a NDEF message
  */
static ErrorStatus User_CheckNDEFMessage(void)
{
uint8_t *OneByte = 0x00;
uint16_t ReadAddr = 0x0000;
	
	// check the E1 at address 0
	M24LR04E_ReadOneByte (M24LR16_EEPROM_ADDRESS_USER, ReadAddr, OneByte);	
	if (*OneByte != 0xE1)
		return ERROR;
	
	ReadAddr = 0x0009;
	M24LR04E_ReadOneByte (M24LR16_EEPROM_ADDRESS_USER, ReadAddr, OneByte);	
	// text or URL message
	if (*OneByte != 0x54 /*&& *OneByte != 0x55*/)
		return ERROR;
		
	return SUCCESS;	
}

/**
  * @brief this functions returns the payload length 
	* @par Parameters None
  * @retval SUCCESS : the M24LR16E contains a NDEF message
	* @retval ERROR : the M24LR16E donesn't contain a NDEF message
  */
static ErrorStatus User_GetPayloadLength(uint8_t *PayloadLength)
{
uint16_t ReadAddr = 0x0008;
	
	*PayloadLength = 0x00;
	
	M24LR04E_ReadOneByte (M24LR16_EEPROM_ADDRESS_USER, ReadAddr, PayloadLength);	
	if (*PayloadLength == 0x00)
		return ERROR;
		
	return SUCCESS;	
}


/**
  * @brief this functions returns the payload length 
	* @par PayloadLength : the number of byte of the NDEF message
	* @par NDEFmessage : pointer on the NDEF message
  * @retval SUCCESS : the M24LR16E contains a NDEF message
	* @retval ERROR : the M24LR16E donesn't contain a NDEF message
  */
static ErrorStatus User_GetNDEFMessage(uint8_t  PayloadLength,uint8_t *NDEFmessage)
{
uint16_t ReadAddr = 0x000D;

	if (PayloadLength == 0x00)
		return SUCCESS;		
		
	M24LR04E_ReadBuffer (M24LR16_EEPROM_ADDRESS_USER, ReadAddr,PayloadLength, NDEFmessage);	
		
	return SUCCESS;	
}

/**
  * @brief this functions desactivates energy harvesting 
	* @par Parameters None
  * @retval void
  */
static void User_DesactivateEnergyHarvesting ( void )
{
uint16_t WriteAddr = 0x0920;
	M24LR04E_WriteOneByte (M24LR16_EEPROM_ADDRESS_SYSTEM, WriteAddr,0x00)	;
}

/**
  * @brief this functions converts a the characters of a string to upper case
	* @par NbCar : Number of byte of StringToConvert field
	* @par StringToConvert : pointer on the string to convert
  * @retval none
  */
static void ToUpperCase (uint8_t  NbCar ,uint8_t *StringToConvert)
{
uint8_t Buffer[0xFF],
				i=3,
				NbSpace = 6;
				
	for (i=0;i<NbSpace;i++)
			Buffer[i] = ' ';
	// copy StringToConvert into buffer
	for (i=0;i<NbCar;i++)
			Buffer[i+NbSpace] = StringToConvert[i];
	
	for (i=0;i<NbCar+NbSpace;i++)
	{
		if (Buffer[i] >= 0x61 && Buffer[i] <= 0x7A)
			StringToConvert[i] = Buffer[i]-32;
		else 	
			StringToConvert[i] = Buffer[i];
	}
	StringToConvert[NbCar+NbSpace] = ' ';
	StringToConvert[NbCar+NbSpace+1] = 0;

}

/**
  * @brief this functions deinitializes all the clocks
	* @par Parameters None
  * @retval none
  */
static void DeInitClock ( void )
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
/**
  * @brief this functions configures all the GPIO as 
	* @par Parameters None
  * @retval none
  */
static void DeInitGPIO ( void )
{
		//GPIO_Mode_In_PU_No_IT
	GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
	GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
//SSe	GPIO_Init( GPIOC, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
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

/**
  * @brief this functions initilizes a buffer
	* @par Buffer : pointer on the string to initialize
	* @par NbCar : Number of byte of Buffer field
  * @retval none
  */
static void InitializeBuffer (uint8_t *Buffer ,uint8_t NbCar)
{
	
	do{
		
		Buffer[NbCar]= 0;
	}	while (NbCar--);
}


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
