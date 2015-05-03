#include "MainScreen.h"

CMainScreen::CMainScreen()
:rtcClock(7,6,5)
{
    OnShow(); // Do this for CMainScreen only because it is first shown screen
}

CMainScreen::~CMainScreen()
{
    //dtor
}

void CMainScreen::Refresh()
{
    lcd.setCursor(0,0);
    char out[10];
    tmElements_t tm;
    uint8_t stat = rtcClock.read(tm);
    sprintf(out,"%02d:%02d,%d",tm.Hour,tm.Minute,stat);
    lcd.print(out);

    lcd.setCursor(0,1);
    lcd.print(rtcClock.haltRTC());
}

void CMainScreen::CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged)
{
    if(justPressed&KEY_OK)
    {
        rtcClock.haltRTC(0);
    }

    if(justPressed&KEY_CANCEL)
    {
        rtcClock.haltRTC(1);
    }

}
