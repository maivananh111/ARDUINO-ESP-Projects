#include <Wire.h> 
#include <TDA7439.h> 
#include <LiquidCrystal.h> 
#include <EEPROM.h> 
#include <Encoder.h>
  TDA7439 tda;
  Encoder myEnc ( 9 , 8 ) ; // CLK, DT,SW=12 
  LiquidCrystal lcd (2,3,4,5,6,7 ) ; // RS, E, D4, D5, D6, D7 
  byte a1 [ 8 ] = { 0b00000,0b11011,0b11011,0b11011,0b11011,0b11011,0b11011,0b00000 } ;
  byte a2 [ 8 ] = { 0b00000,0b11000,0b11000,0b11000,0b11000,0b11000,0b11000,0b00000 } ;

  char twoDigitNumber[3] = "00";

byte leftSide[8] = 
{
  B00111,//1
  B01111,
  B01111,
  B01111,
  B01111,
  B01111,
  B01111,
  B00111
};
byte upperBar[8] =
{
  B11111,//2
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
byte rightSide[8] =
{
  B11100,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,//3
  B11110,
  B11100
};
byte leftEnd[8] =
{
  B01111,
  B00111,
  B00000,
  B00000,
  B00000,
  B00000,//4
  B00011,
  B00111
};
byte lowerBar[8] =
{
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111//5
};
byte rightEnd[8] =
{
  B11110,
  B11100,
  B00000,
  B00000,
  B00000,
  B00000,
  B11000,//6
  B11100
};
byte middleBar[8] =
{
  B11111,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000,//7
  B11111,
  B11111
};
byte lowerEnd[8] = 
{
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00111,
  B01111//8
};
//
//   
  // create custom characters
  int customchar[10][6] ={{0,1,2,0,4,2},  //0
                           {254,254,2,254,254,2},  //1
                           {3,6,2,0,4,4},//2
                           {3,6,2,7,4,2},//3
                         {0,4,2,254,254,2},//4
                           {0,6,5,7,4,2},//5
                           {0,6,5,0,4,2},//6
                           {1,1,2,254,254,2},//7                           
                           {0,6,2,0,4,2},//8
                           {0,6,2,7,4,2},
                  }; 
  
//   int customchar[10][6] ={{0,4,2,0,4,2},  //0
//                           {32,3,32,5,3,5},  //1
//                           {2,2,3,1,7,7},//2
//                           {2,2,3,5,7,3},//3
//                         {1,4,5,2,2,3},//4
//                           {1,2,6,4,7,3},//5
//                           {1,2,6,1,7,3},//6
//                           {2,2,3,32,32,3},//7
//                           {1,7,3,1,5,3},//8
//                           {1,7,3,32,32,3},
//                           }; 
  
  int vol, vol_ram, vol_d, z, bass, bass_ram, mids, mids_ram, treb, treb_ram, balans, balans_ram, in , in_ram, gain , gain1_ram, gain1, gain2, gain2_ram, gain3, gain3_ram, gain4, gain4_ram;
  byte menu , w, w1, in_d;
 unsigned long time , time1; long oldPosition = - 999 ;
void force2Digits(int number){
itoa(number, twoDigitNumber,10);  
if (number >= 0 && number < 10) {
    twoDigitNumber[1] = twoDigitNumber[0];
    twoDigitNumber[0] = '0'; }
}
void printBigDigit(int number, int xpos){

       
     lcd.setCursor(xpos,0);  
  for (int x=0;x<3;x++)
  {
         lcd.write(byte(customchar[number - 48][x]));      
         }
lcd.setCursor(xpos,1);
     for (int x=3;x<6;x++)
     {
      lcd.write(byte(customchar[number -48][x]));       } 
}
 
 void ram ( )
 { vol_ram = vol, bass_ram = bass, mids_ram = mids, treb_ram = treb, balans_ram = balans, in_ram = in , gain1_ram = gain1; gain2_ram = gain2; gain3_ram = gain3; gain4_ram = gain4; } 
 void g1 ( ) { if (in == 1 ) { gain = gain1; } if ( in == 2 ) { gain = gain2; } if ( in == 3 ) { gain = gain3; } if ( in == 4 ) { gain = gain4; } } 
 void g2 ( ) { if ( in == 1 ) { gain1 = gain ; } if( in == 2 ) { gain2 = gain ; } if ( in == 3 ) { gain3 = gain ; } if ( in == 4 ) { gain4 = gain ; } } 
void audio ( ) { 
tda.setInput ( in ) ; // 1 to 4 // channel number 
tda.inputGain ( gain ) ; // 0 to 15 // gain
tda.setVolume ( vol ) ; // 0 to 48 (0 is mute) // volume 
tda.setSnd ( bass, 1 ) ; // - 7 to +7, 1 - Bass | 2 - Mids | 3 - Trebble 
tda.setSnd ( mids, 2 ) ;
tda.setSnd ( treb, 3 ) ;
tda.spkAtt (   balans ) ; // RL balance from 0 to 79 (db) 
 }
 void setup ( ) 
 { 
  Serial. begin ( 9600 ) ;
 Serial. print ( "SK VILAKKUPARA YT" ) ;
  //Initialise the custom characters


  lcd.createChar(0, leftSide);
  lcd.createChar(1, upperBar);
  lcd.createChar(2, rightSide);
  lcd.createChar(3, leftEnd); //design 1
  lcd.createChar(4, lowerBar);
  lcd.createChar(5 ,rightEnd);
  lcd.createChar(6, middleBar);
  lcd.createChar(7, lowerEnd);
  
//    lcd.createChar(1, seg_1);
//    lcd.createChar(2, seg_2);
//    lcd.createChar(3, seg_3); //design 2
//    lcd.createChar(4, seg_4);
//    lcd.createChar(5, seg_5);
//    lcd.createChar(6, seg_6);
//    lcd.createChar(7, seg_7);
//    lcd.createChar(8, seg_8);

lcd. begin ( 16 , 2 ) ;
  lcd. setCursor ( 0 , 0 ) ; 
  lcd. print ( "  SK VILAKKUPARA YT  " ) ; delay ( 1000 ) ;
   pinMode ( 12 , INPUT ) ; // menu encoder button SW add a 10k resistor to vcc +5v toinput pin
   pinMode ( 11 , INPUT ) ; // in 1-4 add aresistor 10k to GND
  lcd. createChar ( 0 , a1 ) ; lcd. createChar ( 1 , a2 ) ;
  vol = EEPROM. read ( 0 ) ; // vol eeprom 
  bass = EEPROM. read ( 1 ) - 7 ; // bass eeprom 
  mids = EEPROM. read ( 2 ) - 7 ; // mids eeprom 
  treb = EEPROM. read ( 3 ) - 7 ; // treb eeprom 
  balans = EEPROM. read ( 4 ) - 4 ; // balans eeprom 
  in = EEPROM. read ( 5) ; // in eeprom 
  gain1 = EEPROM. read ( 6 ) ; // gain1 eeprom 
  gain2 = EEPROM. read ( 7 ) ; // gain2 eeprom 
  gain3 = EEPROM. read ( 8 ) ; // gain3 eeprom 
  gain4 = EEPROM. read ( 9 ) ; // gain4 eeprom 
  g1 ( ) ; 
  audio ( ) ; 
  ram ( ) ;
 }
 
 void loop ( )  { 
   if ( digitalRead ( 12 ) == LOW ) 
   { menu ++; ram ( ) ; in_d = 0 ; 
   myEnc. write ( 0 ) ; oldPosition = - 999 ; delay ( 500 ) ; time1 = millis ( ) ; w = 1 ; 
   if ( menu > 5 ) {
    menu = 0 ;
   }
   } // menu 
   if ( digitalRead ( 11  ) == HIGH) { in ++ ;; ram ( ) ; in_d = 1 ; g2 ( ) ; myEnc. write ( 0 ) ;
   oldPosition = - 999 ; menu = 5 ; w = 1 ; delay ( 500 ) ; time1 = millis ( ) ;
   if ( in > 4 || in < 1 ) { in = 1 ; } }// in 
  // volume indication + control ////////////// 
   if ( menu == 0 ) 
   { 
    long newPosition = myEnc. read ( ) / 4 + vol_ram;
    if  ( newPosition != oldPosition )  { 
    oldPosition = newPosition; lcd. clear ( ) ; vol = newPosition;
   if ( vol > 48 ) { vol = 48 ; }  if ( vol < 0 ) { vol =0 ; } // 48 maximum volume, 0 minimum volume 
  lcd. setCursor ( 0 , 0 ) ; lcd. print ( "VOLUME" ) ;
  //lcd. print ( - 48 + vol ) ; lcd. setCursor ( 15 , 0 ) ; lcd. print ( "dB" ) ;
  vol_d = vol / 2 ;
  force2Digits(vol);
  printBigDigit(twoDigitNumber[0],9);
  printBigDigit(twoDigitNumber[1],13);
  
//   if ( vol_d >= 0 ) { for ( z = 0 ; z <= vol_d; z ++ ) { lcd. setCursor ( z / 2 , 1 ) ; lcd. write ( ( uint8_t ) 0 ) ; } } 
//   if ( vol_d % 2 == 0 ) { lcd. setCursor ( z / 2 ,1 ) ; lcd. write ( ( uint8_t ) 1 ) ; } 
//   lcd. setCursor ( 13 , 1 ) ; lcd. print ( vol ) ;
   audio ( ) ; time = millis ( ) ; time1 = millis ( ) ; w = 1 ; w1 = 1 ;
 }
 }
 
 // indication of bass tone + button control ////////////// 
   if ( menu == 1 ) { 
    long newPosition = myEnc. read ( ) / 4 + bass_ram;
    if  ( newPosition != oldPosition )  { 
    oldPosition = newPosition; lcd. clear ( ) ; bass = newPosition;
   if ( bass > 7 )
   { 
    bass = 7 ;
   }
   if ( bass < - 7 )
   {
    bass = - 7 ; 
   } 


   
  lcd. setCursor ( 0 , 0 ) ;
  lcd. print ( "< BASS >" ) ;
  force2Digits(bass*2);
  printBigDigit(twoDigitNumber[0],9);
  printBigDigit(twoDigitNumber[1],13);
  
 // lcd. print ( bass * 2 ) ; 
 //lcd. setCursor ( 15 , 0 ) ;// 
 // lcd. print ( "dB" ) ;//
 
  // for ( z = - 7 ; z <= bass; z ++ ) {
   //lcd. setCursor ( z + 7 , 1 ) ; 
  // lcd//. write ( ( uint8_t ) 0 ) ;
   } 
   audio ( ) ; time = millis () ; time1 = millis ( ) ; w = 1 ; w1 = 1 ;
 }  
 ////////////////////////////////////////////////// ///////////
 
    // Indication of the MF tone + button control ////////////// 
   if ( menu == 2 ) { 
    long newPosition = myEnc. read ( ) / 4 + mids_ram;
    if  ( newPosition != oldPosition )  { 
    oldPosition = newPosition; lcd. clear ( ) ; mids = newPosition;
   if ( mids > 7 ) { mids = 7 ; } if ( mids < - 7 ) {mids = - 7 ; } 
  lcd. setCursor ( 0 , 0 ) ;
  lcd. print ( "< MID >" ) ;
   
  force2Digits(mids*2);
  printBigDigit(twoDigitNumber[0],9);
  printBigDigit(twoDigitNumber[1],13);
  
  //lcd. setCursor ( 0 , 0 ) ; lcd. print ( "Mids" ) ;lcd. print ( mids * 2 ) ; lcd. setCursor ( 13 , 0 ) ; lcd. print ( "dB" ) ;
   //for ( z = - 7 ; z <= mids; z ++ ) { lcd. setCursor ( z + 7 , 1 ) ; lcd. write ( ( uint8_t ) 0 ) ; } 
   audio ( ) ; time = millis () ; time1 = millis ( ) ; w = 1 ; w1 = 1 ;
 } } 
 ////////////////////////////////////////////////// ///////////
 
  // indication of the tone of the tweeter + control buttons ////////////// 
   if ( menu == 3 ) { 
    long newPosition = myEnc. read ( ) / 4 + treb_ram;
    if  ( newPosition != oldPosition )  { 
    oldPosition = newPosition; lcd. clear ( ) ; treb = newPosition;
   if ( treb > 7 ) { treb = 7 ; } if ( treb < - 7 ) {treb = - 7 ; } 
   lcd. setCursor ( 0 , 0 ) ;
  lcd. print ( "<TREBLE>" ) ;
   
  force2Digits(treb*2);
  printBigDigit(twoDigitNumber[0],9);
  printBigDigit(twoDigitNumber[1],13);
  
//  lcd. setCursor ( 0 , 0 ) ; lcd. print ( "Treble" ) ;
//  lcd. print ( treb * 2 ) ; lcd. setCursor ( 13 , 0 ) ; lcd. print ( "dB" ) ;
//   for ( z = - 7 ; z <= treb; z ++ ) { lcd. setCursor ( z + 7 , 1 ) ; lcd. write ( ( uint8_t ) 0 ) ; } 
   audio ( ) ; time = millis () ; time1 = millis ( ) ; w = 1 ; w1 = 1 ;
 } } 
 ////////////////////////////////////////////////// ///////////
 
  // balance indication + button control + \ - 4 dB /////// 
   if ( menu == 4 ) { 
    long newPosition = myEnc. read ( ) / 4 + balans_ram;
    if  ( newPosition != oldPosition )  { 
    oldPosition = newPosition; lcd. clear ( ) ; balans = newPosition;
   if ( balans > 4 ) { balans = 4 ; } if ( balans < - 4 ) {balans = - 4 ; } 
  lcd. setCursor ( 0 , 0 ) ;
   if ( balans >= 0 ) { lcd. print ( "-" ) ; } else { lcd. print ( "+" ) ; } 
  lcd. print ( abs ( balans ) ) ; lcd. print ( "dB" ) ;
  lcd. print ( "<>" ) ;
   if ( balans >= 0 ) { lcd. print ( "+" ) ; } else { lcd. print ( "-" ) ; } 
  lcd. print ( abs ( balans ) ) ; lcd. print ( "dB" ) ;
  lcd. setCursor ( 0 , 1 ) ; lcd. print ( "R" ) ;
  lcd. setCursor ( 15 , 1 ) ; lcd. print ( "L" ) ;
   if ( balans < 0 ) { lcd. setCursor ( balans + 7 , 1 ) ; lcd. write ( ( uint8_t ) 0 ) ; } 
   if ( balans > 0 ) { lcd. setCursor ( balans + 8 , 1 ) ; lcd.write ( ( uint8_t ) 0 ) ; } 
   if ( balans == 0 ) { lcd. setCursor ( 7 , 1 ) ; lcd. write ( ( uint8_t ) 0 ) ; lcd. setCursor ( 8 , 1 ) ; lcd. write ( ( uint8_t ) 0 ) ; } 
   audio ( ) ; time = millis ( ); time1 = millis ( ) ; w = 1 ; w1 = 1 ;
 } } 
 ////////////////////////////////////////////////// /////////////
 
  /////////// input + gain1 //////////////////////////////////// ///// 
   if ( in_d == 1 && in == 1 ) { 
    long newPosition = myEnc.read() /4 + gain1_ram;
    if  ( newPosition != oldPosition )  
    { 
    oldPosition = newPosition; 
    lcd. clear ( ) ;
    gain1 = newPosition;
   if ( gain1 > 15 ) { gain1 = 15 ; }
   if ( gain1 <0 ) { gain1 = 0 ; } 
    lcd. setCursor ( 0 , 0 ) ;
    lcd. print ( "INPUT " ) ; 
    lcd. print ( in ) ;
    lcd. setCursor ( 0 , 1 ) ;
    lcd. print ( "< GAIN >" ) ;
  force2Digits(gain1*2);
  printBigDigit(twoDigitNumber[0],9);
  printBigDigit(twoDigitNumber[1],13);
  
    //lcd. print ( gain1 ) ;
   g1 ( ) ; audio ( ) ; time = millis ( ) ; time1 = millis ( ) ; w = 1 ; w1 = 1 ; g2 ( ) ;
 } } 
////////////////////////////////////////////////// //////////// 
  /////////// input + gain2 ////////////////////// ///////////////// 
   if ( in_d == 1 && in == 2 ) { 
    long newPosition = myEnc. read ( );                                                                                                                                                                                                                                                                                       
    if  (newPosition != oldPosition )  { 
    oldPosition = newPosition; lcd. clear ( ) ; gain2 = newPosition;
   if ( gain2 > 15 ) { gain2 = 15 ; } if ( gain2 < 0 ) { gain2 = 0 ; } 
    lcd. setCursor ( 0 , 0 ) ;
    lcd. print ( "INPUT " ) ; 
    lcd. print ( in ) ;
    lcd. setCursor ( 0 , 1 ) ;
    lcd. print ( "< GAIN >" ) ;
  force2Digits(gain2*2);
  printBigDigit(twoDigitNumber[0],9);
  printBigDigit(twoDigitNumber[1],13);
  
   //lcd. print ( gain2 ) ;
   g1 ( ) ; audio ( ) ; time = millis ( ) ; time1 = millis ( ) ; w = 1 ; w1 = 1 ; g2 ( ) ;
 } } 
////////////////////////////////////////////////// //////////// 
  /////////// input + gain3 ////////////////////// ///////////////// 
   if ( in_d == 1 && in == 3 ) { 
    long newPosition = myEnc.read ( );
    if  (newPosition != oldPosition )  { 
    oldPosition = newPosition; lcd. clear ( ) ; gain3 = newPosition;
   if ( gain3 > 15 ) { gain3 = 15 ; } if ( gain3 < 0 ) { gain3 = 0 ; } 
    lcd. setCursor ( 0 , 0 ) ;
    lcd. print ( "INPUT " ) ; 
    lcd. print ( in ) ;
    lcd. setCursor ( 0 , 1 ) ;
    lcd. print ( "< GAIN >" ) ;
  force2Digits(gain3*2);
  printBigDigit(twoDigitNumber[0],9);
  printBigDigit(twoDigitNumber[1],13);
  // lcd. print ( gain3 ) ;
   g1 ( ) ; audio ( ) ; time = millis ( ) ; time1 = millis ( ) ; w = 1 ; w1 = 1 ; g2 ( ) ;
 } } 
////////////////////////////////////////////////// //////////// 
  /////////// input + gain4 ////////////////////// ///////////////// 
   if ( in_d == 1 && in == 4 ) { 
    long newPosition = myEnc. read ( ) / 4 + gain4_ram;
    if  (newPosition != oldPosition )  { 
    oldPosition = newPosition; lcd. clear ( ) ; gain4 = newPosition;
   if ( gain4 > 15 ) { gain4 = 15 ; } if ( gain4 < 0 ) { gain4 = 0 ; } 
    lcd. setCursor ( 0 , 0 ) ;
    lcd. print ( "INPUT " ) ; 
    lcd. print ( in ) ;
    lcd. setCursor ( 0 , 1 ) ;
    lcd. print ( "< GAIN >" ) ;
  force2Digits(gain4);
  printBigDigit(twoDigitNumber[0],9);
  printBigDigit(twoDigitNumber[1],13);
  
//    lcd. setCursor ( 0 , 0 ) ; lcd. print ( "Input:" ) ; lcd. print ( in ) ;
//    lcd. setCursor ( 0 , 1 ) ; lcd. print ( "Input Gain:" ) ; lcd. print ( gain4 ) ;
   g1 ( ) ; audio ( ) ; time = millis ( ) ; time1 = millis ( ) ; w = 1 ; w1 = 1 ; 
 } } 
////////////////////////////////////////////////// ////////////
 
   if ( millis ( ) -time > 10000  && w1 == 1 )
   {
    // return to the volume menu after 10 seconds of inactivity 
     ram ( ) ; 
     myEnc. write ( 0 ) ; 
     menu = 0 ;
     w1 = 0 ;
     in_d = 0 ; 
     } 
   if ( millis ( ) -time1 > 60000  && w == 1 ) 
   { // write all settings to the EEPROM after 60 seconds of inactivity of the 
     EEPROM.update ( 0 , vol_ram ) ;
     EEPROM. update ( 4 , balans + 4 ) ;
     EEPROM. update ( 1 , bass + 7 ) ;
     EEPROM. update ( 2 , mids + 7 ) ;
     EEPROM. update ( 3 , treb + 7 ) ;
     EEPROM. update ( 5 , in ) ;
     EEPROM. update ( 6 , gain1 ) ; 
     EEPROM. update ( 7 , gain2 ) ; 
     EEPROM. update ( 8 , gain3 ) ; 
     EEPROM. update ( 9 , gain4 ) ; w = 0;
 } 
 }
 
