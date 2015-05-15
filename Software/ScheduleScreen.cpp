#include "ScheduleScreen.h"

byte CScheduleScreen::icons[SCHEDULE_ICONS_NUM][7] =
{
    {
        B00100,
        B01110,
        B00100,
        B10101,
        B10101,
        B10001,
        B11111,
    },
    {
        B00000,
        B00000,
        B01001,
        B01101,
        B01011,
        B01001,
        B01001,
    },
    {
        B00010,
        B00111,
        B00010,
        B00010,
        B00010,
        B00111,
        B00010,
    },

};

CScheduleScreen::CScheduleScreen()
{
}

CScheduleScreen::~CScheduleScreen()
{
    //dtor
}

void CScheduleScreen::OnShow()
{
    for(int i=0;i<SCHEDULE_ICONS_NUM;i++)
    {
        lcd.createChar(i,icons[i]);
    }

    lcd.setCursor(0,0);
    lcd.print((char)0);
    lcd.print("1\x01""3\x02""025");
    lcd.setCursor(0,1);
    lcd.print("00:00Mon");
}

void CScheduleScreen::Refresh()
{
};

void CScheduleScreen::CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged)
{
    if(justPressed&KEY_CANCEL)
    {
        SwitchScreens(MENU_SCREEN);
    }
}
