#include "ledstrip.hpp"

CRGB leds[NUM_LEDS];
CRGB defaultColor = CRGB::Blue;
uint8_t led_brightness = 255;
bool led_status = true;
// LEDProgram currentPreset = LEDProgram::ColorPicker;
LEDProgram led_current_preset = LEDProgram::Fire;

// LED index arrays to separate left and right of the single strip
uint8_t led_idx1[NUM_LEDS / 2]; // LED index array for right side of painting
uint8_t led_idx2[NUM_LEDS / 2]; // LED index array for left side of painting

// preset variables
// Fire
uint8_t led_fire_sparks = 120; // suggested range 50-200.
uint8_t led_fire_cooling = 55; // suggested range 20-100
uint8_t led_fire_fps = 60;
uint8_t led_fire_palette_no = 0;

void led_setup()
{
    // setup led strip
    FastLED.addLeds<CHIPSET, LEDSTRIP_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(MIN(led_brightness, MAX_BRIGHTNESS));
    // set colorpallet to leds
    for (size_t i = 0; i < NUM_LEDS; i++)
        leds[i] = defaultColor; // myPalette[i % NUM_GROUPS];

    led_setup_split_indices();
}

/**
 * @brief setup index array to adress the left and right half
 * of the led strip, with division in the center.
 * (indexing starts in bottom left corner, and goes counterclockwise)
 */
void led_setup_split_indices()
{
    const int NH = NUM_LEDS / 2;
    const int width = 24;
    // const int height = 31;

    for (int i = 0; i < NH; i++)
    {
        led_idx1[i] = i + width / 2;
        led_idx2[i] = NUM_LEDS + width / 2 - i - 1;
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
        case LEDProgram::Fire:
            random16_add_entropy(random());
            FastLED.setBrightness(MAX_BRIGHTNESS);
            led_run_fire_dual();
            FastLED.show(); // display this frame
            FastLED.delay(1000 / led_fire_fps);

            break;

        case LEDProgram::ColorPicker:
        default:
            FastLED.setBrightness(MIN(led_brightness, MAX_BRIGHTNESS));
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
    for (size_t i = 0; i < NUM_LEDS; i++)
        leds[i] = CRGB(r, g, b);

    FastLED.show(); // display this frame
}

void led_set_preset(String id)
{
    if (id.equals("colorpicker"))
        led_current_preset = LEDProgram::ColorPicker;
    else if (id.equals("fire"))
        led_current_preset = LEDProgram::Fire;
    else
        led_current_preset = LEDProgram::ColorPicker;
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
    led_fire_fps = fps;
    led_fire_palette_no = pnum;
}

void led_run_fire()
{
    // Array of temperature readings at each simulation cell
    static uint8_t heat[NUM_LEDS];

    // Step 1.  Cool down every cell a little
    for (int i = 0; i < NUM_LEDS; i++)
        heat[i] = qsub8(heat[i], random8(0, ((led_fire_cooling * 10) / NUM_LEDS) + 2));

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (int k = NUM_LEDS - 1; k >= 2; k--)
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < led_fire_sparks)
    {
        int y = random8(7);
        heat[y] = qadd8(heat[y], random8(160, 255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (int j = 0; j < NUM_LEDS; j++)
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
    const int nheat = NUM_LEDS / 2;

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
