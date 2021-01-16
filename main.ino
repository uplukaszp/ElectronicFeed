#include <TimerOne.h>

#include <LiquidCrystal.h>
#include <Encoder.h>

#include "InputSwitches.h"
#include "Enumerations.h"
#include "Menu.h"

#define SW 3
#define CLK 2
#define DT 5
#define dirPin 11
#define stepPin 12
#define en 10

InputSwitches switches;
Menu m;
Encoder encoder(CLK, DT);
SwitchState s;
uint8_t distance = 1;
uint8_t feed = 1;
int32_t lastPos;
boolean isRunning = false;
volatile unsigned long last_interrupt_time = millis();
uint8_t stepVal = LOW;
volatile boolean shouldChange = false;

void engineTick()
{
    digitalWrite(stepPin, stepVal);
    if (stepVal == LOW)
        stepVal = HIGH;
    else
        stepVal = LOW;
}
void runEngine()
{
    Timer1.initialize(feed);
    Timer1.attachInterrupt(engineTick);
}
void stopEngine()
{
    Timer1.detachInterrupt();
}
void handleClick()
{
    if (millis() - last_interrupt_time > 500)
    {
        if (shouldChange)
            shouldChange = false;
        else
            shouldChange = true;
    }

    //     unsigned long interrupt_time = millis();

    //     if (interrupt_time - last_interrupt_time > 500)
    //     {
    //         last_interrupt_time = interrupt_time;
    //         if (isRunning)
    //             isRunning = false;
    //         else
    //             isRunning = true;
    //         Serial.println(isRunning);
    //         if (isRunning)
    //         {
    //             m.clear();
    //             runEngine();
    //         }
    //         else
    //         {
    //             m.clear();
    //             stopEngine();
    //         }
    //     }
}
void setup()
{
    Serial.begin(9600);

    lastPos = encoder.read();
    pinMode(SW, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(SW), handleClick, FALLING);
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(stepPin, OUTPUT);
    pinMode(en, OUTPUT);
    // Set the spinning direction CW/CCW:
    digitalWrite(dirPin, HIGH);
    digitalWrite(en, LOW);
}
uint32_t readValueToAdd()
{
    int32_t currentPos = encoder.read();
    uint8_t valueToAdd = (currentPos == lastPos) ? 0 : (currentPos > lastPos) ? 1 : -1;
    lastPos = currentPos;
    return valueToAdd;
}
void updateValues()
{
    uint8_t valueToAdd = readValueToAdd();
    switch (s.direction)
    {
    case Direction::Feed:
        feed += valueToAdd;
        break;
    case Direction::Distance:
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
    if (shouldChange)
    {
        Serial.print("clear");
        delay(100);
        m.clear();
        shouldChange = false;
        if (isRunning)
            isRunning = false;
        else
            isRunning = true;
        last_interrupt_time = millis();
    }
    m.printMenu(s, distance, feed, isRunning);
    // Serial.println(digitalRead(SW));
}
