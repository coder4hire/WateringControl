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
    char row[9]="        ";
    for(int i=0;i<CHANNELS_NUM;i++)
    {
        if(CTimeManager::Inst.IsChannelBusy(i+1))
        {
            row[i]='1'+i;
        }
    }
    lcd.print(row);

}

void CMainScreen::CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged)
{
    if(justPressed&KEY_OK)
    {
        SwitchScreens(MENU_SCREEN);
    }


// Watchdog testing routine

//    if(justPressed&KEY_CANCEL)
//    {
//        while(1);
//    }
}
