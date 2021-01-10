#include "InputSwitches.h"

InputSwitches switches;
void setup()
{
    Serial.begin(9600);
    while (!Serial);
}

void loop()
{
    SwitchState s = switches.getCurrentState();
    Serial.println((long)s.mode);
    Serial.println((long)s.direction);
}