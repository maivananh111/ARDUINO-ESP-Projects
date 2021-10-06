#include <Arduino.h>
#include <avr/pgmspace.h>
#include "lcd1280.h"
//#include "font.h"


#define LCD_X        96
#define LCD_Y        68
#define LCD_String    9

#define W   94
#define H   66

#define SetYAddr   0xB0
#define SetXAddr4  0x00
#define SetXAddr3  0x10

#define pgm     pgm_read_byte
#define swap(a, b) { int t = a; a = b; b = t; }

#define LCD_D         1
#define LCD_C         0

uint8_t LCD_RAM[LCD_X*LCD_String];
volatile uint8_t RES, CS, Data, Clock;

//=====================================================
  LCD1280::LCD1280(uint8_t _Clock, uint8_t _Data, uint8_t _CS, uint8_t _RES){
RES = _RES;
CS = _CS;
Data = _Data;
Clock = _Clock;
}
//======================================================
void LCD1280::Clear() {
  for (int index = 0; index < 864 ; index++){
     LCD_RAM[index] = (0x00);
  }
}
//======================================================= 
void LCD1280::dWrite(uint8_t pin, uint8_t val){
  uint8_t bit = digitalPinToBitMask(pin);
  volatile uint8_t *out;
  out = portOutputRegister(digitalPinToPort(pin));
  (val)? *out |= bit : *out &=~bit;
}

//========================================================= 
void LCD1280::Sendbyte(uint8_t mode, uint8_t c){
  dWrite(CS, 0);
  (mode)? dWrite(Data,1) : dWrite(Data,0);
  dWrite(Clock, 1);
  
  for(uint8_t i=0;i<8;i++){
    dWrite(Clock,0);
    (c & 0x80)? dWrite(Data,1) : dWrite(Data,0);
    dWrite(Clock,1);
    c <<= 1;
  }
  dWrite(Clock, 0);
}
//=========================================================
//=====================================================
void LCD1280::Update(){
  for(uint8_t p = 0; p < 9; p++){
    Sendbyte(LCD_C, SetYAddr| p); 
    Sendbyte(LCD_C, SetXAddr4);
    Sendbyte(LCD_C, SetXAddr3);

    for(uint8_t col=0; col < LCD_X; col++){
      Sendbyte(LCD_D, LCD_RAM[(LCD_X * p) + col]);
    }
  }
}

//===================================================
void LCD1280::Init(){
  pinMode(RES,   OUTPUT);
  pinMode(CS,    OUTPUT);
  pinMode(Data,  OUTPUT);
  pinMode(Clock, OUTPUT);

  dWrite(RES, 1);
  dWrite(Clock, 0);
  dWrite(Data, 0);
  dWrite(CS, 0);
  delay(20);
  dWrite(CS, 1);
  
  Sendbyte(LCD_C,0x2F);           
  Sendbyte(LCD_C,0xA4);   
  Sendbyte(LCD_C,0xAF);           
  Clear();
  Update();
}

//===================================================
void LCD1280::drawPixel (uint8_t x, uint8_t y, boolean color) {
  if ((x < 0) || (x >= LCD_X) || (y < 0) || (y >= LCD_Y)) return;

  if (color) LCD_RAM[x+ (y/8)*LCD_X] |= _BV(y%8);
  else       LCD_RAM[x+ (y/8)*LCD_X] &= ~_BV(y%8); 
}

//**********************************************************************************************************
//======================================================
void LCD1280::fillScreen(boolean color) {
  fillRect(0, 0, LCD_X, LCD_Y, color);
}
//=====================================================
void LCD1280::drawChar(uint8_t x, uint8_t y, boolean color, unsigned char c) {

  if((x >= LCD_X) ||(y >= LCD_Y) || ((x + 4) < 0) || ((y + 7) < 0))
    return;

  if(c<128)            c = c-32;
  if(c>=144 && c<=175) c = c-48;
  if(c>=128 && c<=143) c = c+16;
  if(c>=176 && c<=191) c = c-48;
  if(c>191)  return;

  for (uint8_t i=0; i<6; i++ ) {
    uint8_t line;
    (i == 5)? line = 0x0 : line = pgm(ASCII+(c*5)+i);
    for (uint8_t j = 0; j<8; j++) {
      (line & 0x1)? drawPixel(x+i, y+j, color) : drawPixel(x+i, y+j, !color);
      line >>= 1;
    }
  }
}

//========================================================
void LCD1280::print(uint8_t x, uint8_t y, boolean color, char *str){
  unsigned char type = *str;
  if(type>=128) x=x-3;
  while(*str){ 
    drawChar(x, y, color, *str++);
    unsigned char type = *str;
    (type>=128)? x=x+3 : x=x+6;
  }
}
//========================================================
void LCD1280::print(uint8_t x, uint8_t y, boolean color, long num){
  char c[20];
  print(x, y, color, ltoa(num,c,10));
}
//========================================================
void LCD1280::print_1607(uint8_t x, uint8_t y, boolean color, char *str)
{
uint8_t nPos[16]={0,6,12,18,24,30,36,42,48,54,60,66,72,78,84,90};
uint8_t nStr[7]={1,10,20,30,40,50,60};
print(nPos[x], nStr[y], color, str);
}

//====================================================
void LCD1280::drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, boolean color) {
  int steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swap(x0, y0);
    swap(x1, y1);
  }
  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }
  int dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int err = dx / 2;
  int ystep;

  (y0 < y1)?  ystep = 1 : ystep = -1;

  for (; x0<=x1; x0++) {
    (steep)? drawPixel(y0, x0, color) : drawPixel(x0, y0, color);
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

//========================================
void LCD1280::drawFastVLine(uint8_t x, uint8_t y, uint8_t h, boolean color) {
  drawLine(x, y, x, y+h-1, color);
}

//======================================
void LCD1280::drawFastHLine(uint8_t x, uint8_t y, uint8_t w, boolean color) {
  drawLine(x, y, x+w-1, y, color);
}

//============================================
void LCD1280::drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, boolean color) {
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}

//============================================
void LCD1280::drawCircle(uint8_t x0, uint8_t y0, int16_t r, boolean color) {
  int f = 1 - r;
  int ddF_x = 1;
  int ddF_y = -2 * r;
  int x = 0;
  int y = r;

  drawPixel(x0, y0+r, color);
  drawPixel(x0, y0-r, color);
  drawPixel(x0+r, y0, color);
  drawPixel(x0-r, y0, color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}

//===============================
void LCD1280::drawRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, boolean color) {
  // smarter version
  drawFastHLine(x+r , y , w-2*r, color); // Top
  drawFastHLine(x+r , y+h-1, w-2*r, color); // Bottom
  drawFastVLine( x , y+r , h-2*r, color); // Left
  drawFastVLine( x+w-1, y+r , h-2*r, color); // Right
  // draw four corners
  drawCircleHelper(x+r , y+r , r, 1, color);
  drawCircleHelper(x+w-r-1, y+r , r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r , y+h-r-1, r, 8, color);
}

//==============================================
void LCD1280::drawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, boolean color) {
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}

//======================================================
void LCD1280::drawCircleHelper(uint8_t x0, uint8_t y0, uint8_t r, uint8_t cornername, boolean color) {
  int f = 1 - r;
  int ddF_x = 1;
  int ddF_y = -2 * r;
  int x = 0;
  int y = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
    if (cornername & 0x4) {
      drawPixel(x0 + x, y0 + y, color);
      drawPixel(x0 + y, y0 + x, color);
    }
    if (cornername & 0x2) {
      drawPixel(x0 + x, y0 - y, color);
      drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      drawPixel(x0 - y, y0 + x, color);
      drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      drawPixel(x0 - y, y0 - x, color);
      drawPixel(x0 - x, y0 - y, color);
    }
  }
}

//========================================
void LCD1280::fillCircle(uint8_t x0, uint8_t y0, uint8_t r, boolean color) {
  drawFastVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

//======================================================
void LCD1280::fillCircleHelper(uint8_t x0, uint8_t y0, uint8_t r, uint8_t cornername, uint8_t delta, boolean color) {

  int f = 1 - r;
  int ddF_x = 1;
  int ddF_y = -2 * r;
  int x = 0;
  int y = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;

    if (cornername & 0x1) {
      drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

//=====================================
void LCD1280::fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, boolean color) {
  for (int16_t i=x; i<x+w; i++) {
    drawFastVLine(i, y, h, color);
  }
}

//======================
void LCD1280::fillRoundRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t r, boolean color) {
  // smarter version
  fillRect(x+r, y, w-2*r, h, color);

  // draw four corners
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r , y+r, r, 2, h-2*r-1, color);
}

//=====================================
void LCD1280::fillTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, boolean color) {

  int a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a) a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a) a = x2;
    else if(x2 > b) b = x2;
    drawFastHLine(a, y0, b-a+1, color);
    return;
  }

  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1,
    sa = 0,
    sb = 0;

  if(y1 == y2) last = y1; // Include y1 scanline
  else last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    /* longhand:
a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
*/
    if(a > b) swap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }

  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    /* longhand:
a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
*/
    if(a > b) swap(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }
}

//======================================================
void LCD1280::drawBitmap(uint8_t x, uint8_t y, const char *bitmap, uint8_t w, uint8_t h, boolean color) {
  for (int16_t j=0; j<h; j++) {
    for (int16_t i=0; i<w; i++ ) {
      if (pgm(bitmap + i + (j/8)*w) & _BV(j%8)) {
        drawPixel(x+i, y+j, color);
      }
    }
  }
}
//**********************************************************************************************************//
void LCD1280::Write(uint8_t x, uint8_t y, bool color, uint8_t data){
  for(uint8_t i=0; i<8; i++){
    (data&0x01)? drawPixel(x, y+i, color) : drawPixel(x, y+i, !color);
    data >>= 1;
  }
}
void LCD1280::WriteNByte(uint8_t N, uint8_t x, uint8_t y, const uint8_t *Font, bool color, uint16_t num){
  for(uint8_t i=0; i<N; i++){
    Write(x, y+i*8, color, pgm(&Font[num+i]));
  }
}
void LCD1280::DrawChar(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *Font, bool color, uint8_t Char){
  if((x >= LCD_X) ||(y >= LCD_Y))
    return;
  uint8_t VAL;
  uint8_t i = x, j = y;
  if(h%8 == 0){VAL = h/8;}
  else { VAL = (h/8)+1;}
	 uint8_t ascii = Char - 32;
   int char_start  = (ascii * VAL * w) + 1 + ascii;
    for (uint16_t z = 0; z < w; z++){
     WriteNByte(VAL, i+z, j, Font, color, (char_start + z*VAL));
    }
}
void LCD1280::print(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *Font, boolean color, char *str){
  unsigned char type = *str;
  if(type>=128) x=x-w/2;
  while(*str){ 
    unsigned char type = *str;
    //if(*str==32) x=x-(w/2);
    DrawChar(x, y, w, h, Font, color, *str++); 
    (type>=128)? x=x+w/2 : x=x+w;
  }
}
void LCD1280::write(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *Font, boolean color, int16_t num){
  char data[20];
  sprintf(data, "%3d", num);
  print(x, y, w, h, Font, color, data);
}
void LCD1280::write(uint8_t x, uint8_t y, uint8_t w, uint8_t h, const uint8_t *Font, boolean color, float num){
  char data[5];
  sprintf(data, "%.2f", num);
  print(x, y, w, h, Font, color, data);
}
