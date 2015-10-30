   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.10.2 - 02 Nov 2011
   3                     ; Generator (Limited) V4.3.7 - 29 Nov 2011
  17                     .const:	section	.text
  18  0000               _ErrorMessage:
  19  0000 4e            	dc.b	78
  20  0001 4f            	dc.b	79
  21  0002 54            	dc.b	84
  22  0003 20            	dc.b	32
  23  0004 41            	dc.b	65
  24  0005 20            	dc.b	32
  25  0006 4e            	dc.b	78
  26  0007 44            	dc.b	68
  27  0008 45            	dc.b	69
  28  0009 46            	dc.b	70
  29  000a 20            	dc.b	32
  30  000b 54            	dc.b	84
  31  000c 45            	dc.b	69
  32  000d 58            	dc.b	88
  33  000e 54            	dc.b	84
  34  000f 20            	dc.b	32
  35  0010 4d            	dc.b	77
  36  0011 45            	dc.b	69
  37  0012 53            	dc.b	83
  38  0013 53            	dc.b	83
  39  0014 41            	dc.b	65
  40  0015 47            	dc.b	71
  41  0016 45            	dc.b	69
  42  0017 20            	dc.b	32
  43  0018 20            	dc.b	32
  44  0019 20            	dc.b	32
  45  001a 20            	dc.b	32
  46  001b 20            	dc.b	32
  47  001c 20            	dc.b	32
  48  001d 20            	dc.b	32
  49  001e 20            	dc.b	32
  50  001f 20            	dc.b	32
  51  0020 20            	dc.b	32
  52  0021 20            	dc.b	32
  53  0022 20            	dc.b	32
  54  0023 20            	dc.b	32
  55  0024 20            	dc.b	32
  56  0025 20            	dc.b	32
  57  0026 20            	dc.b	32
  58  0027 20            	dc.b	32
  59  0028 20            	dc.b	32
  68                     .eeprom:	section	.data
  69  0000               _EEInitial:
  70  0000 00            	dc.b	0
  71                     	switch	.const
  72  0029               _FirmwareVersion:
  73  0029 13            	dc.b	19
  74  002a 00            	dc.b	0
 157                     ; 78 void main(void)
 157                     ; 79 { 
 159                     	switch	.text
 160  0000               _main:
 162  0000 5204          	subw	sp,#4
 163       00000004      OFST:	set	4
 166                     ; 85 	DeInitClock();
 168  0002 cd04a7        	call	L71_DeInitClock
 170                     ; 86 	DeInitGPIO();
 172  0005 cd0530        	call	L12_DeInitGPIO
 174                     ; 90 		CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
 176  0008 a601          	ld	a,#1
 177  000a cd0000        	call	_CLK_SYSCLKSourceConfig
 179                     ; 91 		CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_16);	
 181  000d a604          	ld	a,#4
 182  000f cd0000        	call	_CLK_SYSCLKDivConfig
 184                     ; 101   LCD_GLASS_Init();
 186  0012 cd0000        	call	_LCD_GLASS_Init
 188                     ; 105 	GPIO_Init( LED_GPIO_PORT, LED_GPIO_PIN, GPIO_Mode_Out_PP_Low_Fast);
 190  0015 4be0          	push	#224
 191  0017 4b40          	push	#64
 192  0019 ae5014        	ldw	x,#20500
 193  001c cd0000        	call	_GPIO_Init
 195  001f 85            	popw	x
 196                     ; 107 	GPIOE->ODR &= ~LED_GPIO_PIN;
 198  0020 721d5014      	bres	20500,#6
 199                     ; 110   GPIO_Init( BUTTON_GPIO_PORT, USER_GPIO_PIN, GPIO_Mode_In_FL_IT);
 201  0024 4b20          	push	#32
 202  0026 4b80          	push	#128
 203  0028 ae500a        	ldw	x,#20490
 204  002b cd0000        	call	_GPIO_Init
 206  002e 85            	popw	x
 207                     ; 111 	EXTI_SetPinSensitivity(EXTI_Pin_7, EXTI_Trigger_Falling);
 209  002f ae1602        	ldw	x,#5634
 210  0032 cd0000        	call	_EXTI_SetPinSensitivity
 212                     ; 114   BAR0_OFF;
 214  0035 721f0000      	bres	_t_bar,#7
 215                     ; 115   BAR1_OFF;
 217  0039 72170001      	bres	_t_bar+1,#3
 218                     ; 116   BAR2_OFF;
 220  003d 721b0000      	bres	_t_bar,#5
 221                     ; 117   BAR3_OFF;	
 223  0041 72130001      	bres	_t_bar+1,#1
 224                     ; 119 	enableInterrupts();
 227  0045 9a            rim
 229                     ; 123 	bufMessage = NDEFmessage;
 232  0046 ae0000        	ldw	x,#_NDEFmessage
 233  0049 1f01          	ldw	(OFST-3,sp),x
 234                     ; 125 	if (EEMenuState > STATE_TEMPMEAS) 
 236  004b c60001        	ld	a,_EEMenuState
 237  004e a103          	cp	a,#3
 238  0050 2507          	jrult	L111
 239                     ; 126 		EEMenuState = STATE_CHECKNDEFMESSAGE;
 241  0052 4f            	clr	a
 242  0053 ae0001        	ldw	x,#_EEMenuState
 243  0056 cd0000        	call	c_eewrc
 245  0059               L111:
 246                     ; 128 	FLASH_Unlock(FLASH_MemType_Data );
 248  0059 a6f7          	ld	a,#247
 249  005b cd0000        	call	_FLASH_Unlock
 251                     ; 130 	state_machine = EEMenuState ; 
 253  005e 5500010040    	mov	_state_machine,_EEMenuState
 254                     ; 132 	delayLFO_ms (1);
 256  0063 ae0001        	ldw	x,#1
 257  0066 cd0000        	call	_delayLFO_ms
 259                     ; 134 	if (EEInitial == 0)
 261  0069 725d0000      	tnz	_EEInitial
 262  006d 260b          	jrne	L511
 263                     ; 136 			User_WriteFirmwareVersion ();
 265  006f cd01cc        	call	L34_User_WriteFirmwareVersion
 267                     ; 137 			EEInitial =1;
 269  0072 a601          	ld	a,#1
 270  0074 ae0000        	ldw	x,#_EEInitial
 271  0077 cd0000        	call	c_eewrc
 273  007a               L511:
 274                     ; 143     switch (state_machine)
 276  007a b640          	ld	a,_state_machine
 278                     ; 184         break;
 279  007c 4d            	tnz	a
 280  007d 2720          	jreq	L74
 281  007f 4a            	dec	a
 282  0080 2712          	jreq	L54
 283  0082 4a            	dec	a
 284  0083 273b          	jreq	L15
 285  0085               L35:
 286                     ; 180         default:
 286                     ; 181 					LCD_GLASS_Clear();
 289  0085 cd0000        	call	_LCD_GLASS_Clear
 291                     ; 182 					LCD_GLASS_DisplayString("Error");
 293  0088 ae002b        	ldw	x,#L131
 294  008b cd0000        	call	_LCD_GLASS_DisplayString
 296                     ; 183 					state_machine = STATE_VREF;
 298  008e 35010040      	mov	_state_machine,#1
 299                     ; 184         break;
 301  0092 20e6          	jra	L511
 302  0094               L54:
 303                     ; 146 				case STATE_VREF:
 303                     ; 147 					// measure the voltage available at the output of the M24LR04E-R
 303                     ; 148 					Vref_measure();
 305  0094 cd0000        	call	_Vref_measure
 307                     ; 150 					delayLFO_ms (2);
 309  0097 ae0002        	ldw	x,#2
 310  009a cd0000        	call	_delayLFO_ms
 312                     ; 151         break;
 314  009d 20db          	jra	L511
 315  009f               L74:
 316                     ; 153         case STATE_CHECKNDEFMESSAGE:
 316                     ; 154 				
 316                     ; 155 						// read the NDEF message from the M24LR04E-R EEPROM and display it if it is found 				
 316                     ; 156 					if (User_ReadNDEFMessage (&PayloadLength) == SUCCESS)						
 318  009f 96            	ldw	x,sp
 319  00a0 1c0003        	addw	x,#OFST-1
 320  00a3 cd0154        	call	L13_User_ReadNDEFMessage
 322  00a6 a101          	cp	a,#1
 323  00a8 260b          	jrne	L521
 324                     ; 157 						User_DisplayMessage (bufMessage,PayloadLength);
 326  00aa 7b03          	ld	a,(OFST-1,sp)
 327  00ac 88            	push	a
 328  00ad 1e02          	ldw	x,(OFST-2,sp)
 329  00af cd0214        	call	L33_User_DisplayMessage
 331  00b2 84            	pop	a
 333  00b3 20c5          	jra	L511
 334  00b5               L521:
 335                     ; 160 						User_DisplayMessage(ErrorMessage,20);		
 337  00b5 4b14          	push	#20
 338  00b7 ae0000        	ldw	x,#_ErrorMessage
 339  00ba cd0214        	call	L33_User_DisplayMessage
 341  00bd 84            	pop	a
 342  00be 20ba          	jra	L511
 343  00c0               L15:
 344                     ; 166 				case STATE_TEMPMEAS:
 344                     ; 167 						
 344                     ; 168 						// read the ambiant tempserature from the STTS751
 344                     ; 169 						User_GetOneTemperature (&data_sensor);
 346  00c0 96            	ldw	x,sp
 347  00c1 1c0004        	addw	x,#OFST+0
 348  00c4 ad0e          	call	L73_User_GetOneTemperature
 350                     ; 171 						User_DisplayOneTemperature (data_sensor);
 352  00c6 7b04          	ld	a,(OFST+0,sp)
 353  00c8 ad3c          	call	L14_User_DisplayOneTemperature
 355                     ; 173 						delayLFO_ms (2);
 357  00ca ae0002        	ldw	x,#2
 358  00cd cd0000        	call	_delayLFO_ms
 360                     ; 175 				break;
 362  00d0 20a8          	jra	L511
 363  00d2               L321:
 364                     ; 184         break;
 365  00d2 20a6          	jra	L511
 415                     ; 198 static void User_GetOneTemperature (uint8_t *data_sensor)
 415                     ; 199 {
 416                     	switch	.text
 417  00d4               L73_User_GetOneTemperature:
 419  00d4 89            	pushw	x
 420  00d5 88            	push	a
 421       00000001      OFST:	set	1
 424                     ; 200 uint8_t 	Pointer_temperature = 0x00;					/* temperature access */
 426  00d6 0f01          	clr	(OFST+0,sp)
 427                     ; 203 	data_sensor[0] = 0x00;
 429  00d8 7f            	clr	(x)
 430                     ; 205 	I2C_SS_Init();
 432  00d9 cd0000        	call	_I2C_SS_Init
 434                     ; 206 	I2C_SS_Config(STTS751_STOP);
 436  00dc ae0340        	ldw	x,#832
 437  00df cd0000        	call	_I2C_SS_Config
 439                     ; 208 	delayLFO_ms (1);
 441  00e2 ae0001        	ldw	x,#1
 442  00e5 cd0000        	call	_delayLFO_ms
 444                     ; 209 	I2C_SS_Config(STTS751_ONESHOTMODE);
 446  00e8 ae0f00        	ldw	x,#3840
 447  00eb cd0000        	call	_I2C_SS_Config
 449                     ; 211 	delayLFO_ms (1);
 451  00ee ae0001        	ldw	x,#1
 452  00f1 cd0000        	call	_delayLFO_ms
 454                     ; 213 	I2C_SS_ReadOneByte(data_sensor,Pointer_temperature);
 456  00f4 7b01          	ld	a,(OFST+0,sp)
 457  00f6 88            	push	a
 458  00f7 1e03          	ldw	x,(OFST+2,sp)
 459  00f9 cd0000        	call	_I2C_SS_ReadOneByte
 461  00fc 84            	pop	a
 462                     ; 215 	delayLFO_ms (1);
 464  00fd ae0001        	ldw	x,#1
 465  0100 cd0000        	call	_delayLFO_ms
 467                     ; 217 }
 470  0103 5b03          	addw	sp,#3
 471  0105 81            	ret
 517                     ; 224 static void User_DisplayOneTemperature (uint8_t data_sensor)
 517                     ; 225 {
 518                     	switch	.text
 519  0106               L14_User_DisplayOneTemperature:
 521  0106 88            	push	a
 522  0107 520c          	subw	sp,#12
 523       0000000c      OFST:	set	12
 526                     ; 228 	TempChar16[5] = 'C';
 528  0109 ae0043        	ldw	x,#67
 529  010c 1f0b          	ldw	(OFST-1,sp),x
 530                     ; 229 	TempChar16[4] = ' ';
 532  010e ae0020        	ldw	x,#32
 533  0111 1f09          	ldw	(OFST-3,sp),x
 534                     ; 231 	if ((data_sensor & 0x80) != 0)
 536  0113 a580          	bcp	a,#128
 537  0115 270d          	jreq	L771
 538                     ; 233 		data_sensor = (~data_sensor) +1;
 540  0117 7b0d          	ld	a,(OFST+1,sp)
 541  0119 43            	cpl	a
 542  011a 4c            	inc	a
 543  011b 6b0d          	ld	(OFST+1,sp),a
 544                     ; 234 		TempChar16[1] = '-';
 546  011d ae002d        	ldw	x,#45
 547  0120 1f03          	ldw	(OFST-9,sp),x
 549  0122 2005          	jra	L102
 550  0124               L771:
 551                     ; 237 		TempChar16[1] = ' ';							
 553  0124 ae0020        	ldw	x,#32
 554  0127 1f03          	ldw	(OFST-9,sp),x
 555  0129               L102:
 556                     ; 239 	TempChar16[3] = (data_sensor %10) + 0x30 ;
 558  0129 7b0d          	ld	a,(OFST+1,sp)
 559  012b 5f            	clrw	x
 560  012c 97            	ld	xl,a
 561  012d a60a          	ld	a,#10
 562  012f cd0000        	call	c_smodx
 564  0132 1c0030        	addw	x,#48
 565  0135 1f07          	ldw	(OFST-5,sp),x
 566                     ; 240 	TempChar16[2] = (data_sensor /10) + 0x30;
 568  0137 7b0d          	ld	a,(OFST+1,sp)
 569  0139 5f            	clrw	x
 570  013a 97            	ld	xl,a
 571  013b a60a          	ld	a,#10
 572  013d cd0000        	call	c_sdivx
 574  0140 1c0030        	addw	x,#48
 575  0143 1f05          	ldw	(OFST-7,sp),x
 576                     ; 241 	TempChar16[0] = ' ';
 578  0145 ae0020        	ldw	x,#32
 579  0148 1f01          	ldw	(OFST-11,sp),x
 580                     ; 242 	LCD_GLASS_DisplayStrDeci(TempChar16);		
 582  014a 96            	ldw	x,sp
 583  014b 1c0001        	addw	x,#OFST-11
 584  014e cd0000        	call	_LCD_GLASS_DisplayStrDeci
 586                     ; 243 }
 589  0151 5b0d          	addw	sp,#13
 590  0153 81            	ret
 654                     ; 251 static int8_t User_ReadNDEFMessage ( uint8_t *PayloadLength )			
 654                     ; 252 {
 655                     	switch	.text
 656  0154               L13_User_ReadNDEFMessage:
 658  0154 89            	pushw	x
 659  0155 89            	pushw	x
 660       00000002      OFST:	set	2
 663                     ; 253 uint8_t NthAttempt=0, 
 665                     ; 254 				NbAttempt = 2;
 667  0156 a602          	ld	a,#2
 668  0158 6b01          	ld	(OFST-1,sp),a
 669                     ; 256 	*PayloadLength = 0;
 671  015a 7f            	clr	(x)
 672                     ; 258 	for (NthAttempt = 0; NthAttempt < NbAttempt ; NthAttempt++)
 674  015b 0f02          	clr	(OFST+0,sp)
 676  015d 2063          	jra	L532
 677  015f               L132:
 678                     ; 260 		M24LR04E_Init();
 680  015f cd0000        	call	_M24LR04E_Init
 682                     ; 262 		if (User_CheckNDEFMessage() == SUCCESS)
 684  0162 cd02c3        	call	L11_User_CheckNDEFMessage
 686  0165 a101          	cp	a,#1
 687  0167 264b          	jrne	L142
 688                     ; 264 			User_GetPayloadLength(PayloadLength);
 690  0169 1e03          	ldw	x,(OFST+1,sp)
 691  016b cd02fa        	call	L7_User_GetPayloadLength
 693                     ; 265 			if (PayloadLength !=0x00)
 695  016e 1e03          	ldw	x,(OFST+1,sp)
 696  0170 2742          	jreq	L142
 697                     ; 267 				(*PayloadLength) -=2;
 699  0172 1e03          	ldw	x,(OFST+1,sp)
 700  0174 7a            	dec	(x)
 701  0175 7a            	dec	(x)
 702                     ; 268 				InitializeBuffer (NDEFmessage,(*PayloadLength)+10);
 704  0176 1e03          	ldw	x,(OFST+1,sp)
 705  0178 f6            	ld	a,(x)
 706  0179 ab0a          	add	a,#10
 707  017b 88            	push	a
 708  017c ae0000        	ldw	x,#_NDEFmessage
 709  017f cd057c        	call	L72_InitializeBuffer
 711  0182 84            	pop	a
 712                     ; 269 				User_GetNDEFMessage(*PayloadLength,NDEFmessage);
 714  0183 ae0000        	ldw	x,#_NDEFmessage
 715  0186 89            	pushw	x
 716  0187 1e05          	ldw	x,(OFST+3,sp)
 717  0189 f6            	ld	a,(x)
 718  018a cd0316        	call	L31_User_GetNDEFMessage
 720  018d 85            	popw	x
 721                     ; 271 				I2C_Cmd(M24LR04E_I2C, DISABLE);			
 723  018e 4b00          	push	#0
 724  0190 ae5210        	ldw	x,#21008
 725  0193 cd0000        	call	_I2C_Cmd
 727  0196 84            	pop	a
 728                     ; 273 				CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, DISABLE);	
 730  0197 ae0300        	ldw	x,#768
 731  019a cd0000        	call	_CLK_PeripheralClockConfig
 733                     ; 275 				GPIO_HIGH(M24LR04E_I2C_SCL_GPIO_PORT,M24LR04E_I2C_SCL_PIN);	
 735  019d 7212500a      	bset	20490,#1
 736                     ; 276 				GPIO_HIGH(M24LR04E_I2C_SCL_GPIO_PORT,M24LR04E_I2C_SDA_PIN);	
 738  01a1 7210500a      	bset	20490,#0
 739                     ; 278 				ToUpperCase (*PayloadLength,NDEFmessage);
 741  01a5 ae0000        	ldw	x,#_NDEFmessage
 742  01a8 89            	pushw	x
 743  01a9 1e05          	ldw	x,(OFST+3,sp)
 744  01ab f6            	ld	a,(x)
 745  01ac cd0345        	call	L51_ToUpperCase
 747  01af 85            	popw	x
 748                     ; 280 				return SUCCESS;
 750  01b0 a601          	ld	a,#1
 752  01b2 2015          	jra	L41
 753  01b4               L142:
 754                     ; 284 		M24LR04E_DeInit();
 756  01b4 cd0000        	call	_M24LR04E_DeInit
 758                     ; 285 		I2C_Cmd(M24LR04E_I2C, DISABLE);
 760  01b7 4b00          	push	#0
 761  01b9 ae5210        	ldw	x,#21008
 762  01bc cd0000        	call	_I2C_Cmd
 764  01bf 84            	pop	a
 765                     ; 258 	for (NthAttempt = 0; NthAttempt < NbAttempt ; NthAttempt++)
 767  01c0 0c02          	inc	(OFST+0,sp)
 768  01c2               L532:
 771  01c2 7b02          	ld	a,(OFST+0,sp)
 772  01c4 1101          	cp	a,(OFST-1,sp)
 773  01c6 2597          	jrult	L132
 774                     ; 288 	return ERROR;
 776  01c8 4f            	clr	a
 778  01c9               L41:
 780  01c9 5b04          	addw	sp,#4
 781  01cb 81            	ret
 832                     ; 297 static int8_t User_WriteFirmwareVersion ( void )			
 832                     ; 298 {
 833                     	switch	.text
 834  01cc               L34_User_WriteFirmwareVersion:
 836  01cc 5204          	subw	sp,#4
 837       00000004      OFST:	set	4
 840                     ; 299 uint8_t *OneByte = 0x00;
 842  01ce 5f            	clrw	x
 843  01cf 1f01          	ldw	(OFST-3,sp),x
 844                     ; 300 uint16_t WriteAddr = 0x01FC;				
 846  01d1 ae01fc        	ldw	x,#508
 847  01d4 1f03          	ldw	(OFST-1,sp),x
 848                     ; 303 	M24LR04E_Init();
 850  01d6 cd0000        	call	_M24LR04E_Init
 852                     ; 305 	M24LR04E_WriteOneByte (M24LR16_EEPROM_ADDRESS_USER, WriteAddr++, FirmwareVersion [0]);			
 854  01d9 4b13          	push	#19
 855  01db 1604          	ldw	y,(OFST+0,sp)
 856  01dd 0c05          	inc	(OFST+1,sp)
 857  01df 2602          	jrne	L02
 858  01e1 0c04          	inc	(OFST+0,sp)
 859  01e3               L02:
 860  01e3 9089          	pushw	y
 861  01e5 a6a6          	ld	a,#166
 862  01e7 cd0000        	call	_M24LR04E_WriteOneByte
 864  01ea 5b03          	addw	sp,#3
 865                     ; 308 	I2C_Cmd(M24LR04E_I2C, DISABLE);			
 867  01ec 4b00          	push	#0
 868  01ee ae5210        	ldw	x,#21008
 869  01f1 cd0000        	call	_I2C_Cmd
 871  01f4 84            	pop	a
 872                     ; 310 	CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, DISABLE);	
 874  01f5 ae0300        	ldw	x,#768
 875  01f8 cd0000        	call	_CLK_PeripheralClockConfig
 877                     ; 312 	GPIO_HIGH(M24LR04E_I2C_SCL_GPIO_PORT,M24LR04E_I2C_SCL_PIN);	
 879  01fb 7212500a      	bset	20490,#1
 880                     ; 313 	GPIO_HIGH(M24LR04E_I2C_SCL_GPIO_PORT,M24LR04E_I2C_SDA_PIN);	
 882  01ff 7210500a      	bset	20490,#0
 883                     ; 316 	M24LR04E_DeInit();
 885  0203 cd0000        	call	_M24LR04E_DeInit
 887                     ; 317 	I2C_Cmd(M24LR04E_I2C, DISABLE);
 889  0206 4b00          	push	#0
 890  0208 ae5210        	ldw	x,#21008
 891  020b cd0000        	call	_I2C_Cmd
 893  020e 84            	pop	a
 894                     ; 320 	return SUCCESS;
 896  020f a601          	ld	a,#1
 899  0211 5b04          	addw	sp,#4
 900  0213 81            	ret
 950                     ; 328 static void User_DisplayMessage (uint8_t message[],uint8_t PayloadLength )
 950                     ; 329 {
 951                     	switch	.text
 952  0214               L33_User_DisplayMessage:
 954  0214 89            	pushw	x
 955       00000000      OFST:	set	0
 958                     ; 340 		CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
 960  0215 4f            	clr	a
 961  0216 cd0000        	call	_CLK_SYSCLKDivConfig
 963                     ; 341 		CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);
 965  0219 a602          	ld	a,#2
 966  021b cd0000        	call	_CLK_SYSCLKSourceConfig
 968                     ; 342 		CLK_SYSCLKSourceSwitchCmd(ENABLE);
 970  021e a601          	ld	a,#1
 971  0220 cd0000        	call	_CLK_SYSCLKSourceSwitchCmd
 974  0223               L313:
 975                     ; 343 		while (((CLK->SWCR)& 0x01)==0x01);
 977  0223 c650c9        	ld	a,20681
 978  0226 a401          	and	a,#1
 979  0228 a101          	cp	a,#1
 980  022a 27f7          	jreq	L313
 981                     ; 344 		CLK_HSICmd(DISABLE);
 983  022c 4f            	clr	a
 984  022d cd0000        	call	_CLK_HSICmd
 986                     ; 345 		CLK->ECKCR &= ~0x01; 
 988  0230 721150c6      	bres	20678,#0
 989                     ; 348 		LCD_GLASS_ScrollSentenceNbCar(message,30,PayloadLength+6);		
 991  0234 7b05          	ld	a,(OFST+5,sp)
 992  0236 ab06          	add	a,#6
 993  0238 88            	push	a
 994  0239 ae001e        	ldw	x,#30
 995  023c 89            	pushw	x
 996  023d 1e04          	ldw	x,(OFST+4,sp)
 997  023f cd0000        	call	_LCD_GLASS_ScrollSentenceNbCar
 999  0242 5b03          	addw	sp,#3
1000                     ; 352 			CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_16);
1002  0244 a604          	ld	a,#4
1003  0246 cd0000        	call	_CLK_SYSCLKDivConfig
1005                     ; 353 			CLK_HSICmd(ENABLE);
1007  0249 a601          	ld	a,#1
1008  024b cd0000        	call	_CLK_HSICmd
1011  024e               L123:
1012                     ; 354 			while (((CLK->ICKCR)& 0x02)!=0x02);			
1014  024e c650c2        	ld	a,20674
1015  0251 a402          	and	a,#2
1016  0253 a102          	cp	a,#2
1017  0255 26f7          	jrne	L123
1018                     ; 355 			CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
1020  0257 a601          	ld	a,#1
1021  0259 cd0000        	call	_CLK_SYSCLKSourceConfig
1023                     ; 356 			CLK_SYSCLKSourceSwitchCmd(ENABLE);
1025  025c a601          	ld	a,#1
1026  025e cd0000        	call	_CLK_SYSCLKSourceSwitchCmd
1029  0261               L723:
1030                     ; 357 			while (((CLK->SWCR)& 0x01)==0x01);
1032  0261 c650c9        	ld	a,20681
1033  0264 a401          	and	a,#1
1034  0266 a101          	cp	a,#1
1035  0268 27f7          	jreq	L723
1036                     ; 367 }
1039  026a 85            	popw	x
1040  026b 81            	ret
1083                     ; 374 static void User_DisplayMessageActiveHaltMode ( uint8_t PayloadLength )
1083                     ; 375 {
1084                     	switch	.text
1085  026c               L53_User_DisplayMessageActiveHaltMode:
1089                     ; 387 			CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
1091  026c 4f            	clr	a
1092  026d cd0000        	call	_CLK_SYSCLKDivConfig
1094                     ; 388 			CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_LSI);
1096  0270 a602          	ld	a,#2
1097  0272 cd0000        	call	_CLK_SYSCLKSourceConfig
1099                     ; 389 			CLK_SYSCLKSourceSwitchCmd(ENABLE);
1101  0275 a601          	ld	a,#1
1102  0277 cd0000        	call	_CLK_SYSCLKSourceSwitchCmd
1105  027a               L353:
1106                     ; 390 			while (((CLK->SWCR)& 0x01)==0x01);
1108  027a c650c9        	ld	a,20681
1109  027d a401          	and	a,#1
1110  027f a101          	cp	a,#1
1111  0281 27f7          	jreq	L353
1112                     ; 391 			CLK_HSICmd(DISABLE);
1114  0283 4f            	clr	a
1115  0284 cd0000        	call	_CLK_HSICmd
1117                     ; 392 			CLK->ECKCR &= ~0x01; 
1119  0287 721150c6      	bres	20678,#0
1120                     ; 396 		sim();
1123  028b 9b            sim
1125                     ; 399 			if (!(_fctcpy('D')))
1128  028c a644          	ld	a,#68
1129  028e cd0000        	call	__fctcpy
1131  0291 a30000        	cpw	x,#0
1132  0294 2602          	jrne	L753
1133  0296               L163:
1134                     ; 400 				while(1);
1136  0296 20fe          	jra	L163
1137  0298               L753:
1138                     ; 403 			Display_Ram (); // Call in RAM
1140  0298 cd0000        	call	_Display_Ram
1142                     ; 409 			CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_16);
1144  029b a604          	ld	a,#4
1145  029d cd0000        	call	_CLK_SYSCLKDivConfig
1147                     ; 410 			CLK_HSICmd(ENABLE);
1149  02a0 a601          	ld	a,#1
1150  02a2 cd0000        	call	_CLK_HSICmd
1153  02a5               L763:
1154                     ; 411 			while (((CLK->ICKCR)& 0x02)!=0x02);			
1156  02a5 c650c2        	ld	a,20674
1157  02a8 a402          	and	a,#2
1158  02aa a102          	cp	a,#2
1159  02ac 26f7          	jrne	L763
1160                     ; 412 			CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);
1162  02ae a601          	ld	a,#1
1163  02b0 cd0000        	call	_CLK_SYSCLKSourceConfig
1165                     ; 413 			CLK_SYSCLKSourceSwitchCmd(ENABLE);
1167  02b3 a601          	ld	a,#1
1168  02b5 cd0000        	call	_CLK_SYSCLKSourceSwitchCmd
1171  02b8               L573:
1172                     ; 414 			while (((CLK->SWCR)& 0x01)==0x01);
1174  02b8 c650c9        	ld	a,20681
1175  02bb a401          	and	a,#1
1176  02bd a101          	cp	a,#1
1177  02bf 27f7          	jreq	L573
1178                     ; 426 		rim();
1181  02c1 9a            rim
1183                     ; 429 }
1187  02c2 81            	ret
1254                     ; 440 static ErrorStatus User_CheckNDEFMessage(void)
1254                     ; 441 {
1255                     	switch	.text
1256  02c3               L11_User_CheckNDEFMessage:
1258  02c3 5204          	subw	sp,#4
1259       00000004      OFST:	set	4
1262                     ; 442 uint8_t *OneByte = 0x00;
1264  02c5 5f            	clrw	x
1265  02c6 1f03          	ldw	(OFST-1,sp),x
1266                     ; 443 uint16_t ReadAddr = 0x0000;
1268                     ; 446 	M24LR04E_ReadOneByte (M24LR16_EEPROM_ADDRESS_USER, ReadAddr, OneByte);	
1270  02c8 5f            	clrw	x
1271  02c9 89            	pushw	x
1272  02ca 5f            	clrw	x
1273  02cb 89            	pushw	x
1274  02cc a6a6          	ld	a,#166
1275  02ce cd0000        	call	_M24LR04E_ReadOneByte
1277  02d1 5b04          	addw	sp,#4
1278                     ; 447 	if (*OneByte != 0xE1)
1280  02d3 1e03          	ldw	x,(OFST-1,sp)
1281  02d5 f6            	ld	a,(x)
1282  02d6 a1e1          	cp	a,#225
1283  02d8 2703          	jreq	L334
1284                     ; 448 		return ERROR;
1286  02da 4f            	clr	a
1288  02db 2016          	jra	L03
1289  02dd               L334:
1290                     ; 450 	ReadAddr = 0x0009;
1292                     ; 451 	M24LR04E_ReadOneByte (M24LR16_EEPROM_ADDRESS_USER, ReadAddr, OneByte);	
1294  02dd 1e03          	ldw	x,(OFST-1,sp)
1295  02df 89            	pushw	x
1296  02e0 ae0009        	ldw	x,#9
1297  02e3 89            	pushw	x
1298  02e4 a6a6          	ld	a,#166
1299  02e6 cd0000        	call	_M24LR04E_ReadOneByte
1301  02e9 5b04          	addw	sp,#4
1302                     ; 453 	if (*OneByte != 0x54 /*&& *OneByte != 0x55*/)
1304  02eb 1e03          	ldw	x,(OFST-1,sp)
1305  02ed f6            	ld	a,(x)
1306  02ee a154          	cp	a,#84
1307  02f0 2704          	jreq	L534
1308                     ; 454 		return ERROR;
1310  02f2 4f            	clr	a
1312  02f3               L03:
1314  02f3 5b04          	addw	sp,#4
1315  02f5 81            	ret
1316  02f6               L534:
1317                     ; 456 	return SUCCESS;	
1319  02f6 a601          	ld	a,#1
1321  02f8 20f9          	jra	L03
1368                     ; 465 static ErrorStatus User_GetPayloadLength(uint8_t *PayloadLength)
1368                     ; 466 {
1369                     	switch	.text
1370  02fa               L7_User_GetPayloadLength:
1372  02fa 89            	pushw	x
1373  02fb 89            	pushw	x
1374       00000002      OFST:	set	2
1377                     ; 467 uint16_t ReadAddr = 0x0008;
1379                     ; 469 	*PayloadLength = 0x00;
1381  02fc 7f            	clr	(x)
1382                     ; 471 	M24LR04E_ReadOneByte (M24LR16_EEPROM_ADDRESS_USER, ReadAddr, PayloadLength);	
1384  02fd 89            	pushw	x
1385  02fe ae0008        	ldw	x,#8
1386  0301 89            	pushw	x
1387  0302 a6a6          	ld	a,#166
1388  0304 cd0000        	call	_M24LR04E_ReadOneByte
1390  0307 5b04          	addw	sp,#4
1391                     ; 472 	if (*PayloadLength == 0x00)
1393  0309 1e03          	ldw	x,(OFST+1,sp)
1394  030b 7d            	tnz	(x)
1395  030c 2603          	jrne	L164
1396                     ; 473 		return ERROR;
1398  030e 4f            	clr	a
1400  030f 2002          	jra	L43
1401  0311               L164:
1402                     ; 475 	return SUCCESS;	
1404  0311 a601          	ld	a,#1
1406  0313               L43:
1408  0313 5b04          	addw	sp,#4
1409  0315 81            	ret
1465                     ; 486 static ErrorStatus User_GetNDEFMessage(uint8_t  PayloadLength,uint8_t *NDEFmessage)
1465                     ; 487 {
1466                     	switch	.text
1467  0316               L31_User_GetNDEFMessage:
1469  0316 88            	push	a
1470  0317 89            	pushw	x
1471       00000002      OFST:	set	2
1474                     ; 488 uint16_t ReadAddr = 0x000D;
1476                     ; 490 	if (PayloadLength == 0x00)
1478  0318 4d            	tnz	a
1479  0319 2604          	jrne	L115
1480                     ; 491 		return SUCCESS;		
1482  031b a601          	ld	a,#1
1484  031d 2013          	jra	L04
1485  031f               L115:
1486                     ; 493 	M24LR04E_ReadBuffer (M24LR16_EEPROM_ADDRESS_USER, ReadAddr,PayloadLength, NDEFmessage);	
1488  031f 1e06          	ldw	x,(OFST+4,sp)
1489  0321 89            	pushw	x
1490  0322 7b05          	ld	a,(OFST+3,sp)
1491  0324 88            	push	a
1492  0325 ae000d        	ldw	x,#13
1493  0328 89            	pushw	x
1494  0329 a6a6          	ld	a,#166
1495  032b cd0000        	call	_M24LR04E_ReadBuffer
1497  032e 5b05          	addw	sp,#5
1498                     ; 495 	return SUCCESS;	
1500  0330 a601          	ld	a,#1
1502  0332               L04:
1504  0332 5b03          	addw	sp,#3
1505  0334 81            	ret
1541                     ; 503 static void User_DesactivateEnergyHarvesting ( void )
1541                     ; 504 {
1542                     	switch	.text
1543  0335               L32_User_DesactivateEnergyHarvesting:
1545  0335 89            	pushw	x
1546       00000002      OFST:	set	2
1549                     ; 505 uint16_t WriteAddr = 0x0920;
1551                     ; 506 	M24LR04E_WriteOneByte (M24LR16_EEPROM_ADDRESS_SYSTEM, WriteAddr,0x00)	;
1553  0336 4b00          	push	#0
1554  0338 ae0920        	ldw	x,#2336
1555  033b 89            	pushw	x
1556  033c a6ae          	ld	a,#174
1557  033e cd0000        	call	_M24LR04E_WriteOneByte
1559  0341 5b03          	addw	sp,#3
1560                     ; 507 }
1563  0343 85            	popw	x
1564  0344 81            	ret
1636                     ; 515 static void ToUpperCase (uint8_t  NbCar ,uint8_t *StringToConvert)
1636                     ; 516 {
1637                     	switch	.text
1638  0345               L51_ToUpperCase:
1640  0345 88            	push	a
1641  0346 52ff          	subw	sp,#255
1642  0348 5212          	subw	sp,#18
1643       00000111      OFST:	set	273
1646                     ; 518 				i=3,
1648                     ; 519 				NbSpace = 6;
1650  034a a606          	ld	a,#6
1651  034c 6b11          	ld	(OFST-256,sp),a
1652                     ; 521 	for (i=0;i<NbSpace;i++)
1654  034e 96            	ldw	x,sp
1655  034f 1c0111        	addw	x,#OFST+0
1656  0352 7f            	clr	(x)
1658  0353 201a          	jra	L375
1659  0355               L765:
1660                     ; 522 			Buffer[i] = ' ';
1662  0355 96            	ldw	x,sp
1663  0356 1c0012        	addw	x,#OFST-255
1664  0359 9f            	ld	a,xl
1665  035a 5e            	swapw	x
1666  035b 9096          	ldw	y,sp
1667  035d 72a90111      	addw	y,#OFST+0
1668  0361 90fb          	add	a,(y)
1669  0363 2401          	jrnc	L64
1670  0365 5c            	incw	x
1671  0366               L64:
1672  0366 02            	rlwa	x,a
1673  0367 a620          	ld	a,#32
1674  0369 f7            	ld	(x),a
1675                     ; 521 	for (i=0;i<NbSpace;i++)
1677  036a 96            	ldw	x,sp
1678  036b 1c0111        	addw	x,#OFST+0
1679  036e 7c            	inc	(x)
1680  036f               L375:
1683  036f 96            	ldw	x,sp
1684  0370 1c0111        	addw	x,#OFST+0
1685  0373 f6            	ld	a,(x)
1686  0374 1111          	cp	a,(OFST-256,sp)
1687  0376 25dd          	jrult	L765
1688                     ; 524 	for (i=0;i<NbCar;i++)
1690  0378 96            	ldw	x,sp
1691  0379 1c0111        	addw	x,#OFST+0
1692  037c 7f            	clr	(x)
1694  037d 2030          	jra	L306
1695  037f               L775:
1696                     ; 525 			Buffer[i+NbSpace] = StringToConvert[i];
1698  037f 96            	ldw	x,sp
1699  0380 1c0012        	addw	x,#OFST-255
1700  0383 1f0f          	ldw	(OFST-258,sp),x
1701  0385 96            	ldw	x,sp
1702  0386 1c0111        	addw	x,#OFST+0
1703  0389 f6            	ld	a,(x)
1704  038a 5f            	clrw	x
1705  038b 1b11          	add	a,(OFST-256,sp)
1706  038d 2401          	jrnc	L05
1707  038f 5c            	incw	x
1708  0390               L05:
1709  0390 02            	rlwa	x,a
1710  0391 72fb0f        	addw	x,(OFST-258,sp)
1711  0394 89            	pushw	x
1712  0395 96            	ldw	x,sp
1713  0396 1c0117        	addw	x,#OFST+6
1714  0399 fe            	ldw	x,(x)
1715  039a 01            	rrwa	x,a
1716  039b 9096          	ldw	y,sp
1717  039d 72a90113      	addw	y,#OFST+2
1718  03a1 90fb          	add	a,(y)
1719  03a3 2401          	jrnc	L25
1720  03a5 5c            	incw	x
1721  03a6               L25:
1722  03a6 02            	rlwa	x,a
1723  03a7 f6            	ld	a,(x)
1724  03a8 85            	popw	x
1725  03a9 f7            	ld	(x),a
1726                     ; 524 	for (i=0;i<NbCar;i++)
1728  03aa 96            	ldw	x,sp
1729  03ab 1c0111        	addw	x,#OFST+0
1730  03ae 7c            	inc	(x)
1731  03af               L306:
1734  03af 96            	ldw	x,sp
1735  03b0 1c0111        	addw	x,#OFST+0
1736  03b3 f6            	ld	a,(x)
1737  03b4 96            	ldw	x,sp
1738  03b5 1c0112        	addw	x,#OFST+1
1739  03b8 f1            	cp	a,(x)
1740  03b9 25c4          	jrult	L775
1741                     ; 527 	for (i=0;i<NbCar+NbSpace;i++)
1743  03bb 96            	ldw	x,sp
1744  03bc 1c0111        	addw	x,#OFST+0
1745  03bf 7f            	clr	(x)
1747  03c0 cc044a        	jra	L316
1748  03c3               L706:
1749                     ; 529 		if (Buffer[i] >= 0x61 && Buffer[i] <= 0x7A)
1751  03c3 96            	ldw	x,sp
1752  03c4 1c0012        	addw	x,#OFST-255
1753  03c7 9f            	ld	a,xl
1754  03c8 5e            	swapw	x
1755  03c9 9096          	ldw	y,sp
1756  03cb 72a90111      	addw	y,#OFST+0
1757  03cf 90fb          	add	a,(y)
1758  03d1 2401          	jrnc	L45
1759  03d3 5c            	incw	x
1760  03d4               L45:
1761  03d4 02            	rlwa	x,a
1762  03d5 f6            	ld	a,(x)
1763  03d6 a161          	cp	a,#97
1764  03d8 2543          	jrult	L716
1766  03da 96            	ldw	x,sp
1767  03db 1c0012        	addw	x,#OFST-255
1768  03de 9f            	ld	a,xl
1769  03df 5e            	swapw	x
1770  03e0 9096          	ldw	y,sp
1771  03e2 72a90111      	addw	y,#OFST+0
1772  03e6 90fb          	add	a,(y)
1773  03e8 2401          	jrnc	L65
1774  03ea 5c            	incw	x
1775  03eb               L65:
1776  03eb 02            	rlwa	x,a
1777  03ec f6            	ld	a,(x)
1778  03ed a17b          	cp	a,#123
1779  03ef 242c          	jruge	L716
1780                     ; 530 			StringToConvert[i] = Buffer[i]-32;
1782  03f1 96            	ldw	x,sp
1783  03f2 1c0115        	addw	x,#OFST+4
1784  03f5 fe            	ldw	x,(x)
1785  03f6 01            	rrwa	x,a
1786  03f7 9096          	ldw	y,sp
1787  03f9 72a90111      	addw	y,#OFST+0
1788  03fd 90fb          	add	a,(y)
1789  03ff 2401          	jrnc	L06
1790  0401 5c            	incw	x
1791  0402               L06:
1792  0402 02            	rlwa	x,a
1793  0403 89            	pushw	x
1794  0404 96            	ldw	x,sp
1795  0405 1c0014        	addw	x,#OFST-253
1796  0408 9f            	ld	a,xl
1797  0409 5e            	swapw	x
1798  040a 9096          	ldw	y,sp
1799  040c 72a90113      	addw	y,#OFST+2
1800  0410 90fb          	add	a,(y)
1801  0412 2401          	jrnc	L26
1802  0414 5c            	incw	x
1803  0415               L26:
1804  0415 02            	rlwa	x,a
1805  0416 f6            	ld	a,(x)
1806  0417 a020          	sub	a,#32
1807  0419 85            	popw	x
1808  041a f7            	ld	(x),a
1810  041b 2028          	jra	L126
1811  041d               L716:
1812                     ; 532 			StringToConvert[i] = Buffer[i];
1814  041d 96            	ldw	x,sp
1815  041e 1c0115        	addw	x,#OFST+4
1816  0421 fe            	ldw	x,(x)
1817  0422 01            	rrwa	x,a
1818  0423 9096          	ldw	y,sp
1819  0425 72a90111      	addw	y,#OFST+0
1820  0429 90fb          	add	a,(y)
1821  042b 2401          	jrnc	L46
1822  042d 5c            	incw	x
1823  042e               L46:
1824  042e 02            	rlwa	x,a
1825  042f 89            	pushw	x
1826  0430 96            	ldw	x,sp
1827  0431 1c0014        	addw	x,#OFST-253
1828  0434 9f            	ld	a,xl
1829  0435 5e            	swapw	x
1830  0436 9096          	ldw	y,sp
1831  0438 72a90113      	addw	y,#OFST+2
1832  043c 90fb          	add	a,(y)
1833  043e 2401          	jrnc	L66
1834  0440 5c            	incw	x
1835  0441               L66:
1836  0441 02            	rlwa	x,a
1837  0442 f6            	ld	a,(x)
1838  0443 85            	popw	x
1839  0444 f7            	ld	(x),a
1840  0445               L126:
1841                     ; 527 	for (i=0;i<NbCar+NbSpace;i++)
1843  0445 96            	ldw	x,sp
1844  0446 1c0111        	addw	x,#OFST+0
1845  0449 7c            	inc	(x)
1846  044a               L316:
1849  044a 9c            	rvf
1850  044b 96            	ldw	x,sp
1851  044c 1c0111        	addw	x,#OFST+0
1852  044f f6            	ld	a,(x)
1853  0450 5f            	clrw	x
1854  0451 97            	ld	xl,a
1855  0452 1f0f          	ldw	(OFST-258,sp),x
1856  0454 96            	ldw	x,sp
1857  0455 1c0112        	addw	x,#OFST+1
1858  0458 f6            	ld	a,(x)
1859  0459 5f            	clrw	x
1860  045a 1b11          	add	a,(OFST-256,sp)
1861  045c 2401          	jrnc	L07
1862  045e 5c            	incw	x
1863  045f               L07:
1864  045f 02            	rlwa	x,a
1865  0460 130f          	cpw	x,(OFST-258,sp)
1866  0462 2d03          	jrsle	L67
1867  0464 cc03c3        	jp	L706
1868  0467               L67:
1869                     ; 534 	StringToConvert[NbCar+NbSpace] = ' ';
1871  0467 96            	ldw	x,sp
1872  0468 1c0112        	addw	x,#OFST+1
1873  046b f6            	ld	a,(x)
1874  046c 5f            	clrw	x
1875  046d 1b11          	add	a,(OFST-256,sp)
1876  046f 2401          	jrnc	L27
1877  0471 5c            	incw	x
1878  0472               L27:
1879  0472 02            	rlwa	x,a
1880  0473 9096          	ldw	y,sp
1881  0475 72a90115      	addw	y,#OFST+4
1882  0479 90fe          	ldw	y,(y)
1883  047b 90bf00        	ldw	c_x,y
1884  047e 72bb0000      	addw	x,c_x
1885  0482 a620          	ld	a,#32
1886  0484 f7            	ld	(x),a
1887                     ; 535 	StringToConvert[NbCar+NbSpace+1] = 0;
1889  0485 96            	ldw	x,sp
1890  0486 1c0112        	addw	x,#OFST+1
1891  0489 f6            	ld	a,(x)
1892  048a 5f            	clrw	x
1893  048b 1b11          	add	a,(OFST-256,sp)
1894  048d 2401          	jrnc	L47
1895  048f 5c            	incw	x
1896  0490               L47:
1897  0490 02            	rlwa	x,a
1898  0491 9096          	ldw	y,sp
1899  0493 72a90115      	addw	y,#OFST+4
1900  0497 90fe          	ldw	y,(y)
1901  0499 90bf00        	ldw	c_x,y
1902  049c 72bb0000      	addw	x,c_x
1903  04a0 6f01          	clr	(1,x)
1904                     ; 537 }
1907  04a2 5bff          	addw	sp,#255
1908  04a4 5b13          	addw	sp,#19
1909  04a6 81            	ret
1933                     ; 544 static void DeInitClock ( void )
1933                     ; 545 {
1934                     	switch	.text
1935  04a7               L71_DeInitClock:
1939                     ; 546 	CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, DISABLE);
1941  04a7 5f            	clrw	x
1942  04a8 cd0000        	call	_CLK_PeripheralClockConfig
1944                     ; 547 	CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, DISABLE);
1946  04ab ae0100        	ldw	x,#256
1947  04ae cd0000        	call	_CLK_PeripheralClockConfig
1949                     ; 548 	CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, DISABLE);
1951  04b1 ae0200        	ldw	x,#512
1952  04b4 cd0000        	call	_CLK_PeripheralClockConfig
1954                     ; 549 	CLK_PeripheralClockConfig(CLK_Peripheral_I2C1, DISABLE);
1956  04b7 ae0300        	ldw	x,#768
1957  04ba cd0000        	call	_CLK_PeripheralClockConfig
1959                     ; 550 	CLK_PeripheralClockConfig(CLK_Peripheral_SPI1, DISABLE);
1961  04bd ae0400        	ldw	x,#1024
1962  04c0 cd0000        	call	_CLK_PeripheralClockConfig
1964                     ; 551 	CLK_PeripheralClockConfig(CLK_Peripheral_USART1, DISABLE);
1966  04c3 ae0500        	ldw	x,#1280
1967  04c6 cd0000        	call	_CLK_PeripheralClockConfig
1969                     ; 552 	CLK_PeripheralClockConfig(CLK_Peripheral_BEEP, DISABLE);
1971  04c9 ae0600        	ldw	x,#1536
1972  04cc cd0000        	call	_CLK_PeripheralClockConfig
1974                     ; 553 	CLK_PeripheralClockConfig(CLK_Peripheral_DAC, DISABLE);
1976  04cf ae0700        	ldw	x,#1792
1977  04d2 cd0000        	call	_CLK_PeripheralClockConfig
1979                     ; 554 	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
1981  04d5 ae1000        	ldw	x,#4096
1982  04d8 cd0000        	call	_CLK_PeripheralClockConfig
1984                     ; 555 	CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, DISABLE);
1986  04db ae1100        	ldw	x,#4352
1987  04de cd0000        	call	_CLK_PeripheralClockConfig
1989                     ; 556 	CLK_PeripheralClockConfig(CLK_Peripheral_RTC, DISABLE);
1991  04e1 ae1200        	ldw	x,#4608
1992  04e4 cd0000        	call	_CLK_PeripheralClockConfig
1994                     ; 557 	CLK_PeripheralClockConfig(CLK_Peripheral_LCD, DISABLE);
1996  04e7 ae1300        	ldw	x,#4864
1997  04ea cd0000        	call	_CLK_PeripheralClockConfig
1999                     ; 558 	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
2001  04ed ae1000        	ldw	x,#4096
2002  04f0 cd0000        	call	_CLK_PeripheralClockConfig
2004                     ; 559 	CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, DISABLE);
2006  04f3 ae1400        	ldw	x,#5120
2007  04f6 cd0000        	call	_CLK_PeripheralClockConfig
2009                     ; 560 	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
2011  04f9 ae1000        	ldw	x,#4096
2012  04fc cd0000        	call	_CLK_PeripheralClockConfig
2014                     ; 561 	CLK_PeripheralClockConfig(CLK_Peripheral_BOOTROM, DISABLE);
2016  04ff ae1700        	ldw	x,#5888
2017  0502 cd0000        	call	_CLK_PeripheralClockConfig
2019                     ; 562 	CLK_PeripheralClockConfig(CLK_Peripheral_AES, DISABLE);
2021  0505 ae2000        	ldw	x,#8192
2022  0508 cd0000        	call	_CLK_PeripheralClockConfig
2024                     ; 563 	CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, DISABLE);
2026  050b ae1000        	ldw	x,#4096
2027  050e cd0000        	call	_CLK_PeripheralClockConfig
2029                     ; 564 	CLK_PeripheralClockConfig(CLK_Peripheral_TIM5, DISABLE);
2031  0511 ae2100        	ldw	x,#8448
2032  0514 cd0000        	call	_CLK_PeripheralClockConfig
2034                     ; 565 	CLK_PeripheralClockConfig(CLK_Peripheral_SPI2, DISABLE);
2036  0517 ae2200        	ldw	x,#8704
2037  051a cd0000        	call	_CLK_PeripheralClockConfig
2039                     ; 566 	CLK_PeripheralClockConfig(CLK_Peripheral_USART2, DISABLE);
2041  051d ae2300        	ldw	x,#8960
2042  0520 cd0000        	call	_CLK_PeripheralClockConfig
2044                     ; 567 	CLK_PeripheralClockConfig(CLK_Peripheral_USART3, DISABLE);
2046  0523 ae2400        	ldw	x,#9216
2047  0526 cd0000        	call	_CLK_PeripheralClockConfig
2049                     ; 568 	CLK_PeripheralClockConfig(CLK_Peripheral_CSSLSE, DISABLE);
2051  0529 ae2500        	ldw	x,#9472
2052  052c cd0000        	call	_CLK_PeripheralClockConfig
2054                     ; 570 }
2057  052f 81            	ret
2081                     ; 576 static void DeInitGPIO ( void )
2081                     ; 577 {
2082                     	switch	.text
2083  0530               L12_DeInitGPIO:
2087                     ; 579 	GPIO_Init( GPIOA, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
2089  0530 4ba0          	push	#160
2090  0532 4bff          	push	#255
2091  0534 ae5000        	ldw	x,#20480
2092  0537 cd0000        	call	_GPIO_Init
2094  053a 85            	popw	x
2095                     ; 580 	GPIO_Init( GPIOB, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
2097  053b 4ba0          	push	#160
2098  053d 4bff          	push	#255
2099  053f ae5005        	ldw	x,#20485
2100  0542 cd0000        	call	_GPIO_Init
2102  0545 85            	popw	x
2103                     ; 582   GPIO_Init( GPIOC, GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 |\
2103                     ; 583 	           GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7, GPIO_Mode_Out_OD_Low_Fast);
2105  0546 4ba0          	push	#160
2106  0548 4bfc          	push	#252
2107  054a ae500a        	ldw	x,#20490
2108  054d cd0000        	call	_GPIO_Init
2110  0550 85            	popw	x
2111                     ; 584 	GPIO_Init( GPIOD, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
2113  0551 4ba0          	push	#160
2114  0553 4bff          	push	#255
2115  0555 ae500f        	ldw	x,#20495
2116  0558 cd0000        	call	_GPIO_Init
2118  055b 85            	popw	x
2119                     ; 585 	GPIO_Init( GPIOE, GPIO_Pin_All, GPIO_Mode_Out_OD_Low_Fast);
2121  055c 4ba0          	push	#160
2122  055e 4bff          	push	#255
2123  0560 ae5014        	ldw	x,#20500
2124  0563 cd0000        	call	_GPIO_Init
2126  0566 85            	popw	x
2127                     ; 587 	GPIOA->ODR = 0xFF;
2129  0567 35ff5000      	mov	20480,#255
2130                     ; 588 	GPIOB->ODR = 0xFF;
2132  056b 35ff5005      	mov	20485,#255
2133                     ; 589 	GPIOC->ODR = 0xFF;
2135  056f 35ff500a      	mov	20490,#255
2136                     ; 590 	GPIOD->ODR = 0xFF;
2138  0573 35ff500f      	mov	20495,#255
2139                     ; 591 	GPIOE->ODR = 0xFF;
2141  0577 35ff5014      	mov	20500,#255
2142                     ; 593 }
2145  057b 81            	ret
2189                     ; 601 static void InitializeBuffer (uint8_t *Buffer ,uint8_t NbCar)
2189                     ; 602 {
2190                     	switch	.text
2191  057c               L72_InitializeBuffer:
2193  057c 89            	pushw	x
2194       00000000      OFST:	set	0
2197  057d               L566:
2198                     ; 606 		Buffer[NbCar]= 0;
2200  057d 7b01          	ld	a,(OFST+1,sp)
2201  057f 97            	ld	xl,a
2202  0580 7b02          	ld	a,(OFST+2,sp)
2203  0582 1b05          	add	a,(OFST+5,sp)
2204  0584 2401          	jrnc	L601
2205  0586 5c            	incw	x
2206  0587               L601:
2207  0587 02            	rlwa	x,a
2208  0588 7f            	clr	(x)
2209                     ; 607 	}	while (NbCar--);
2211  0589 7b05          	ld	a,(OFST+5,sp)
2212  058b 0a05          	dec	(OFST+5,sp)
2213  058d 4d            	tnz	a
2214  058e 26ed          	jrne	L566
2215                     ; 608 }
2218  0590 85            	popw	x
2219  0591 81            	ret
2318                     	xdef	_main
2319                     	xdef	_FirmwareVersion
2320                     	xdef	_EEInitial
2321                     	switch	.eeprom
2322  0001               _EEMenuState:
2323  0001 00            	ds.b	1
2324                     	xdef	_EEMenuState
2325                     	xdef	_ErrorMessage
2326                     	switch	.ubsct
2327  0000               _NDEFmessage:
2328  0000 000000000000  	ds.b	64
2329                     	xdef	_NDEFmessage
2330                     	xref.b	_t_bar
2331  0040               _state_machine:
2332  0040 00            	ds.b	1
2333                     	xdef	_state_machine
2334                     	xref	_delayLFO_ms
2335                     	xref	_Display_Ram
2336                     	xref	_Vref_measure
2337                     	xref	__fctcpy
2338                     	xref	_I2C_SS_ReadOneByte
2339                     	xref	_I2C_SS_Config
2340                     	xref	_I2C_SS_Init
2341                     	xref	_M24LR04E_WriteOneByte
2342                     	xref	_M24LR04E_ReadBuffer
2343                     	xref	_M24LR04E_ReadOneByte
2344                     	xref	_M24LR04E_Init
2345                     	xref	_M24LR04E_DeInit
2346                     	xref	_I2C_Cmd
2347                     	xref	_LCD_GLASS_ScrollSentenceNbCar
2348                     	xref	_LCD_GLASS_Clear
2349                     	xref	_LCD_GLASS_DisplayStrDeci
2350                     	xref	_LCD_GLASS_DisplayString
2351                     	xref	_LCD_GLASS_Init
2352                     	xref	_GPIO_Init
2353                     	xref	_FLASH_Unlock
2354                     	xref	_EXTI_SetPinSensitivity
2355                     	xref	_CLK_PeripheralClockConfig
2356                     	xref	_CLK_SYSCLKSourceSwitchCmd
2357                     	xref	_CLK_SYSCLKDivConfig
2358                     	xref	_CLK_SYSCLKSourceConfig
2359                     	xref	_CLK_HSICmd
2360                     	switch	.const
2361  002b               L131:
2362  002b 4572726f7200  	dc.b	"Error",0
2363                     	xref.b	c_x
2383                     	xref	c_sdivx
2384                     	xref	c_smodx
2385                     	xref	c_eewrc
2386                     	end
