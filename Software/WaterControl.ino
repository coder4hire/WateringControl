#include <Arduino.h>
/************************************************************
*  Display pinout:  * shift register: 74HC164
*   1 - CLK         * latch on LOW-to-HIGH clock transition
*   2 - DAT         * Q0 - DB7 * Q4 - DB3 * Q8  - RS
*   3 - n/c         * Q1 - DB6 * Q5 - DB2 * gnd - RW
*   4 - GND         * Q2 - DB5 * Q6 - DB1 *
*   5 - E           * Q3 - DB4 * Q7 - DB0 *
*   6 - +5VDC       *
*************************************************************/
#include "LiquidCrystal2.h" // include updated library code:
#include "string.h"
#include "KeysParser.h"

#include "ScreenBase.h"
#include "MainScreen.h"

#define QUANT_DELAY 100
#define REFRESH_DELAY_MULTIPLIER 5

LiquidCrystal lcd(4, 3, 2); //shift input (clk-dat-E)
byte smiley[8] =
{
    B01110,
    B10101,
    B10101,
    B10101,
    B11001,
    B10001,
    B01110,
};

byte arrow[8] =
{
    B10000,
    B00100,
    B11110,
    B11111,
    B11110,
    B00100,
    B10000,
};

void setup()
{
    lcd.createChar(1,smiley);
    lcd.begin(8, 2);

//  pinMode(7, INPUT);
//  pinMode(9, OUTPUT);
}

void loop()
{
    static int refreshCounter=0;

    // Processing keypresses
    EKeys justPressed;
    EKeys justReleased;
    bool isChanged = CKeysParser::Inst.PerformCheck(justPressed,justReleased);
    CScreenBase::GetCurrentScreen()->CheckKeys(CKeysParser::Inst.GetKeyMap(),justPressed,justReleased,isChanged);

    // Processing refreshes
    if(++refreshCounter>REFRESH_DELAY_MULTIPLIER)
    {
        refreshCounter=0;
        CScreenBase::GetCurrentScreen()->Refresh();
    }

    delay(100);
}

void ShowKeys()
{
    EKeys justPressed;
    EKeys justReleased;
    CKeysParser::Inst.PerformCheck(justPressed,justReleased);
    EKeys keys=CKeysParser::Inst.GetKeyMap();
    char strKeys[]="ULRODC";
    for(int i=0;i<6;i++)
    {
        if(!(keys&(1<<i)))
        {
            strKeys[i]=' ';
        }
    }

    lcd.setCursor(0,1);
    lcd.print(strKeys);

}

//  if(!digitalRead(7))
//  {
//    motorCountDown=11;
//    digitalWrite(9,1);
//  }

//  lcd.setCursor(0, 1); lcd.print(motorCountDown ? "\x1" : " ");

//    if(motorCountDown)
//    {
//        motorCountDown--;
//        if(motorCountDown==1)
//        {
//            digitalWrite(9,0);
//        }
//    }
