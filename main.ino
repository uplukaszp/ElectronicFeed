
#include <LiquidCrystal.h>
#include <Encoder.h>

#include "InputSwitches.h"
#include "Enumerations.h"
#include "Menu.h"
#include "Engine.h"
#define SW 3
#define CLK 2
#define DT 5

InputSwitches switches;
Menu m;
Encoder encoder(CLK, DT);
SwitchState s;
uint16_t distance = 1;
uint16_t feed = 1;
int32_t lastPos;
boolean isRunning = false;
volatile unsigned long last_interrupt_time = millis();
uint8_t stepVal = LOW;
volatile boolean shouldChange = false;

void handleClick()
{
    if (millis() - last_interrupt_time > 500)
    {
        if (shouldChange)
            shouldChange = false;
        else
            shouldChange = true;
    }
}
void setup()
{
    lastPos = encoder.read();
    pinMode(SW, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(SW), handleClick, FALLING);
    EngineClass::init();
}
uint16_t readValueToAdd()
{
    int32_t currentPos = encoder.read();
    if (currentPos % 4 != 0)
        return 0;
    uint16_t valueToAdd = (currentPos == lastPos) ? 0 : (currentPos > lastPos) ? 1 : -1;
    lastPos = currentPos;
    return valueToAdd;
}
void updateValues()
{
    uint16_t valueToAdd = readValueToAdd();
    switch (s.direction)
    {
    case Direction::Feed:
        if (feed > 30 && feed < 100)
            valueToAdd *= 2;
        if (feed >= 100)
            valueToAdd *= 5;
        feed += valueToAdd;
        if (feed >= 2000)
            feed = 1;
        if (feed <= 0)
            feed = 1999;
        break;
    case Direction::Distance:
        if (distance >= 300)
            distance = 1;
        if (distance <= 0)
            distance = 299;
        distance += valueToAdd;
        break;
    default:
        break;
    }
}

void loop()
{
    s = switches.getCurrentState();
    updateValues();
    if (!isRunning)
        EngineClass::setEnabled(s.direction);
    if (shouldChange)
    {
        Serial.print("clear");
        delay(100);
        m.clear();
        shouldChange = false;
        if (isRunning)
        {
            isRunning = false;
            EngineClass::stopEngine();
        }
        else
        {
            isRunning = true;
            EngineClass::startEngine(s.mode, feed, distance);
        }
        last_interrupt_time = millis();
    }
    m.printMenu(s, distance, feed, isRunning);
}
