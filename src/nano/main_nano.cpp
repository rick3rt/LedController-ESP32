#include <Arduino.h>
#include <FastLED.h>
#include "ledstrip.hpp"

void setup()
{
    Serial.begin(115200);
    Serial.println("Serial started");
    led_setup();
    Serial.println("LED setup done");
    // led_set_color(255, 255, 255);
    led_set_preset(LEDProgram::ColorSweep);
}

static int count = 0;
void loop()
{
    led_update();
    Serial.printf("loop %i", count++);
}
