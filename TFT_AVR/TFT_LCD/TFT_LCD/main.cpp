/*
 * TFT_LCD.cpp
 *
 * Created: 02/08/2021 9:15:54 CH
 * Author : A315-56
 */ 

#include "main.h"
#include "ili9341.h"
#include "TouchPad.h"

PS2Mouse mouse(MOUSE_CLOCK, MOUSE_DATA, STREAM);
int16_t x = 160, y = 120;

int main(void)
{
    TFT9341_ini();
	TFT9341_FillScreen(BLACK);
	TFT9341_SetRotation(1);
	TFT9341_String(10, 10, YELLOW, BLACK, "Mai Van Anh :)", 1);
	
	mouse.Init();
	
    while (1) {
		char str[20];
		int data[3];
		mouse.report(data);
        x += data[1];
		y -= data[2];
		if(x<0) x=0;
		if(x>320) x=320;
		if(y<0) y=0;
		if(y>240) y=240;
		TFT9341_DrawPixel(x, y, WHITE);
    }
}

