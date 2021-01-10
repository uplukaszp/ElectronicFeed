#ifndef InputSwitches_h
#define InputSwitches_h

#include <Arduino.h>
#include "Enumerations.h"

#define SWITCH_1_UP_PIN 7
#define SWITCH_1_DOWN_PIN 9
#define SWITCH_2_UP_PIN 8
#define SWITCH_2_DOWN_PIN 6

class InputSwitches
{
public:
    InputSwitches();
    SwitchState getCurrentState();

private:
    Direction readDirection();
    Mode readMode();
};

#endif