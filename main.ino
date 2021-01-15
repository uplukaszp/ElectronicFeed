#include <TimerOne.h>

#include <LiquidCrystal.h>
#include <Encoder.h>

#include "InputSwitches.h"
#include "Enumerations.h"

#define LCD_D4 PIN_A4
#define LCD_D5 PIN_A5
#define LCD_D6 PIN_A1
#define LCD_D7 PIN_A0
#define LCD_RS PIN_A2
#define LCD_EN PIN_A3
#define SW 3
#define CLK 2
#define DT 5
#define dirPin 11
#define stepPin 12
#define en 10

InputSwitches switches;
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);
Encoder encoder(CLK, DT);

SwitchState s;
uint8_t distance = 0;
uint8_t feed = 0;
int32_t lastPos;
boolean lastIsRunning = false;
boolean isRunning = false;
unsigned last_interrupt_time = millis();
uint8_t stepVal = LOW;

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
void stopEngine(){
    Timer1.detachInterrupt();
}
void handleClick()
{
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > 500)
    {
        if (isRunning)
            isRunning = false;
        else
            isRunning = true;
        Serial.println(isRunning);
        if (isRunning)
        {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("START!");
            runEngine();
        }else {
            stopEngine();
        }
    }
    last_interrupt_time = interrupt_time;
}
void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    lcd.begin(16, 2);
    lastPos = encoder.read();
    pinMode(SW, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(SW), handleClick, LOW);
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
    if (!isRunning)
    {
        printDirection();
        printMode();
        printSettings();
    }
}

void loop()
{
    s = switches.getCurrentState();
    updateValues();
    printMenu();
    delay(50);
}
