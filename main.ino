#include <LiquidCrystal.h>
#include "InputSwitches.h"
#include "Enumerations.h"

#define LCD_D4 PIN_A4
#define LCD_D5 PIN_A5
#define LCD_D6 PIN_A1
#define LCD_D7 PIN_A0
#define LCD_RS PIN_A2
#define LCD_EN PIN_A3

InputSwitches switches;
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
SwitchState s;
uint8_t distance = 0;
uint8_t feed = 0;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    lcd.begin(16, 2);
}

void printMode()
{
    Direction d = s.direction;
    lcd.setCursor(0, 1);
    switch (d)
    {
    case Direction::Feed:
        lcd.print("Posuw: ");
        break;
    case Direction::Distance:
        lcd.print("Droga: ");
        break;
    case Direction::None:
        lcd.print("BLOKADA     ");
        break;
    default:
        break;
    }
}
void printDirection()
{
    Mode m = s.mode;

    lcd.setCursor(0, 0);
    lcd.print("Kier.:");
    lcd.setCursor(7, 0);
    switch (m)
    {
    case Mode::Forward:
        lcd.print("P  ");
        break;
    case Mode::Backward:
        lcd.print("T  ");
        break;
    case Mode::ForwardBackward:
        lcd.print("P-T");
        break;
    default:
        break;
    }
}

void printSettings()
{
    Direction d = s.direction;
    lcd.setCursor(7, 1);
    switch (d)
    {
    case Direction::Feed:
        lcd.print(feed);
        lcd.print(" mm/min    ");
        break;
    case Direction::Distance:
        lcd.print(distance);
        lcd.print(" mm        ");
        break;
    case Direction::None:
        lcd.print("           ");
        break;
    default:
        break;
    }
}
void printMenu()
{
    printDirection();
    printMode();
    printSettings();
}

void loop()
{
    s = switches.getCurrentState();
    printMenu();
}
