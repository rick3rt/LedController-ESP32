# ESP8266 w/ WS2812b

NodeMCU with WS2812b led strip, controllable from android app.

## Basic LED strip control from NodeMCU

```cpp
#include <Arduino.h>
#include <FastLED.h>

#define DEBUG 1

// LED STRIP details
#define LD_STRIP_PIN 3
#define LD_COLOR_ORDER GRB
#define LD_CHIPSET WS2812
#define LD_NUM_LEDS 110 // bottom 24, right 31, top 24, left 31 == total 110

CRGB leds[LD_NUM_LEDS];
uint8_t ledBrightVal = 200;
int ledBrightDir = 1;
CRGB myColor = CRGB::Blue;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    // setup led strip
    FastLED.addLeds<LD_CHIPSET, LD_STRIP_PIN, LD_COLOR_ORDER>(leds, LD_NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(ledBrightVal);
    // set colorpallet to leds
    for (size_t i = 0; i < LD_NUM_LEDS; i++)
        leds[i] = myColor; // myPalette[i % NUM_GROUPS];
}

unsigned long last_LED_update = 0;
unsigned long last_LED_switch = 0;
bool LED_status = false;
void loop()
{
    unsigned long cur_time = millis();

// builtin LED flash (debug)
#if DEBUG
    if (cur_time - last_LED_switch >= 500)
    {
        last_LED_switch = cur_time;
        LED_status = !LED_status;              // toggle bool
        digitalWrite(LED_BUILTIN, LED_status); // and turn led off or on
    }
#endif

    // update LED stip
    FastLED.setBrightness(ledBrightVal);
    FastLED.show(); // display this frame
}
```
