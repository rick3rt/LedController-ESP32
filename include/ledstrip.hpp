#ifndef LEDSTRIP_HPP
#define LEDSTRIP_HPP

#include <Arduino.h>
#include <FastLED.h>

// LED STRIP details
#define LEDSTRIP_PIN 3
#define COLOR_ORDER GRB
#define CHIPSET WS2812
#define NUM_LEDS 110 // bottom 24, right 31, top 24, left 31 == total 110

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX_BRIGHTNESS 100

// main routines
void led_setup();
void led_update();
void led_setup_split_indices();

// setters
void led_set_brightness(uint8_t value);
void led_set_color(uint8_t r, uint8_t g, uint8_t b);
void led_set_status(bool status);
void led_set_preset(String id);

// getters
uint8_t led_get_brightness();

// presets
enum class LEDProgram
{
    ColorPicker,
    Fire,
};

void led_set_fire_parameters(uint8_t spark, uint8_t cool, uint8_t fps, uint8_t palno);
void led_run_fire();
void led_run_fire_dual();

#endif //! LEDSTRIP_HPP