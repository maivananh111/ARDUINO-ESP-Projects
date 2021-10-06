#ifndef __LCD1280
#define __LCD1280
#include <Arduino.h>
#include "font.h"
// прототипы функций

//const G_Font *gfxFont;
//uint8_t Width, Height, VAL;
class LCD1280 {
  public:
       LCD1280(uint8_t _Clock, uint8_t _Data, uint8_t _CS, uint8_t _RES);
       void Init();
       void Clear();
       void Update();
       void drawPixel(uint8_t x, uint8_t y, boolean color);
       void fillScreen(boolean color);
       void drawChar(uint8_t x, uint8_t y, boolean color, uint8_t c);
       void print(uint8_t x, uint8_t y, boolean color, char *str);
	     void print(uint8_t x, uint8_t y, boolean color, long num);
	     void print_1607(uint8_t x, uint8_t y, boolean color, char *str);
       void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, boolean color);
       void drawFastVLine(uint8_t x, uint8_t y, uint8_t h, boolean color);
       void drawFastHLine(uint8_t x, uint8_t y, uint8_t w, boolean color);
       void drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, boolean color);
       void drawCircle(uint8_t x0, uint8_t y0, int16_t r, boolean color);
       void drawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, boolean color);
       void drawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, boolean color);
       void drawCircleHelper(uint8_t x0, uint8_t y0, uint8_t r, uint8_t cornername, boolean color);
       void fillCircle(uint8_t x0, uint8_t y0, uint8_t r, boolean color);
       void fillCircleHelper(uint8_t x0, uint8_t y0, uint8_t r, uint8_t cornername, uint8_t delta, boolean color);
       void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, boolean color);
       void fillRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, boolean color);
       void fillTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, boolean color);
       void drawBitmap(uint8_t x, uint8_t y, const char *bitmap, uint8_t w, uint8_t h, boolean color);
       void Write(uint8_t x, uint8_t y, bool color, uint8_t data);
       void WriteNByte(uint8_t N, uint8_t x, uint8_t y, const uint8_t *Font, bool color, uint16_t num);
       void DrawChar(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *Font, bool color, uint8_t Char);
       void print(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *Font, boolean color, char *str);
       void write(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *Font, boolean color, int16_t num);
       void write(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *Font, boolean color, float num);
  private:
       void dWrite(uint8_t pin, uint8_t val);
       void Sendbyte(uint8_t mode, uint8_t c);
	     
};
#endif
