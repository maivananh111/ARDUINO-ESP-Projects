/*
 * ili9341.h
 *
 * Created: 02/08/2021 9:07:08 CH
 *  Author: A315-56
 */ 


#ifndef ILI9341_H_
#define ILI9341_H_


#include <stdlib.h>
#include <avr/pgmspace.h>
#include "main.h"

#define  swap(a,b)   {int16_t t = a; a = b; b = t;}
#define  DATA_DDR_M   DDRD   //D7,6,5,4,3,2 to PORTD
#define  DATA_PORT_M  PORTD
#define  DATA_DDR_L   DDRB   //D0,1 to PORTB (D0 to PB0, D1 to PB1)
#define  DATA_PORT_L  PORTB
#define  DATA_PIN_M   PIND
#define  DATA_PIN_L   PINB
#define  COMMAND_DDR  DDRC
#define  COMMAND_PORT PORTC
#define  LCD_CS    3  //Chip Select
#define  LCD_CD    2  //Command/Data
#define  LCD_WR    1  //LCD Write
#define  LCD_RD    0  //LCD Read
#define  LCD_RESET 4  //LCD Reset
#define  RESET_IDLE   COMMAND_PORT    |=(1<<LCD_RESET)
#define  CS_IDLE      COMMAND_PORT    |=(1<<LCD_CS)
#define  WR_IDLE      COMMAND_PORT    |=(1<<LCD_WR)
#define  RD_IDLE      COMMAND_PORT    |=(1<<LCD_RD)
#define  RESET_ACTIVE COMMAND_PORT    &=~(1<<LCD_RESET)
#define  CS_ACTIVE    COMMAND_PORT    &=~(1<<LCD_CS)
#define  WR_ACTIVE    COMMAND_PORT    &=~(1<<LCD_WR)
#define  RD_ACTIVE    COMMAND_PORT    &=~(1<<LCD_RD)
#define  CD_COMMAND   COMMAND_PORT    &=~(1<<LCD_CD)
#define  CD_DATA      COMMAND_PORT    |=(1<<LCD_CD)
#define  WR_STROBE   { WR_ACTIVE; WR_IDLE; }

#define  BLACK   0x0000
#define  BLUE    0x001F
#define  RED     0x0F800
#define  GREEN   0x07E0
#define  CYAN    0x07FF
#define  MAGENTA 0xF81F
#define  YELLOW  0xFFE0
#define  WHITE   0xFFFF

#define  setReadDir()  { DATA_DDR_M = 0x03; DATA_DDR_L = 0xFC; }
#define  setWriteDir() { DATA_DDR_M = 0xFC; DATA_DDR_L = 0x03; }

void TFT9341_ini(void);
void TFT9341_FillScreen(unsigned int color);
void TFT9341_FillRectangle(unsigned int color,unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
unsigned int TFT9341_RandColor(void);
void TFT9341_DrawPixel(int x, int y, unsigned int color);
void TFT9341_DrawLine(unsigned int color,unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void TFT9341_DrawRect(unsigned int color,unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void TFT9341_DrawCircle(unsigned int x0, unsigned int y0, int r, unsigned int color);
void TFT9341_Draw_Char(int x, int y, unsigned int color, unsigned int phone, unsigned char charcode, unsigned char size);
void TFT9341_String(unsigned int x, unsigned int y, unsigned int color, unsigned int phone,
char *str, unsigned char size);



#endif /* ILI9341_H_ */