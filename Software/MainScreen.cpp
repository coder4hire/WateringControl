#include "MainScreen.h"
#include "TimeManager.h"
#include "EnvLogger.h"

byte CMainScreen::icons[MAIN_ICONS_NUM][7] =
{
    {
        B00000,
        B00000,
        B10010,
        B01100,
        B10010,
        B00000,
        B00000,
    },
    {
        B01100,
        B10010,
        B10010,
        B01100,
        B00000,
        B00000,
        B00000,
    }
};

CMainScreen::CMainScreen()
{
    OnShow(); // Do this for CMainScreen only because it is first shown screen
    channelCheckIndex=0;
}

CMainScreen::~CMainScreen()
{
    //dtor
}

void CMainScreen::OnShow()
{
    for(int i=0;i<MAIN_ICONS_NUM;i++)
    {
        lcd.createChar(i,icons[i]);
    }
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

    //--- Figuring out which working channel to show
    char workingChannel=' ';
    for(int i=0;i<CHANNELS_NUM && workingChannel==' ';i++)
    {
        bool isHPChanBusy = CTimeManager::Inst.IsAnyHPChannelBusy();
        if((isHPChanBusy && CTimeManager::Inst.IsHPChannelBusy(channelCheckIndex+1)) ||
           (!isHPChanBusy && CTimeManager::Inst.IsLPChannelBusy(channelCheckIndex+1)))
        {
            workingChannel='1'+channelCheckIndex;
        }
        channelCheckIndex=channelCheckIndex<CHANNELS_NUM-1 ? channelCheckIndex+1 : 0;
    }

    //--- Printing second row
    byte temp=0,humi=0;
    sprintf(out,"%2d\x01%2d%% %c",CEnvLogger::Inst.Temperature,CEnvLogger::Inst.Humidity,workingChannel);
    lcd.print(out);

}

void CMainScreen::CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged)
{
    switch(justPressed)
    {
        case KEY_OK:
            SwitchScreens(MENU_SCREEN);
            break;
        case KEY_CANCEL|KEY_UP:
            CTimeManager::Inst.StartQuickLaunch(1);
            break;
        case KEY_CANCEL|KEY_LEFT:
            CTimeManager::Inst.StartQuickLaunch(2);
            break;
        case KEY_CANCEL|KEY_RIGHT:
            CTimeManager::Inst.StartQuickLaunch(3);
            break;
        case KEY_CANCEL|KEY_DOWN:
            CTimeManager::Inst.StartQuickLaunch(4);
            break;
        case KEY_CANCEL:
            CTimeManager::Inst.StopQuickLaunch();

    }


// Watchdog testing routine

//    if(justPressed&KEY_CANCEL)
//    {
//        while(1);
//    }
}
