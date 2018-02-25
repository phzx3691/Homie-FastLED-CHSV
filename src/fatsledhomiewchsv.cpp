#define FASTLED_ESP8266_RAW_PIN_ORDER
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
#include "Arduino.h"
#include <Homie.h>


#define FW_NAME "led-control"
#define FW_VERSION "1.0.2"
#define NUM_LEDS 65
#define LED_TYPE WS2812B
#define LED_PIN D1
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define BRIGHTNESS          250
#define FRAMES_PER_SECOND  120
int SoffitH;
int SoffitS;
int SoffitV;
int scaledH;
int scaledS;
int scaledV;

HomieNode ledNode("led", "led");

bool lightOnHandler(HomieRange range, String value) {
    if (value == "OFF") {
      for( int i = 0; i < NUM_LEDS; i++) {
         leds[i].fadeToBlackBy(64);
         leds[i].CRGB::Black;
         FastLED.show();
      }
    ledNode.setProperty("color").send("OFF");
    } else if (value == "ON") {
        for( int i = 0; i < NUM_LEDS; i++) {
           leds[i].CRGB::Gold;
           FastLED.show();
        }
    ledNode.setProperty("color").send("ON");
    } else {
    SoffitH = value.substring(0,value.indexOf(',')).toInt();
    SoffitS = value.substring(value.indexOf(',')+1,value.lastIndexOf(',')).toInt();
    SoffitV = value.substring(value.lastIndexOf(',')+1).toInt();
    scaledH = map(SoffitH, 0, 359, 0, 255);
    scaledS = map(SoffitS, 0, 100, 0, 255);
    scaledV = map(SoffitV, 0, 100, 0, 255);
    for( int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CHSV(scaledH,scaledS,scaledV);
      FastLED.show();
    }
  }
  ledNode.setProperty("color").send(value);
  return true;
  return value;
}


void loopHandler() {


  //FastLED.show();
  //FastLED.delay(1000 / FRAMES_PER_SECOND);
}

void setup() {
  Serial.begin(115200);
  Serial.println(FW_NAME FW_VERSION);
  Homie_setFirmware(FW_NAME, FW_VERSION);
  ledNode.advertise("color").settable(lightOnHandler);
  //ledNode.advertise("anime").settable(animationHandler);
  //Homie.setLoopFunction(loopHandler);
  Homie.setup();
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  Homie.loop();
}
