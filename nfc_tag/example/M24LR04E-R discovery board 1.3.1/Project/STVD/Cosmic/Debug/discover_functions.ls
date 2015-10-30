   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.10.2 - 02 Nov 2011
   3                     ; Generator (Limited) V4.3.7 - 29 Nov 2011
  17                     .dataeeprom:	section	.bss
  18  0000               _Bias_Current:
  19  0000 00            	ds.b	1
  64                     ; 57 void FLASH_ProgramBias(uint8_t Data)
  64                     ; 58 {
  66                     	switch	.text
  67  0000               _FLASH_ProgramBias:
  69  0000 88            	push	a
  70       00000000      OFST:	set	0
  73                     ; 59   FLASH_Unlock(FLASH_MemType_Data);
  75  0001 a6f7          	ld	a,#247
  76  0003 cd0000        	call	_FLASH_Unlock
  78                     ; 60   Bias_Current = Data;
  80  0006 7b01          	ld	a,(OFST+1,sp)
  81  0008 c70000        	ld	_Bias_Current,a
  82                     ; 61   FLASH_WaitForLastOperation(FLASH_MemType_Data);
  84  000b a6f7          	ld	a,#247
  85  000d cd0000        	call	_FLASH_WaitForLastOperation
  87                     ; 62   FLASH_Lock(FLASH_MemType_Data);
  89  0010 a6f7          	ld	a,#247
  90  0012 cd0000        	call	_FLASH_Lock
  92                     ; 63 }	
  95  0015 84            	pop	a
  96  0016 81            	ret
 185                     ; 359 void convert_into_char(uint16_t number, uint16_t *p_tab)
 185                     ; 360 {
 186                     	switch	.text
 187  0017               _convert_into_char:
 189  0017 89            	pushw	x
 190  0018 520a          	subw	sp,#10
 191       0000000a      OFST:	set	10
 194                     ; 361   uint16_t units=0, tens=0, hundreds=0, thousands=0, misc=0;
 204                     ; 363   units = (((number%10000)%1000)%100)%10;
 206  001a 90ae2710      	ldw	y,#10000
 207  001e 65            	divw	x,y
 208  001f 51            	exgw	x,y
 209  0020 90ae03e8      	ldw	y,#1000
 210  0024 65            	divw	x,y
 211  0025 51            	exgw	x,y
 212  0026 90ae0064      	ldw	y,#100
 213  002a 65            	divw	x,y
 214  002b 51            	exgw	x,y
 215  002c 90ae000a      	ldw	y,#10
 216  0030 65            	divw	x,y
 217  0031 51            	exgw	x,y
 218  0032 1f09          	ldw	(OFST-1,sp),x
 219                     ; 364   tens = ((((number-units)/10)%1000)%100)%10;
 221  0034 1e0b          	ldw	x,(OFST+1,sp)
 222  0036 72f009        	subw	x,(OFST-1,sp)
 223  0039 90ae000a      	ldw	y,#10
 224  003d 65            	divw	x,y
 225  003e 90ae03e8      	ldw	y,#1000
 226  0042 65            	divw	x,y
 227  0043 51            	exgw	x,y
 228  0044 90ae0064      	ldw	y,#100
 229  0048 65            	divw	x,y
 230  0049 51            	exgw	x,y
 231  004a 90ae000a      	ldw	y,#10
 232  004e 65            	divw	x,y
 233  004f 51            	exgw	x,y
 234  0050 1f07          	ldw	(OFST-3,sp),x
 235                     ; 365   hundreds = (((number-tens-units)/100))%100%10;
 237  0052 1e0b          	ldw	x,(OFST+1,sp)
 238  0054 72f007        	subw	x,(OFST-3,sp)
 239  0057 72f009        	subw	x,(OFST-1,sp)
 240  005a 90ae0064      	ldw	y,#100
 241  005e 65            	divw	x,y
 242  005f 90ae0064      	ldw	y,#100
 243  0063 65            	divw	x,y
 244  0064 51            	exgw	x,y
 245  0065 90ae000a      	ldw	y,#10
 246  0069 65            	divw	x,y
 247  006a 51            	exgw	x,y
 248  006b 1f05          	ldw	(OFST-5,sp),x
 249                     ; 366   thousands = ((number-hundreds-tens-units)/1000)%10;
 251  006d 1e0b          	ldw	x,(OFST+1,sp)
 252  006f 72f005        	subw	x,(OFST-5,sp)
 253  0072 72f007        	subw	x,(OFST-3,sp)
 254  0075 72f009        	subw	x,(OFST-1,sp)
 255  0078 90ae03e8      	ldw	y,#1000
 256  007c 65            	divw	x,y
 257  007d 90ae000a      	ldw	y,#10
 258  0081 65            	divw	x,y
 259  0082 51            	exgw	x,y
 260  0083 1f03          	ldw	(OFST-7,sp),x
 261                     ; 367   misc = ((number-thousands-hundreds-tens-units)/10000);
 263  0085 1e0b          	ldw	x,(OFST+1,sp)
 264  0087 72f003        	subw	x,(OFST-7,sp)
 265  008a 72f005        	subw	x,(OFST-5,sp)
 266  008d 72f007        	subw	x,(OFST-3,sp)
 267  0090 72f009        	subw	x,(OFST-1,sp)
 268  0093 90ae2710      	ldw	y,#10000
 269  0097 65            	divw	x,y
 270  0098 1f01          	ldw	(OFST-9,sp),x
 271                     ; 369   *(p_tab+4) = units + 0x30;
 273  009a 1e09          	ldw	x,(OFST-1,sp)
 274  009c 1c0030        	addw	x,#48
 275  009f 160f          	ldw	y,(OFST+5,sp)
 276  00a1 90ef08        	ldw	(8,y),x
 277                     ; 370   *(p_tab+3) = tens + 0x30;
 279  00a4 1e07          	ldw	x,(OFST-3,sp)
 280  00a6 1c0030        	addw	x,#48
 281  00a9 160f          	ldw	y,(OFST+5,sp)
 282  00ab 90ef06        	ldw	(6,y),x
 283                     ; 371   *(p_tab+2) = hundreds + 0x30;
 285  00ae 1e05          	ldw	x,(OFST-5,sp)
 286  00b0 1c0030        	addw	x,#48
 287  00b3 160f          	ldw	y,(OFST+5,sp)
 288  00b5 90ef04        	ldw	(4,y),x
 289                     ; 372   *(p_tab+1) = thousands + 0x30;
 291  00b8 1e03          	ldw	x,(OFST-7,sp)
 292  00ba 1c0030        	addw	x,#48
 293  00bd 160f          	ldw	y,(OFST+5,sp)
 294  00bf 90ef02        	ldw	(2,y),x
 295                     ; 373   *(p_tab) = misc + 0x30;
 297  00c2 1e01          	ldw	x,(OFST-9,sp)
 298  00c4 1c0030        	addw	x,#48
 299  00c7 160f          	ldw	y,(OFST+5,sp)
 300  00c9 90ff          	ldw	(y),x
 301                     ; 375 }
 304  00cb 5b0c          	addw	sp,#12
 305  00cd 81            	ret
 351                     ; 442 void	Display_Ram(void)
 351                     ; 443 #endif
 351                     ; 444 { 
 352                     .DISPLAY:	section	.text
 353  0000               _Display_Ram:
 355  0000 89            	pushw	x
 356       00000002      OFST:	set	2
 359                     ; 445   uint8_t NbCar = 0;
 361  0001 0f02          	clr	(OFST+0,sp)
 362                     ; 446   uint8_t i = 0;
 364  0003 0f01          	clr	(OFST-1,sp)
 365                     ; 455   FLASH->CR1 = 0x08;
 367  0005 35085050      	mov	20560,#8
 369  0009               L321:
 370                     ; 456   while(((CLK->REGCSR)&0x80)==0x80);
 372  0009 c650cf        	ld	a,20687
 373  000c a480          	and	a,#128
 374  000e a180          	cp	a,#128
 375  0010 27f7          	jreq	L321
 376                     ; 469   WFE->CR2 = 0x08;
 378  0012 350850a7      	mov	20647,#8
 379                     ; 470   GPIOC->CR2 = 0x80;
 381  0016 3580500e      	mov	20494,#128
 383  001a 2006          	jra	L331
 384  001c               L721:
 385                     ; 479 	NbCar +=3;
 387  001c 7b02          	ld	a,(OFST+0,sp)
 388  001e ab03          	add	a,#3
 389  0020 6b02          	ld	(OFST+0,sp),a
 390  0022               L331:
 391                     ; 477 	while(NDEFmessage[NbCar++] != 0)
 393  0022 7b02          	ld	a,(OFST+0,sp)
 394  0024 97            	ld	xl,a
 395  0025 0c02          	inc	(OFST+0,sp)
 396  0027 9f            	ld	a,xl
 397  0028 5f            	clrw	x
 398  0029 97            	ld	xl,a
 399  002a 6d00          	tnz	(_NDEFmessage,x)
 400  002c 26ee          	jrne	L721
 401                     ; 480 	LCD_GLASS_ScrollSentenceNbCarLP(NDEFmessage, NbCar);
 403  002e 7b02          	ld	a,(OFST+0,sp)
 404  0030 88            	push	a
 405  0031 ae0000        	ldw	x,#_NDEFmessage
 406  0034 cd0000        	call	_LCD_GLASS_ScrollSentenceNbCarLP
 408  0037 84            	pop	a
 409                     ; 483   wfe();
 412  0038 728f          wfe
 414                     ; 485   EXTI->SR1 |= 0x40;
 417  003a 721c50a3      	bset	20643,#6
 418                     ; 486   WFE->CR2 = 0x00;
 420  003e 725f50a7      	clr	20647
 421                     ; 489   CLK->REGCSR = 0x00;
 423  0042 725f50cf      	clr	20687
 425  0046               L341:
 426                     ; 490   while(((CLK->REGCSR)&0x1) != 0x1);		
 428  0046 c650cf        	ld	a,20687
 429  0049 a401          	and	a,#1
 430  004b a101          	cp	a,#1
 431  004d 26f7          	jrne	L341
 432                     ; 493 }
 435  004f 85            	popw	x
 436  0050 81            	ret
 490                     ; 572 float Vdd_appli(void)
 490                     ; 573 {
 491                     	switch	.text
 492  00ce               _Vdd_appli:
 494  00ce 520c          	subw	sp,#12
 495       0000000c      OFST:	set	12
 498                     ; 578   P_VREFINT_Factory = VREFINT_Factory_CONV_ADDRESS;
 500                     ; 581   MeasurINT = ADC_Supply();	
 502  00d0 cd0000        	call	_ADC_Supply
 504  00d3 1f07          	ldw	(OFST-5,sp),x
 505                     ; 603     f_Vdd_appli = (VREF/MeasurINT) * ADC_CONV;
 507  00d5 1e07          	ldw	x,(OFST-5,sp)
 508  00d7 cd0000        	call	c_uitof
 510  00da 96            	ldw	x,sp
 511  00db 1c0001        	addw	x,#OFST-11
 512  00de cd0000        	call	c_rtol
 514  00e1 ae0004        	ldw	x,#L102
 515  00e4 cd0000        	call	c_ltor
 517  00e7 96            	ldw	x,sp
 518  00e8 1c0001        	addw	x,#OFST-11
 519  00eb cd0000        	call	c_fdiv
 521  00ee ae0000        	ldw	x,#L112
 522  00f1 cd0000        	call	c_fmul
 524  00f4 96            	ldw	x,sp
 525  00f5 1c0009        	addw	x,#OFST-3
 526  00f8 cd0000        	call	c_rtol
 528                     ; 607   f_Vdd_appli *= 1000L;
 530  00fb ae03e8        	ldw	x,#1000
 531  00fe cd0000        	call	c_itof
 533  0101 96            	ldw	x,sp
 534  0102 1c0009        	addw	x,#OFST-3
 535  0105 cd0000        	call	c_fgmul
 537                     ; 609   return f_Vdd_appli;
 539  0108 96            	ldw	x,sp
 540  0109 1c0009        	addw	x,#OFST-3
 541  010c cd0000        	call	c_ltor
 545  010f 5b0c          	addw	sp,#12
 546  0111 81            	ret
 593                     ; 618 uint16_t Vref_measure(void)
 593                     ; 619 {
 594                     	switch	.text
 595  0112               _Vref_measure:
 597  0112 520e          	subw	sp,#14
 598       0000000e      OFST:	set	14
 601                     ; 623   Vdd_mV = (uint16_t)Vdd_appli();
 603  0114 adb8          	call	_Vdd_appli
 605  0116 cd0000        	call	c_ftoi
 607  0119 1f01          	ldw	(OFST-13,sp),x
 608                     ; 625   convert_into_char (Vdd_mV, tab);
 610  011b 96            	ldw	x,sp
 611  011c 1c0003        	addw	x,#OFST-11
 612  011f 89            	pushw	x
 613  0120 1e03          	ldw	x,(OFST-11,sp)
 614  0122 cd0017        	call	_convert_into_char
 616  0125 85            	popw	x
 617                     ; 628   tab[5] = 'V';
 619  0126 ae0056        	ldw	x,#86
 620  0129 1f0d          	ldw	(OFST-1,sp),x
 621                     ; 629   tab[4] = ' ';
 623  012b ae0020        	ldw	x,#32
 624  012e 1f0b          	ldw	(OFST-3,sp),x
 625                     ; 630   tab[1] |= DOT; /* To add decimal point for display in volt */
 627  0130 7b05          	ld	a,(OFST-9,sp)
 628  0132 aa80          	or	a,#128
 629  0134 6b05          	ld	(OFST-9,sp),a
 630                     ; 631   tab[0] = ' ';
 632  0136 ae0020        	ldw	x,#32
 633  0139 1f03          	ldw	(OFST-11,sp),x
 634                     ; 633   LCD_GLASS_DisplayStrDeci(tab);
 636  013b 96            	ldw	x,sp
 637  013c 1c0003        	addw	x,#OFST-11
 638  013f cd0000        	call	_LCD_GLASS_DisplayStrDeci
 640                     ; 635   return Vdd_mV;
 642  0142 1e01          	ldw	x,(OFST-13,sp)
 645  0144 5b0e          	addw	sp,#14
 646  0146 81            	ret
 670                     	xdef	_Bias_Current
 671                     	xref.b	_NDEFmessage
 672                     	xref	_LCD_GLASS_ScrollSentenceNbCarLP
 673                     	xref	_LCD_GLASS_DisplayStrDeci
 674                     	xdef	_Display_Ram
 675                     	xdef	_Vdd_appli
 676                     	xdef	_FLASH_ProgramBias
 677                     	xdef	_Vref_measure
 678                     	xdef	_convert_into_char
 679                     	xref	_FLASH_WaitForLastOperation
 680                     	xref	_FLASH_Lock
 681                     	xref	_FLASH_Unlock
 682                     	xref	_ADC_Supply
 683                     .const:	section	.text
 684  0000               L112:
 685  0000 45800000      	dc.w	17792,0
 686  0004               L102:
 687  0004 3f9cac08      	dc.w	16284,-21496
 688                     	xref.b	c_x
 708                     	xref	c_ftoi
 709                     	xref	c_fgmul
 710                     	xref	c_itof
 711                     	xref	c_fmul
 712                     	xref	c_fdiv
 713                     	xref	c_rtol
 714                     	xref	c_uitof
 715                     	xref	c_ltor
 716                     	end
