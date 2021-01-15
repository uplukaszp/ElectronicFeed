#include "Menu.h"
Menu::Menu() : lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7)
{
    lcd.begin(16, 2);
}
void Menu::printMode(SwitchState s)
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
void Menu::clear(){
    lcd.clear();
}
void Menu::printDirection(SwitchState s)
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

void Menu::printSettings(SwitchState s, uint8_t distance, uint8_t feed)
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
void Menu::printRunning()
{
    lcd.setCursor(0, 0);
    lcd.print("START!");
}
void Menu::printMenu(SwitchState s, uint8_t distance, uint8_t feed, boolean isEngineRunning)
{
    if (!isEngineRunning)
    {
        printDirection(s);
        printMode(s);
        printSettings(s, distance, feed);
    }
    else
    {
        printRunning();
    }
}