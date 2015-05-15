#include "ScreenBase.h"
#include "MainScreen.h"
#include "MenuScreen.h"
#include "ScheduleScreen.h"
#include "EnvLogScreen.h"
#include "SettingsScreen.h"

CScreenBase* CScreenBase::pScreens[TOTAL_SCREENS_NUM] =
{
    new CMainScreen(),
    new CMenuScreen(),
    new CScheduleScreen(),
    new CEnvLogScreen(),
    new CSettingsScreen()
};

CScreenBase* CScreenBase::pCurrentScreen = pScreens[MAIN_SCREEN];

CScreenBase::CScreenBase()
{
    //ctor
}

CScreenBase::~CScreenBase()
{
    //dtor
}

void CScreenBase::SwitchScreens(EScreenIDs newScreenID)
{
    if(newScreenID>=0 && newScreenID<TOTAL_SCREENS_NUM)
    {
        pCurrentScreen->OnClose();
        pCurrentScreen=pScreens[newScreenID];
        pCurrentScreen->OnShow();
        pCurrentScreen->Refresh();
    }
}
