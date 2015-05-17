#include "MainScreen.h"
#include "TimeManager.h"

CMainScreen::CMainScreen()
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
    CTimeManager::Inst.GetTimeString(out);
    lcd.print(out);
    lcd.print(' ');
    lcd.print(CTimeManager::Inst.GetWeekDayString());
    lcd.setCursor(0,1);
    lcd.print("        ");

}

void CMainScreen::CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged)
{
    if(justPressed&KEY_OK)
    {
        SwitchScreens(MENU_SCREEN);
    }
}
