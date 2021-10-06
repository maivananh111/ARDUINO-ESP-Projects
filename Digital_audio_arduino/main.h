    #include <avr/io.h>
    #include <avr/interrupt.h>
    #include "lcd1280.h"
    #include "tda7439.h"
    
  
    #define BUT    PD2
    #define EN_1   PD3
    #define EN_2   PD4

    
    uint8_t ST, LST;
    int16_t T = 0, Time = 2000;
    static volatile int16_t counter = 0, last_counter = 0;
    static volatile int8_t run = 0, set = 0;

    int16_t volume = 0, bass = 0, mid = 0, treble = 0, input = 0, balance = 0;
    int16_t gain = 0;
    

    
      
