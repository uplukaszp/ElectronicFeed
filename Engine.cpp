#include "Engine.h"
volatile uint32_t EngineClass::stepsToStop;
volatile uint32_t EngineClass::step;
unsigned long EngineClass::delay;
volatile uint8_t EngineClass::stepVal;

void EngineClass::init()
{
    pinMode(STEP, OUTPUT);
    pinMode(DIR, OUTPUT);
    pinMode(EN, OUTPUT);
    digitalWrite(DIR, LOW);
    digitalWrite(EN, LOW);
}
void EngineClass::setEnabled(Direction d)
{
    // if (d == Direction::None)
    //     digitalWrite(EN, LOW);
    // else
    //     digitalWrite(EN, HIGH);
}
void EngineClass::doTick()
{
    if (step < stepsToStop)
    {
        digitalWrite(STEP, stepVal);
        if (stepVal == LOW){
            stepVal = HIGH;
            step++;
        }
        else
            stepVal = LOW;
        
    }
    else
    {
        stopEngine();
    }
}
void EngineClass::startEngine(Mode m, uint16_t feed, uint16_t distance)
{
    delay = calculateDelay(feed);
    stepsToStop = calculateNumberOfSteps(distance);
    stepVal = LOW;
    step = 0;
    digitalWrite(DIR, (m == Mode::Forward) ? LOW : HIGH);
    Timer1.initialize(delay);
    Timer1.attachInterrupt(doTick);
}

void EngineClass::stopEngine()
{
    Timer1.detachInterrupt();
}

uint32_t EngineClass::calculateDelay(uint16_t feed)
{
    return 28571 / feed;
}

uint32_t EngineClass::calculateNumberOfSteps(uint16_t distance)
{
    return distance * (STEPS_PER_ROTATION/RATIO);
}