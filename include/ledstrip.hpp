#pragma once

#include <Arduino.h>
#include <FastLED.h>

// LED STRIP configuration
#define LD_STRIP_PIN 19
#define LD_COLOR_ORDER GRB
#define LD_CHIPSET WS2812B
#define LD_NUM_LEDS 110 // bottom 24, right 31, top 24, left 31 == total 110
#define LD_MAX_BRIGHTNESS 255

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// presets
enum class LEDProgram
{
    ColorPicker,
    ColorSweep,
    Fire,
    Rainbow,
    Confetti,
    Juggle,
    BPM,
    Circle,
    Sine,
    Test,
};

// main routines
void led_setup();
void led_update();
void led_setup_split_indices();

// setters
void led_set_brightness(uint8_t value);
void led_set_color(uint8_t r, uint8_t g, uint8_t b);
void led_set_status(bool status);
void led_set_preset(String id);
void led_set_preset(LEDProgram program);
void led_set_parameter_by_id(String id, uint8_t val);

// getters
uint8_t led_get_brightness();

void led_set_fire_parameters(uint8_t spark, uint8_t cool, uint8_t fps, uint8_t palno);
void led_run_fire();
void led_run_fire_dual();

void led_run_colorsweep();
void led_run_rainbow();
void led_run_confetti();
void led_run_sinelon();
void led_run_circular();
void led_run_bpm();
void led_run_juggle();
