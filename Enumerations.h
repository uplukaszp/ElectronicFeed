#ifndef Enumerations_h
#define Enumerations_h

enum class Direction
{
    Distance,
    Feed,
    None
};

enum class Mode
{
    Forward,
    Backward,
    ForwardBackward
};

struct SwitchState
{
    Mode mode;
    Direction direction;
};

#endif