
    #include "main.h"
   
    
    LCD1280 lcd(10, 11, 12, 13);
    TDA7439 tda;

    
    void setup(){ 

       pinMode(BUT, INPUT_PULLUP);
       pinMode(EN_1, INPUT_PULLUP);
       pinMode(EN_2, INPUT_PULLUP);
       lcd.Init();
       lcd.Clear();
       attachInterrupt(0, but, FALLING);
       attachInterrupt(1, encoder, FALLING);

       LST = digitalRead(EN_1);
    }
    
    void loop(){
        if(set) InMenu();
        else mainMenu();

    }                     
    void mainMenu(){
       if(counter>6) counter = 0;
       if(counter<0) counter = 6;
       lcd.Clear();
       lcd.print(8, 0, 10, 15, Bodoni_MT10x15, 1, "MainMenu");
       switch(counter){  
        case 0:
//          lcd.print(8, 0, 10, 15, Bodoni_MT10x15, 1, "MainMenu");
          lcd.print(0, 16, 1, "=>>");
          lcd.print(20, 16, 10, 15, Bodoni_MT10x15, 0, "VOLUME ");
          lcd.print(0, 29, 10, 15, Bodoni_MT10x15, 1, "  Bass   ");
          lcd.print(0, 42, 10, 15, Bodoni_MT10x15, 1, "  Midle  ");
          lcd.print(0, 55, 10, 15, Bodoni_MT10x15, 1, "  Treble ");
          lcd.Update();
        break;
        case 1:
//          lcd.print(8, 0, 10, 15, Bodoni_MT10x15, 1, "MainMenu");
          lcd.print(0, 16, 10, 15, Bodoni_MT10x15, 1, "  VOLUME ");
          lcd.print(0, 32, 1, "=>>");
          lcd.print(20, 29, 10, 15, Bodoni_MT10x15, 0, "Bass   ");         
          lcd.print(0, 42, 10, 15, Bodoni_MT10x15, 1, "  Midle  ");          
          lcd.print(0, 55, 10, 15, Bodoni_MT10x15, 1, "  Treble ");
          lcd.Update();
        break;
        case 2:
//          lcd.print(8, 0, 10, 15, Bodoni_MT10x15, 1, "MainMenu");
          lcd.print(0, 16, 10, 15, Bodoni_MT10x15, 1, "  VOLUME ");
          lcd.print(0, 29, 10, 15, Bodoni_MT10x15, 1, "  Bass   ");
          lcd.print(0, 45, 1, "=>>");
          lcd.print(20, 42, 10, 15, Bodoni_MT10x15, 0, "Midle  ");          
          lcd.print(0, 55, 10, 15, Bodoni_MT10x15, 1, "  Treble ");
          lcd.Update();
        break;
        case 3:
//          lcd.print(8, 0, 10, 15, Bodoni_MT10x15, 1, "MainMenu");
          lcd.print(0, 16, 10, 15, Bodoni_MT10x15, 1, "  VOLUME ");
          lcd.print(0, 29, 10, 15, Bodoni_MT10x15, 1, "  Bass   ");
          lcd.print(0, 42, 10, 15, Bodoni_MT10x15, 1, "  Midle  ");
          lcd.print(0, 58, 1, "=>>");
          lcd.print(20, 55, 10, 15, Bodoni_MT10x15, 0, "Treble ");
          lcd.Update();
        break;
        case 4:
//          lcd.print(8, 0, 10, 15, Bodoni_MT10x15, 1, "MainMenu");
          lcd.print(0, 16, 10, 15, Bodoni_MT10x15, 1, "  Bass   ");
          lcd.print(0, 29, 10, 15, Bodoni_MT10x15, 1, "  Midle  ");
          lcd.print(0, 42, 10, 15, Bodoni_MT10x15, 1, "  Treble ");
          lcd.print(0, 58, 1, "=>>");
          lcd.print(20, 55, 10, 15, Bodoni_MT10x15, 0, "Balance");
          lcd.Update();
        break;
        case 5:
//          lcd.print(8, 0, 10, 15, Bodoni_MT10x15, 1, "MainMenu");
          lcd.print(0, 16, 10, 15, Bodoni_MT10x15, 1, "  Midle  ");
          lcd.print(0, 29, 10, 15, Bodoni_MT10x15, 1, "  Treble ");
          lcd.print(0, 42, 10, 15, Bodoni_MT10x15, 1, "  Balance");
          lcd.print(0, 58, 1, "=>>");
          lcd.print(20, 55, 10, 15, Bodoni_MT10x15, 0, "LineIn ");
          lcd.Update();
        break;
        case 6:
//          lcd.print(8, 0, 10, 15, Bodoni_MT10x15, 1, "MainMenu");
          lcd.print(0, 16, 10, 15, Bodoni_MT10x15, 1, "  Treble ");
          lcd.print(0, 29, 10, 15, Bodoni_MT10x15, 1, "  Balance");
          lcd.print(0, 42, 10, 15, Bodoni_MT10x15, 1, "  LineIn ");
          lcd.print(0, 58, 1, "=>>");
          lcd.print(20, 55, 10, 15, Bodoni_MT10x15, 0, "Gain In");
          lcd.Update();
        break;
      }  
    }

    void InMenu(){
       if(last_counter>6) last_counter = 0;
       if(last_counter<0) last_counter = 6;
       lcd.Clear();
       if(last_counter == 0){
          lcd.print(20, 5, 10, 15, Bodoni_MT10x15, 1, "VOLUME");
          if(counter>48) counter = 0;
          if(counter<0) counter = 48;
          volume = counter;
          uint8_t x_vol = volume*2;          
          lcd.write(15, 20, 10, 15, Bodoni_MT10x15, 1, (int16_t)volume);
          lcd.print(50, 20, 10, 15, Bodoni_MT10x15, 1, "dB");
          lcd.fillRect(0, 40, x_vol, 10, 1);
          lcd.fillRect(x_vol, 40, 95-x_vol, 10, 0);
          lcd.Update();
       }
       else if(last_counter == 1){
          lcd.print(30, 5, 10, 15, Bodoni_MT10x15, 1, "BASS");
          if(counter>7) counter = -7;
          if(counter<-7) counter = 7;
          bass = counter;
          uint8_t x_bass = bass*(85/14) + 95/2;          
          lcd.write(15, 20, 10, 15, Bodoni_MT10x15, 1, (int16_t)bass);
          lcd.print(50, 20, 10, 15, Bodoni_MT10x15, 1, "dB");
          lcd.fillRect(5, 40, 5+x_bass, 10, 1);
          lcd.fillRect(x_bass+5, 40, 95-x_bass, 10, 0);
          lcd.Update();
       }
        else if(last_counter == 2){
          lcd.print(25, 5, 10, 15, Bodoni_MT10x15, 1, "MIDLE");
          if(counter>7) counter = -7;
          if(counter<-7) counter = 7;
          mid = counter;
          uint8_t x_mid = mid*(85/14) + 95/2;          
          lcd.write(15, 20, 10, 15, Bodoni_MT10x15, 1, (int16_t)mid);
          lcd.print(50, 20, 10, 15, Bodoni_MT10x15, 1, "dB");
          lcd.fillRect(5, 40, 5+x_mid, 10, 1);
          lcd.fillRect(x_mid+5, 40, 95-x_mid, 10, 0);
          lcd.Update();
        }
        else if(last_counter == 3){
          lcd.print(18, 5, 10, 15, Bodoni_MT10x15, 1, "TREBLE");
          if(counter>7) counter = -7;
          if(counter<-7) counter = 7;
          treble = counter;
          uint8_t x_treble = treble*(85/14) + 95/2;          
          lcd.write(15, 20, 10, 15, Bodoni_MT10x15, 1, (int16_t)treble);
          lcd.print(50, 20, 10, 15, Bodoni_MT10x15, 1, "dB");
          lcd.fillRect(5, 40, 5+x_treble, 10, 1);
          lcd.fillRect(x_treble+5, 40, 95-x_treble, 10, 0);
          lcd.Update();
        }
        else if(last_counter == 4){
          lcd.print(15, 5, 10, 15, Bodoni_MT10x15, 1, "BALANCE");
          if(counter>4) counter = -4;
          if(counter<-4) counter = 4;
          balance = counter;
          uint8_t x_balance = balance*10 + 43;          
          lcd.print(0, 20, 1, "L:  dB");
          lcd.print(11, 20, 1, -balance);
          lcd.print(60, 20, 1, "R:  dB");
          lcd.print(71, 20, 1, balance);
          lcd.fillRect(x_balance, 40, 10, 10, 1);
          lcd.fillRect(0, 40, x_balance, 10, 0);
          lcd.fillRect(x_balance+10, 40, (93-x_balance-10), 10, 0);
          lcd.print(0, 55, 10, 15, Bodoni_MT10x15, 1, "L>");
          lcd.print(76, 55, 10, 15, Bodoni_MT10x15, 1, "<R");
          lcd.Update();
        }
        else if(last_counter == 5){
          lcd.print(25, 5, 10, 15, Bodoni_MT10x15, 1, "AUDIO");
          lcd.print(18, 20, 10, 15, Bodoni_MT10x15, 1, "LineIn");
          if(counter>4) counter = 1;
          if(counter<1) counter = 4;
          input = counter;
          uint8_t x_input = input*(85/14) + 95/2;     
          lcd.print(5, 40, 10, 15, Bodoni_MT10x15, 1, "Input:");     
          lcd.write(67, 40, 10, 15, Bodoni_MT10x15, 1, (int16_t)input);
          lcd.Update();
        }
        else if(last_counter == 6){
          lcd.print(2, 5, 10, 15, Bodoni_MT10x15, 1, "INPUTGAIN");
          if(counter>15) counter = 0;
          if(counter<0) counter = 15;
          gain = counter;
          uint8_t x_gain = gain*5.6+6;          
          lcd.write(15, 20, 10, 15, Bodoni_MT10x15, 1, (int16_t)gain);
          lcd.print(50, 20, 10, 15, Bodoni_MT10x15, 1, "dB");
          lcd.fillRect(3, 40, x_gain+3, 10, 1);
          lcd.fillRect(x_gain+3, 40, 95-x_gain, 10, 0);
          lcd.Update();
        }
    }  

   void but(){
       if(digitalRead(BUT) == LOW){
        for(T = 0; T<Time; T++){
          delay(1);
          if(digitalRead(BUT) == HIGH){
              set = !set;
              if(set) {last_counter = counter;}
              else    {counter = last_counter;}
              T = 0;
              lcd.Clear();
              break;
          }
        }
        if(T == Time){
          run = !run;
          T = 0;
          while(digitalRead(BUT) == LOW);
          lcd.Clear();
          delay(1000);
        }
       }
     }

       void encoder(){
         ST = digitalRead(EN_1);
         if (ST != LST){ 
           if (digitalRead(EN_2) != ST) { 
             counter --;
           } else {
             counter ++;
           }
         LST = ST; 
       }
      }
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 
