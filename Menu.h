#ifndef Menu_h
#define Menu_h

#define LCD_D4 PIN_A4
#define LCD_D5 PIN_A5
#define LCD_D6 PIN_A1
#define LCD_D7 PIN_A0
#define LCD_RS PIN_A2
#define LCD_EN PIN_A3

#include <Arduino.h>
#include <LiquidCrystal.h>
#include "Enumerations.h"

class Menu
{
public:
    Menu();
    void printMenu(SwitchState s, uint8_t distance, uint8_t feed, boolean isEngineRunning);
    void clear();

private:
    void printMode(SwitchState s);
    void printDirection(SwitchState s);
    void printSettings(SwitchState s, uint8_t distance, uint8_t feed);
    void printRunning();
    LiquidCrystal lcd;
};

#endif