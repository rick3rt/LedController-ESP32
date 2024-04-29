#include "ledstrip.hpp"

CRGB leds[LD_NUM_LEDS];
CRGB led_default_color = CRGB::White;
uint8_t led_brightness = 255;
bool led_status = true;
LEDProgram led_current_preset = LEDProgram::ColorSweep;

// LED index arrays to separate left and right of the single strip
uint8_t led_idx1[LD_NUM_LEDS / 2]; // LED index array for right side of painting
uint8_t led_idx2[LD_NUM_LEDS / 2]; // LED index array for left side of painting

// preset variables

// general
uint8_t led_animation_fps = 60;
uint8_t led_animation_hue = 0; // rotating "base color" used by many of the patterns
uint8_t led_animation_hue_speed = 1;
uint8_t led_animation_hue_increment = 1;
uint8_t led_animation_bpm = 60;

// Fire
uint8_t led_fire_sparks = 120; // suggested range 50-200.
uint8_t led_fire_cooling = 55; // suggested range 20-100
uint8_t led_fire_palette_no = 0;

void led_setup()
{
    // setup led strip
    FastLED.addLeds<LD_CHIPSET, LD_STRIP_PIN, LD_COLOR_ORDER>(leds, LD_NUM_LEDS);
    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.clear();
    FastLED.setBrightness(MIN(led_brightness, LD_MAX_BRIGHTNESS));

    // set colorpallet to leds
    for (size_t i = 0; i < LD_NUM_LEDS; i++)
        leds[i] = led_default_color; // myPalette[i % NUM_GROUPS];

    led_setup_split_indices();
    led_set_preset("colorsweep");
}

/**
 * @brief setup index array to adress the left and right half
 * of the led strip, with division in the center.
 * (indexing starts in bottom left corner, and goes counterclockwise)
 */
void led_setup_split_indices()
{
    const int NH = LD_NUM_LEDS / 2;
    const int width = 24;
    // const int height = 31;

    for (int i = 0; i < NH; i++)
    {
        led_idx1[i] = i + width / 2;
        led_idx2[i] = LD_NUM_LEDS + width / 2 - i - 1;
    }
    for (int i = 0; i < width / 2; i++)
        led_idx2[i] = width / 2 - 1 - i;

    // Serial.println("led_idx1");
    // for (int i = 0; i < NH; i++)
    //     Serial.println(led_idx1[i]);
    // Serial.println("led_idx2");
    // for (int i = 0; i < NH; i++)
    //     Serial.println(led_idx2[i]);
}

void led_update()
{
    // update LED stip
    if (led_status)
    {
        switch (led_current_preset)
        {
        case LEDProgram::ColorPicker:
            FastLED.setBrightness(MIN(led_brightness, LD_MAX_BRIGHTNESS));
            FastLED.show(); // display this frame
            break;

        case LEDProgram::ColorSweep:
            led_run_colorsweep();
            FastLED.setBrightness(MIN(led_brightness, LD_MAX_BRIGHTNESS));
            EVERY_N_MILLISECONDS(led_animation_hue_speed) { led_animation_hue += led_animation_hue_increment; } // slowly cycle the "base color" through the rainbow
            FastLED.show();                                                                                     // display this frame
            FastLED.delay(1000 / led_animation_fps);
            break;

        case LEDProgram::Fire:
            random16_add_entropy(random());
            FastLED.setBrightness(LD_MAX_BRIGHTNESS);
            led_run_fire_dual();
            FastLED.show(); // display this frame
            FastLED.delay(1000 / led_animation_fps);
            break;

        case LEDProgram::Rainbow:
            FastLED.setBrightness(MIN(led_brightness, LD_MAX_BRIGHTNESS));
            led_run_rainbow();
            FastLED.show(); // display this frame
            FastLED.delay(1000 / led_animation_fps);
            EVERY_N_MILLISECONDS(led_animation_hue_speed) { led_animation_hue += led_animation_hue_increment; } // slowly cycle the "base color" through the rainbow
            break;

        case LEDProgram::Confetti:
            FastLED.setBrightness(MIN(led_brightness, LD_MAX_BRIGHTNESS));
            led_run_confetti();
            FastLED.show(); // display this frame
            FastLED.delay(1000 / led_animation_fps);
            EVERY_N_MILLISECONDS(led_animation_hue_speed) { led_animation_hue += led_animation_hue_increment; } // slowly cycle the "base color" through the rainbow
            break;

        case LEDProgram::Test:
            FastLED.setBrightness(MIN(led_brightness, LD_MAX_BRIGHTNESS));
            led_run_circular();
            FastLED.show(); // display this frame
            FastLED.delay(1000 / led_animation_fps);
            EVERY_N_MILLISECONDS(led_animation_hue_speed) { led_animation_hue += led_animation_hue_increment; } // slowly cycle the "base color" through the rainbow
            break;

        case LEDProgram::Sine:
            FastLED.setBrightness(MIN(led_brightness, LD_MAX_BRIGHTNESS));
            led_run_sinelon();
            FastLED.show(); // display this frame
            FastLED.delay(1000 / led_animation_fps);
            EVERY_N_MILLISECONDS(led_animation_hue_speed) { led_animation_hue += led_animation_hue_increment; } // slowly cycle the "base color" through the rainbow
            break;

        case LEDProgram::BPM:
            FastLED.setBrightness(MIN(led_brightness, LD_MAX_BRIGHTNESS));
            led_run_bpm();
            FastLED.show(); // display this frame
            FastLED.delay(1000 / led_animation_fps);
            EVERY_N_MILLISECONDS(led_animation_hue_speed) { led_animation_hue += led_animation_hue_increment; } // slowly cycle the "base color" through the rainbow
            break;

        case LEDProgram::Juggle:
            FastLED.setBrightness(MIN(led_brightness, LD_MAX_BRIGHTNESS));
            led_run_juggle();
            FastLED.show(); // display this frame
            FastLED.delay(1000 / led_animation_fps);
            EVERY_N_MILLISECONDS(led_animation_hue_speed) { led_animation_hue += led_animation_hue_increment; } // slowly cycle the "base color" through the rainbow
            break;

        default:
            FastLED.setBrightness(0);
            FastLED.show(); // display this frame
            break;
        }
    }
    else
    {
        FastLED.setBrightness(0);
        FastLED.show(); // display this frame}
    }
}

void led_set_brightness(uint8_t value)
{
    led_brightness = value;
}

void led_set_status(bool status)
{
    led_status = status;
}

void led_set_color(uint8_t r, uint8_t g, uint8_t b)
{
    for (size_t i = 0; i < LD_NUM_LEDS; i++)
        leds[i] = CRGB(r, g, b);

    FastLED.show(); // display this frame
}

void led_set_preset(LEDProgram preset)
{
    led_current_preset = preset;
}

void led_set_preset(String id)
{
    if (id.equals("colorpicker"))
        led_current_preset = LEDProgram::ColorPicker;
    else if (id.equals("colorsweep"))
        led_current_preset = LEDProgram::ColorSweep;
    else if (id.equals("fire"))
        led_current_preset = LEDProgram::Fire;
    else if (id.equals("rainbow"))
        led_current_preset = LEDProgram::Rainbow;
    else if (id.equals("confetti"))
        led_current_preset = LEDProgram::Confetti;
    else if (id.equals("sine"))
        led_current_preset = LEDProgram::Sine;
    else if (id.equals("juggle"))
        led_current_preset = LEDProgram::Juggle;
    else if (id.equals("bpm"))
        led_current_preset = LEDProgram::BPM;
    else if (id.equals("test"))
        led_current_preset = LEDProgram::Test;
    else
        led_current_preset = LEDProgram::ColorPicker;
}

void led_set_parameter_by_id(String param_id, uint8_t val)
{
    if (param_id.equals("fps"))
        led_animation_fps = val;
    else if (param_id.equals("huespeed"))
        led_animation_hue_speed = val;
    else if (param_id.equals("hueinc"))
        led_animation_hue_increment = val;
    else if (param_id.equals("spark"))
        led_fire_sparks = val;
    else if (param_id.equals("cool"))
        led_fire_cooling = val;
    else if (param_id.equals("palno"))
        led_fire_palette_no = val;
    else if (param_id.equals("bpm"))
        led_animation_bpm = val;
}

uint8_t led_get_brightness()
{
    return led_brightness;
}

// PRESESTS
CRGBPalette16 led_color_palettes[11] = {
    HeatColors_p,
    CRGBPalette16(CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White),
    CRGBPalette16(CRGB::Black, CRGB::Blue, CRGB::Aqua, CRGB::White),
    CRGBPalette16(CRGB::Black, CRGB::Red, CRGB::White),
    CloudColors_p,
    LavaColors_p,
    OceanColors_p,
    ForestColors_p,
    RainbowColors_p,
    RainbowStripeColors_p,
    PartyColors_p,
};

void led_set_fire_parameters(uint8_t spark, uint8_t cool, uint8_t fps, uint8_t pnum)
{
    led_fire_sparks = spark;
    led_fire_cooling = cool;
    led_fire_palette_no = pnum;

    led_animation_fps = fps;
}

void led_run_fire()
{
    // Array of temperature readings at each simulation cell
    static uint8_t heat[LD_NUM_LEDS];

    // Step 1.  Cool down every cell a little
    for (int i = 0; i < LD_NUM_LEDS; i++)
        heat[i] = qsub8(heat[i], random8(0, ((led_fire_cooling * 10) / LD_NUM_LEDS) + 2));

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = LD_NUM_LEDS - 1; k >= 2; k--)
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < led_fire_sparks)
    {
        int y = random8(7);
        heat[y] = qadd8(heat[y], random8(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (int j = 0; j < LD_NUM_LEDS; j++)
    {
        // Scale the heat value from 0-255 down to 0-240
        // for best results with color palettes.
        uint8_t colorindex = scale8(heat[j], 240);
        CRGB color = ColorFromPalette(led_color_palettes[led_fire_palette_no], colorindex);
        leds[j] = color;
    }
}

void led_run_fire_dual()
{
    const int nheat = LD_NUM_LEDS / 2;

    // Array of temperature readings at each simulation cell
    static uint8_t heat1[nheat];
    static uint8_t heat2[nheat];

    // Step 1.  Cool down every cell a little
    for (int i = 0; i < nheat; i++)
    {
        heat1[i] = qsub8(heat1[i], random8(0, ((led_fire_cooling * 10) / nheat) + 2));
        heat2[i] = qsub8(heat2[i], random8(0, ((led_fire_cooling * 10) / nheat) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = nheat - 1; k >= 2; k--)
    {
        heat1[k] = (heat1[k - 1] + heat1[k - 2] + heat1[k - 2]) / 3;
        heat2[k] = (heat2[k - 1] + heat2[k - 2] + heat2[k - 2]) / 3;
    }
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < led_fire_sparks)
    {
        int y = random8(7);
        heat1[y] = qadd8(heat1[y], random8(160, 255));
        y = random8(7);
        heat2[y] = qadd8(heat2[y], random8(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (int j = 0; j < nheat; j++)
    {
        // Scale the heat value from 0-255 down to 0-240
        // for best results with color palettes.
        uint8_t colorindex = scale8(heat1[j], 240);
        CRGB color = ColorFromPalette(led_color_palettes[led_fire_palette_no], colorindex);
        leds[led_idx1[j]] = color;
        // and do same for second array
        colorindex = scale8(heat2[j], 240);
        color = ColorFromPalette(led_color_palettes[led_fire_palette_no], colorindex);
        leds[led_idx2[j]] = color;
    }
}

// =============================================================================
// OTHER
// =============================================================================

void led_run_colorsweep()
{
    for (int i = 0; i < LD_NUM_LEDS; i++)
        leds[i] = CHSV(led_animation_hue, 255, 255);
}

void led_run_rainbow()
{
    // FastLED's built-in rainbow generator
    fill_rainbow(leds, LD_NUM_LEDS, led_animation_hue, 7);
}

void led_addGlitter(fract8 chanceOfGlitter)
{
    if (random8() < chanceOfGlitter)
    {
        leds[random16(LD_NUM_LEDS)] += CRGB::White;
    }
}

void led_run_rainbowWithGlitter()
{
    // built-in FastLED rainbow, plus some random sparkly glitter
    led_run_rainbow();
    led_addGlitter(80);
}

void led_run_confetti()
{
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy(leds, LD_NUM_LEDS, 10);
    int pos = random16(LD_NUM_LEDS);
    leds[pos] += CHSV(led_animation_hue + random8(64), 200, 255);
}

void led_run_sinelon()
{
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy(leds, LD_NUM_LEDS, 20);
    int pos = beatsin16(13, 0, LD_NUM_LEDS - 1);
    leds[pos] += CHSV(led_animation_hue, 255, 192);
}

void led_run_circular()
{
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy(leds, LD_NUM_LEDS, 20);
    int pos = beatsin16(13, 0, LD_NUM_LEDS * 2 - 1);
    pos = pos % LD_NUM_LEDS;
    leds[pos] += CHSV(led_animation_hue, 255, 192);
}

void led_run_bpm()
{
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = led_animation_bpm;
    CRGBPalette16 palette = led_color_palettes[led_fire_palette_no];
    uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
    for (int i = 0; i < LD_NUM_LEDS; i++)
    { // 9948
        leds[i] = ColorFromPalette(palette, led_animation_hue + (i * 2), beat - led_animation_hue + (i * 10));
    }
}

void led_run_juggle()
{
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy(leds, LD_NUM_LEDS, 20);
    uint8_t dothue = 0;
    for (int i = 0; i < 8; i++)
    {
        leds[beatsin16(i + 7, 0, LD_NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
        dothue += 32;
    }
}
