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
#include "TimeManager.h"
#include "EnvLogger.h"
#include <avr/wdt.h>

#define QUANT_DELAY 100

byte RefreshMultiplier=5;
byte EnvironmentMultiplier=50;

LiquidCrystal lcd(4, 3, 2); //shift input (clk-dat-E)

void setup()
{
    wdt_disable();
    lcd.begin(8, 2);

    pinMode(CHAN_PIN1, OUTPUT);
    pinMode(CHAN_PIN2, OUTPUT);
    pinMode(CHAN_PIN3, OUTPUT);
    pinMode(CHAN_PIN4, OUTPUT);

    digitalWrite(CHAN_PIN1,LOW);
    digitalWrite(CHAN_PIN2,LOW);
    digitalWrite(CHAN_PIN3,LOW);
    digitalWrite(CHAN_PIN4,LOW);

    RefreshMultiplier=REFRESH_MULTIPLIER_DEFAULT;

    Serial.begin(9600);
    Serial.println("Initialising...");
    delay(100); //Allow for serial print to complete.

    //--- Setup the Watchdog Timer --------------------------------------------
    MCUSR &= ~(1<<WDRF);

    /* In order to change WDE or the prescaler, we need to
    * set WDCE (This will allow updates for 4 clock cycles).
    */
    WDTCSR |= (1<<WDCE) | (1<<WDE);

    /* set new watchdog timeout prescaler value */
    WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */

    /* Enable the WD interrupt (note no reset). */
    WDTCSR |= _BV(WDIE);

    //--- Showing first screen
    CScreenBase::GetCurrentScreen()->SwitchScreens(MAIN_SCREEN);
}

void loop()
{
    static int refreshCounter=0;
    static int envCounter=0;

    // Processing keypresses
    EKeys justPressed;
    EKeys justReleased;
    bool isChanged = CKeysParser::Inst.PerformCheck(justPressed,justReleased);
    CScreenBase::GetCurrentScreen()->CheckKeys(CKeysParser::Inst.GetKeyMap(),justPressed,justReleased,isChanged);

    // Processing refreshes
    if(++refreshCounter>RefreshMultiplier)
    {
        refreshCounter=0;
        CTimeManager::Inst.CheckEvents();
        CScreenBase::GetCurrentScreen()->Refresh();
    }

    // Processing environment readings
    if(++envCounter>RefreshMultiplier)
    {
        envCounter=0;
        CEnvLogger::Inst.RefreshData();
    }

    wdt_reset();
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


ISR(WDT_vect)
{
    //--- This is watchdog timer handler which is called after device is hanged up longer than 8 seconds
    //--- TODO: Save some status here (into time chip memory) to indicate that chip was reset by WDT

    //--- This command will reset the chip (reset PC to 0)
    asm volatile ("  jmp 0");
}
