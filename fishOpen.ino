#include <FastLED.h>
#define NUM_LEDS 321
#define DATA_PIN 6
#define BRIGHTNESS 255

// hue is 0-255 (unlike inkscape, which is 0-360)
CRGB cRed = CHSV(0, 255, 128);
CRGB cBlindingWhite = CHSV(0, 0, 220);
CRGB cWhite = CHSV(0, 0, 80);
CRGB cDimWhite = CHSV(0, 0, 50);
CRGB cGreen = CHSV(84, 255, 128);
CRGB cDarkGreen = CHSV(84, 255, 50);
CRGB cPink = CHSV(212, 255, 128);
CRGB cCyan = CHSV(127, 255, 128);
CRGB cYellow = CHSV(63, 255, 128);
CRGB cOrange = CHSV(20, 255, 128);
const CRGB blue(byte brightness) {
    if (brightness <= 128) {
        return CHSV(170, 255, brightness);
    } else {
        return CHSV(170, 255 - (brightness - 128) * 2, 128);
    }
}
CRGB cBlue = CHSV(170, 255, 128);
CRGB cBlues[4];
CRGB cOpen[4];
CRGB cFoot = CHSV(16, 100, 128);
CRGB cEye = CHSV(63, 90, 88);
CRGB cFace = CHSV(16, 100, 60);
CRGB cBeak = CHSV(16, 100, 40);
CRGB cOff = CHSV(0, 0, 0);

CRGB cLeftBubbles[10];
CRGB cRightBubbles[10];
CRGB cWater[70];

CRGB
    *lb = &(cLeftBubbles[0]),
    *le = &cCyan,
    *lf = &cPink,
    *ls = &cYellow,
    *o = &(cOpen[0]),
    *p = &(cOpen[1]),
    *e = &(cOpen[2]),
    *n = &(cOpen[3]),
    *rb = &(cRightBubbles[0]),
    *re = &cCyan,
    *rf = &cPink,
    *rs = &cYellow,
    *water = &(cWater[0]),
    *off = &cOff;

typedef struct {
    byte count;
    CRGB** color;
    byte colorInc;
} ledRun;

ledRun runs[] = {
    {70, &water, 1},
    {53, &rf, 0},
    {10, &rs, 0},
    { 3, &re, 0},
    {10, &rb, 1},
    { 1, &off, 0},
    {25, &n, 0},
    {21, &e, 0},
    {19, &p, 0},
    {19, &o, 0},
    {10, &lb, 1},
    { 1, &off, 0},
    {48, &lf, 0},
    { 3, &le, 0},
    {24, &ls, 0},
};

#define ARRAY_COUNT(a) (sizeof(a) / sizeof(a[0]))

CRGB leds[NUM_LEDS];

void setup() {
    memset8(leds, 0, NUM_LEDS * sizeof(CRGB));
    delay(1000);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
}

#define FLASH_FRAME_COUNT 8
#define TIME_TO_FLASH ((loopCounter % FLASH_FRAME_COUNT) == 0)


unsigned int loopCounter = 0;
CRGB **open[] = {&o, &p, &e, &n};
byte openPhase = 0;
CRGB bubbleColors[] = {
    CHSV(0, 0, 0),
    cWhite,
    CHSV(0, 0, 0),
};
void loop() {
    // bubbles
    #define LBM 57
    for (byte i = 0; i < 10; ++i) {
        unsigned int phase = loopCounter % LBM;
        cLeftBubbles[i] = (i == (phase * 10) / LBM) ? cWhite : cOff;
    }
    #define RBM 49
    for (byte i = 0; i < 10; ++i) {
        unsigned int phase = loopCounter % RBM;
        cRightBubbles[i] = (i == (phase * 10) / RBM) ? cWhite : cOff;
    }

    // waves
    for (byte i = 0; i < ARRAY_COUNT(cWater); ++i) {
        cWater[i] = blue(
            30 + // min brightness
            ((7 + i) % 5) * 30 + // brighten based on height
            ((loopCounter - 5 * i) % 50) * 2 // brighten based on timing
        );
    }


    // "OPEN": rainbow/caterpiler
    for (byte i = 0; i < 4; ++i) {
        unsigned int lci = loopCounter - i * 5;
        unsigned int bright = lci % 32;
        if (bright < 8) {
            bright = 16 - 2 * bright;
        } else if (bright < 12) {
            bright = (bright - 8) * 4;
        } else {
            bright = 16;
        }
        cOpen[i] = CHSV(lci - openPhase * 50, 255, 31 + bright * 14);
    }

    // set the led colors according to the areas
    for (unsigned int runI = 0, ledI = 0; runI < ARRAY_COUNT(runs); ++runI) {
        ledRun *run = &(runs[runI]);
        for (int i = 0, colorI = 0; i < run->count; ++i, ++ledI, colorI += run->colorInc) {
            leds[ledI] = (*(run->color))[colorI];
        }
    }

    FastLED.show();

    loopCounter += 1;
    delay(11);
}