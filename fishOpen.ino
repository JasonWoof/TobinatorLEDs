#include "leds.h"
#define NUM_LEDS 321
#define DATA_PIN 6
#define BRIGHTNESS 255



// hue is 0-255 (unlike inkscape, which is 0-360)
CRGB
    cRed = CHSV(0, 255, 128),
    cPink = CHSV(212, 255, 128),
    cBlindingWhite = CHSV(0, 0, 220),
    cWhite = CHSV(0, 0, 80),
    cDimWhite = CHSV(0, 0, 50),
    cGreen = CHSV(84, 255, 128),
    cDarkGreen = CHSV(84, 255, 50),
    cBlue = CHSV(170, 255, 128),
    cYellow = CHSV(63, 255, 128),
    cCyan = CHSV(127, 255, 128),
    cBlack = CRGB(0, 0, 0);
CRGB
    cLeftStripe = cYellow,
    cRightStripe = cYellow;

const CRGB blue(uint8_t brightness) {
    if (brightness <= 128) {
        return CHSV(170, 255, brightness);
    } else {
        return CHSV(170, 255 - (brightness - 128) * 2, 128);
    }
}


CRGB leds[NUM_LEDS];
void setup() {
    memset8(leds, 0, NUM_LEDS * sizeof(CRGB));
    delay(1000);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
}





CRGB solidColor (uint16_t time, uint16_t i, CRGB color) {
    return color;
}
CRGB unused (uint16_t time, uint16_t i) {
    return cBlack;
}
ANIMATION(leftBubbleQuick, 66, -4, {
    {2, cWhite},
    {2, cWhite},
    {2, cBlack},
    {60, cBlack},
});
ANIMATION(rightBubbleQuick, 77, -5, {
    {2, cWhite},
    {3, cWhite},
    {2, cBlack},
    {70, cBlack},
});
ANIMATION(rightBubbleSlow, 198, -11, {
    {5, cWhite},
    {5, cWhite},
    {5, cBlack},
    {183, cBlack},
});
ANIMATION(leftBubbleSlow, 207, -11, {
    {5, cWhite},
    {5, cWhite},
    {5, cBlack},
    {192, cBlack},
});
CRGB mergeMaxGrayscale (CRGB a, CRGB b) {
    uint8_t value = a.r > b.r ? a.r : b.r;
    return CRGB(value, value, value);
}
CRGB leftBubbles (uint16_t time, uint16_t i) {
    return mergeMaxGrayscale(leftBubbleQuick(time, i), leftBubbleSlow(time, i));
}
CRGB rightBubbles (uint16_t time, uint16_t i) {
    return mergeMaxGrayscale(rightBubbleQuick(time, i), rightBubbleSlow(time, i));
}

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
ANIMATION(wave, 84, -6, {
    {30, CRGB(80, 0, 0)},
    {27, CRGB(20, 0, 0)},
    {20, CRGB(140, 0, 0)},
    {7, CRGB(20, 0, 0)},
});
CRGB water (uint16_t time, uint8_t pixel) {
    uint8_t bright = wave(time, pixel).r + ((pixel + 2) % 5) * 23;
    return blue(bright);
}

ANIMATION(rightFish, 53, 1, {
    {11, CHSV(212, 255, 128)},
    {11, CHSV(212, 255, 100)},
    {11, CHSV(212, 255, 128)},
    {10, CHSV(232, 255, 128)},
    {10, CHSV(232, 255, 100)},
});

ANIMATION(leftFish, 48, 1, {
    { 9, CHSV(212, 255, 128)},
    { 9, CHSV(212, 255, 100)},
    {10, CHSV(212, 255, 128)},
    {10, CHSV(232, 255, 128)},
    {10, CHSV(232, 255, 100)},
});

ANIMATION(leftWink, 267, 0, {
    { 255, cCyan},
    { 0, cBlack},
    { 2, cBlack},
    { 2, cCyan},
    { 4, cCyan},
    { 0, cBlack},
    { 2, cBlack},
    { 2, cCyan},
});

ANIMATION(rightWink, 387, 0, {
    { 165, cCyan},
    { 210, cCyan},
    { 0, cBlack},
    { 2, cBlack},
    { 2, cCyan},
    { 4, cCyan},
    { 0, cBlack},
    { 2, cBlack},
    { 2, cCyan},
});


void loop() {
    RUNS_START
        RUN(70, water);
        RUN(53, rightFish);
        RUN(10, solidColor, cRightStripe);
        RUN( 3, rightWink);
        RUN(10, rightBubbles);
        RUN( 1, unused);
        RUN(25, OPEN, 3); // N
        RUN(21, OPEN, 2); // E
        RUN(19, OPEN, 1); // P
        RUN(19, OPEN, 0); // O
        RUN(10, leftBubbles);
        RUN( 1, unused);
        RUN(48, leftFish);
        RUN( 3, leftWink);
        RUN(24, solidColor, cLeftStripe);
    RUNS_END

    FastLED.show();
    // delay(4);
}