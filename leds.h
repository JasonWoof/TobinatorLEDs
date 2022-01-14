#include <FastLED.h>

// struct[] to struct*
#define PTR(array) (&(array[0]))

// number of elemnts in struct[]
#define ARRAY_COUNT(a) (sizeof(a) / sizeof(a[0]))



#define ANIMATION(name, frameCount, seqOffset,...) struct animationStop name##Stops[] = __VA_ARGS__; struct animationSequence name##Seq {ARRAY_COUNT(name##Stops), frameCount, seqOffset, PTR(name##Stops)}; CRGB name (uint16_t time, uint16_t offset) { return animate(&name##Seq, time, offset); }

#define SOLID_COLOR(color) const CRGB name##Color = color; CRGB name (uint16_t time, uint16_t offset) { return name##Color; }

struct animationStop {
    uint16_t frameCount;
    CRGB toColor;
};

struct animationSequence {
    uint8_t stopCount;
    uint16_t frameCount;
    int8_t offset;
    struct animationStop *stops;
};


CRGB animate (struct animationSequence *seq, uint16_t time, uint16_t offset) {
    uint8_t stopCount = seq->stopCount;
    struct animationStop *stops = seq->stops;
    uint16_t relTime = (time + offset * seq->offset) % seq->frameCount;
    uint8_t stopI = 0;
    while (relTime >= stops[stopI].frameCount) {
        relTime -= stops[stopI].frameCount;
        stopI += 1;
    }
    struct animationStop *prev = &(stops[(stopI + stopCount - 1) % stopCount]);
    if (relTime == 0) {
        return prev->toColor;
    }
    struct animationStop *cur = &(stops[stopI]);
    return CRGB(
        (((uint16_t)prev->toColor.r) * (cur->frameCount - relTime) + ((uint16_t)cur->toColor.r) * (relTime)) / cur->frameCount,
        (((uint16_t)prev->toColor.g) * (cur->frameCount - relTime) + ((uint16_t)cur->toColor.g) * (relTime)) / cur->frameCount,
        (((uint16_t)prev->toColor.b) * (cur->frameCount - relTime) + ((uint16_t)cur->toColor.b) * (relTime)) / cur->frameCount
    );
}

#define RUNS_START { static uint16_t loopCounter = 1000; uint16_t ledI = 0;
#define RUNS_END ++loopCounter;}
#define RUN(ledCount, fn, ...) for (uint16_t i = 0; i < ledCount; ++i) { leds[ledI++] = fn(loopCounter, i, ##__VA_ARGS__); }
