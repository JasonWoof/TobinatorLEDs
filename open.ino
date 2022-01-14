#include "leds.h"
#define NUM_LEDS 174
#define DATA_PIN 6



CRGB leds[NUM_LEDS];
void setup() {
    memset8(leds, 0, NUM_LEDS * sizeof(CRGB));
    delay(1000);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(255);
}



SOLID_COLOR(off, CRGB(0, 0, 0));

ANIMATION(slowRainbow, 120, -5, {
    {40, CRGB(255, 0, 0)},
    {40, CRGB(0, 255, 0)},
    {40, CRGB(0, 0, 255)},
});
ANIMATION(rollingWink, 28, -4, {
    {8, CHSV(0, 0, 0)},
    {4, CHSV(0, 0, 255)},
    {16, CHSV(0, 0, 255)},
});
CRGB OPEN (uint16_t time, uint16_t i, uint8_t letter) {
    uint8_t bright = rollingWink(time, letter).r;
    CRGB color = slowRainbow(time, letter);
    return CRGB(
        ((uint16_t)color.r) * bright / 255,
        ((uint16_t)color.g) * bright / 255,
        ((uint16_t)color.b) * bright / 255
    );
}

ANIMATION(border, 60, -5, {
    {10, CRGB(255, 0, 0)},
    {10, CRGB(, 180, 0)},
    {10, CRGB(0, 0, 0)},
    {30, CRGB(0, 0, 0)},
});


void loop() {
    RUNS_START
        RUN(90, border);
        RUN(19, OPEN, 0); // O
        RUN(19, OPEN, 1); // P
        RUN(21, OPEN, 2); // E
        RUN(25, OPEN, 3); // N
    RUNS_END

    FastLED.show();
    delay(10);
}