#include "InputSwitches.h"

InputSwitches::InputSwitches()
{
    pinMode(SWITCH_1_UP_PIN, INPUT_PULLUP);
    pinMode(SWITCH_1_DOWN_PIN, INPUT_PULLUP);
    pinMode(SWITCH_2_UP_PIN, INPUT_PULLUP);
    pinMode(SWITCH_2_DOWN_PIN, INPUT_PULLUP);
}

SwitchState InputSwitches::getCurrentState()
{
    SwitchState s;
    s.direction = this->readDirection();
    s.mode = this->readMode();
    return s;
}

Direction InputSwitches::readDirection()
{
    int dirDown = digitalRead(SWITCH_1_DOWN_PIN);
    int dirUP = digitalRead(SWITCH_1_UP_PIN);
    if (dirDown == LOW)
        return Direction::Distance;
    if (dirUP == LOW)
        return Direction::Feed;
    return Direction::None;
}

Mode InputSwitches::readMode()
{
    int modeDOWN = digitalRead(SWITCH_2_DOWN_PIN);
    int modeUP = digitalRead(SWITCH_2_UP_PIN);
    if (modeDOWN == LOW)
        return Mode::Forward;
    if (modeUP == LOW)
        return Mode::Backward;
    return Mode::ForwardBackward;
}