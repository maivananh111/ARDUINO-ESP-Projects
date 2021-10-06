  
  #define BLYNK_PRINT Serial
  #include <ESP8266WiFi.h>
  #include "DHT.h"
  #include <BlynkSimpleEsp8266.h>
  #include <string.h>
  
  #define DHTPIN 2   
  #define DHTTYPE DHT11 
  
  char auth[] = "Ar70t5_E6aE24FtelCeWAH0EmAORsBwf";
  char ssid[] = "Nhat Nam";
  char pass[] = "0989339608";
  char *buf = 0;
  int Size = 10;
  char RxBuf[10] = {};
  volatile uint8_t RxCounter = 0;
  char TempBuf[18];
  char HumiBuf[14];
  int _V1, _V5, _V6, _V7, _V8, _V9;
  
  WidgetTerminal terminal(V2);
  WidgetLED led_Connected(V0);
  BlynkTimer led_timer;
  DHT dht(DHTPIN, DHTTYPE);
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  BLYNK_CONNECTED() { Blynk.syncAll();}

  void clearbuf(char buffer[], uint16_t size){
  for(int i=0; i<size; i++) buffer[i] = '\0';
}

  void sendSensor(){
    float h = dht.readHumidity();
    float t = dht.readTemperature(); 
    sprintf(TempBuf, "(Nhiet Do: %2.1f*C|", t);
    digitalWrite(D3, HIGH);
    Serial.print(TempBuf);
    delay(50);
    sprintf(HumiBuf, ")Do Am: %2.1f", h);
    strcat(HumiBuf, "%    |");
    digitalWrite(D3, HIGH);
    Serial.print(HumiBuf);
    delay(50);
    digitalWrite(D3, LOW);
    Blynk.virtualWrite(V4, h);
    Blynk.virtualWrite(V3, t);
  }
  
  void blinkLedWidget(){
    if (led_Connected.getValue()) {
      led_Connected.off();
    } else {
      led_Connected.on();
    }
    sendSensor();
  }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  BLYNK_WRITE(V1){//////////////////////////////////LED1/////////////////
    _V1 = param.asInt(); 
    digitalWrite(D0, _V1);
    digitalWrite(D3, LOW);
    if(_V1 == 1){
      Serial.print("*TIMo|");
    }
    else {
      Serial.print("*TIMf|");
    }
    delay(50);
    digitalWrite(D3, HIGH);
  }

  WidgetLED led1(V20);
  BLYNK_WRITE(V5){//////////////////////////////////LED1/////////////////
    _V5= param.asInt(); 
    digitalWrite(D3, LOW);
    if(_V5 == 1){
      Serial.print("*BT1o|");
      led1.on();
    }
    else {
      Serial.print("*BT1f|");
      led1.off();
    }
    delay(50);
    digitalWrite(D3, HIGH);
  }
  
  WidgetLED led2(V21);
    BLYNK_WRITE(V6){//////////////////////////////////LED2/////////////////
    _V6 = param.asInt(); 
    digitalWrite(D3, LOW);
    if(_V6 == 1){
      Serial.print("*BT2o|");
      led2.on();
    }
    else {
      Serial.print("*BT2f|");
      led2.off();
    }
    delay(50);
    digitalWrite(D3, HIGH);
  }
  
  WidgetLED led3(V22);
    BLYNK_WRITE(V7){//////////////////////////////////LED3/////////////////
    _V7 = param.asInt(); 
    digitalWrite(D3, LOW);
    if(_V7 == 1){
      Serial.print("*BT3o|");
      led3.on();
    }
    else {
      Serial.print("*BT3f|");
      led3.off();
    }
    delay(50);
    digitalWrite(D3, HIGH);
  }
  
  WidgetLED led4(V23);
    BLYNK_WRITE(V8){//////////////////////////////////LED4/////////////////
    _V8 = param.asInt(); 
    digitalWrite(D3, LOW);
    if(_V8 == 1){
      Serial.print("*BT4o|");
      led4.on();
    }
    else {
      Serial.print("*BT4f|");
      led4.off();
    }
    delay(50);
    digitalWrite(D3, HIGH);
  }

  WidgetLED led5(V24);
    BLYNK_WRITE(V9){//////////////////////////////////LED5/////////////////
    _V9 = param.asInt(); 
    digitalWrite(D3, LOW);
    if(_V9 == 1){
      Serial.print("*BT5o|");
      led5.on();
    }
    else {
      Serial.print("*BT5f|");
      led5.off();
    }
    delay(50);
    digitalWrite(D3, HIGH);
  }
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  BLYNK_WRITE(V2){
      buf = (char *)malloc(param.getLength()+1);
      strcpy(buf, param.asStr());
      strcat(buf, "|");
      digitalWrite(D3, LOW);
      Serial.print(buf);
      terminal.flush();
     free(buf);
     delay(50);
     digitalWrite(D3, HIGH);
  }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  void setup(){
    Serial.begin(9600);
    dht.begin();
    Blynk.begin(auth, ssid, pass);
    led_timer.setInterval(1500L, blinkLedWidget);
    pinMode(D0,OUTPUT);
    pinMode(D3,OUTPUT);
    digitalWrite(D0, LOW);
    digitalWrite(D3, HIGH);
  }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  void loop(){
    Blynk.run();
    led_timer.run();
    if(Serial.available() > 0) {
      char data = Serial.read();
      if(RxCounter < Size){
        RxBuf[RxCounter++] = data;
        if(data == '|'){
          if(RxBuf[0] == '*' && RxBuf[1] == 'B' && RxBuf[2] == 'T'){
            if     (RxBuf[4] == 'o'){
              switch(RxBuf[3]){
                case '1':
                 Blynk.virtualWrite(V5, 1);
                break;
                case '2':
                 Blynk.virtualWrite(V6, 1);
                break;
                case '3':
                 Blynk.virtualWrite(V7, 1);
                break;
                case '4':
                 Blynk.virtualWrite(V8, 1);
                break;
                case '5':
                 Blynk.virtualWrite(V9, 1);
                break;
              }
            }
            else if(RxBuf[4] == 'f'){
              switch(RxBuf[3]){
                case '1':
                 Blynk.virtualWrite(V5, 0);
                break;
                case '2':
                 Blynk.virtualWrite(V6, 0);
                break;
                case '3':
                 Blynk.virtualWrite(V7, 0);
                break;
                case '4':
                 Blynk.virtualWrite(V8, 0);
                break;
                case '5':
                 Blynk.virtualWrite(V9, 0);
                break;
              }
            }
          }
          RxCounter = 0;
        }
      }
    }
  }
