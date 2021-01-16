#ifndef Engine_h
#include <Arduino.h>
#include <TimerOne.h>
#include "Enumerations.h"
#define Engine_h

#define DIR 11
#define STEP 12
#define EN 10

#define STEPS_PER_ROTATION 6400
#define RATIO 3.0
class EngineClass
{
public:
    static void init();
    static void setEnabled(Direction d);
    static void startEngine(Mode m, uint16_t feed, uint16_t distance);
    static void stopEngine();

private:
    static void doTick();
    static uint32_t calculateDelay(uint16_t feed);
    static uint32_t calculateNumberOfSteps(uint16_t distance);
    static volatile uint32_t stepsToStop;
    static volatile uint32_t step;
    static unsigned long delay;
    static volatile uint8_t stepVal;
};
#endif