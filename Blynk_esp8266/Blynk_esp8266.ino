
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

  int _V5 = 0;
  char auth[] = "Ar70t5_E6aE24FtelCeWAH0EmAORsBwf";
  char ssid[] = "Nhat Nam";
  char pass[] = "0989339608";
  WidgetLED led_Connected(V0);
  WidgetLED led(V20);
  BlynkTimer led_timer;
    void blinkLedWidget(){
    if (led_Connected.getValue()) {
      led_Connected.off();
    } else {
      led_Connected.on();
    }
  }
  BLYNK_WRITE(V5){//////////////////////////////////LED1/////////////////
    _V5= param.asInt(); 
    digitalWrite(D0, _V5);
    if (_V5) {
      led.on();
    } else {
      led.off();
    }
  }
void setup(){
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  led_timer.setInterval(1000L, blinkLedWidget);
  pinMode(D0, OUTPUT);
  digitalWrite(D0, LOW);
}
void loop(){
  Blynk.run();
  led_timer.run();
}
