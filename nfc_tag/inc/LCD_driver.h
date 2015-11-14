
#ifndef __LCD_H
#define __LCD_H

#include "lib_config.h"
   
/* Define for scrolling sentences*/
#define SCROLL_SPEED  	        160
#define SCROLL_SPEED_L          40
#define SCROLL_NUM    	        1

/* Define for character '.' */
#define  POINT_OFF              FALSE
#define  POINT_ON               TRUE

/* Define for caracter ":" */
#define  COLUMN_OFF             FALSE
#define  COLUMN_ON              TRUE

#define DOT                     0x8000 /* for add decimal point in string */
#define DOUBLE_DOT              0x4000 /* for add decimal point in string */

/* Macros used for set/reset bar LCD bar */
#define BAR0_ON  t_bar[0] |= 0x80
#define BAR0_OFF t_bar[0] &= ~0x80
#define BAR1_ON  t_bar[1] |= 0x08
#define BAR1_OFF t_bar[1] &= ~0x08
#define BAR2_ON  t_bar[0] |= 0x20
#define BAR2_OFF t_bar[0] &= ~0x20
#define BAR3_ON  t_bar[1] |= 0x02
#define BAR3_OFF t_bar[1] &= ~0x02

/* code for 'µ' character */
#define C_UMAP 0x6081

/* code for 'm' character */
#define C_mMap 0xb210

/* code for 'n' character */
#define C_nMap 0x2210

/* constant code for '*' character */
#define star 0xA0D7

/* constant code for '-' character */
#define C_minus 0xA000

void LCD_bar(void);
void LCD_GLASS_Init(void);
void LCD_GLASS_WriteChar(uint8_t* ch, bool point, bool column,uint8_t position);
void LCD_GLASS_DisplayString(uint8_t* ptr);
void LCD_GLASS_DisplayStrDeci(uint16_t* ptr);
void LCD_GLASS_ClearChar(uint8_t position);
void LCD_GLASS_Clear(void);
void LCD_GLASS_ScrollSentence(uint8_t* ptr, uint16_t nScroll, uint16_t ScrollSpeed);
void LCD_GLASS_ScrollSentenceNbCar(uint8_t* ptr, uint16_t ScrollSpeed,uint8_t NbCar);
void LCD_GLASS_WriteTime(char a, uint8_t posi, bool column);
void LCD_GLASS_ScrollSentenceNbCarLP(uint8_t* ptr, uint8_t NbCar);

#endif

