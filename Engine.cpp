#include "Engine.h"
volatile uint32_t EngineClass::stepsToStop;
volatile uint32_t EngineClass::step;
unsigned long EngineClass::delay;
volatile uint8_t EngineClass::stepVal;
volatile uint16_t EngineClass::distance;
volatile uint16_t EngineClass::feed;
volatile Mode EngineClass::mode;

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
        stopSequence();
    }
}
void EngineClass::startEngine(Mode m, uint16_t f, uint16_t d)
{
    mode=m;
    feed=f;
    distance=d;
    delay = calculateDelay();
    stepsToStop = calculateNumberOfSteps();
    stepVal = LOW;
    step = 0;
    digitalWrite(DIR, (m == Mode::Backward) ? LOW : HIGH);
    Timer1.initialize(delay);
    Timer1.attachInterrupt(doTick);
}

void EngineClass::stopEngine()
{
    Timer1.detachInterrupt();
}

uint32_t EngineClass::calculateDelay()
{
    return 28571 / feed;
}

uint32_t EngineClass::calculateNumberOfSteps()
{
    return distance * (STEPS_PER_ROTATION/RATIO);
}

void EngineClass::stopSequence(){
    if(mode==Mode::ForwardBackward){
        step=0;
        digitalWrite(DIR,!digitalRead(DIR));
    }else
    {
        stopEngine();
    }
    
}