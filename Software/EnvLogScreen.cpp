#include "EnvLogScreen.h"
#include "EnvLogger.h"

byte CEnvLogScreen::icons[ENVLOG_ICONS_NUM][7] =
{
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

CEnvLogScreen::CEnvLogScreen()
{
    for(int i=1;i<ENVLOG_ICONS_NUM;i++)
    {
        for(int j=0;j<(7-i);j++)
        {
            icons[i][j]=0;
        }
        for(int j=(7-i);j<7;j++)
        {
            icons[i][j]=0xFF;
        }
    }
}

CEnvLogScreen::~CEnvLogScreen()
{
    //dtor
}

void CEnvLogScreen::OnShow()
{
    for(int i=0;i<ENVLOG_ICONS_NUM;i++)
    {
        lcd.createChar(i,icons[i]);
    }

    lcd.setCursor(0,0);
    lcd.print("\x01\x02\x03\x04\x05\x06\x07 ");
    lcd.setCursor(0,1);
    lcd.print("10");
    lcd.print((char)0);
    lcd.print("20");
    lcd.print((char)0);
    lcd.print("30");

    pageType=PT_DATE;
    dayNum=LOG_DAYS_NUM-1;
    Refresh();
}

void CEnvLogScreen::Refresh()
{
};

void CEnvLogScreen::CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged)
{
    if(justPressed)
    {
        switch(justPressed)
        {
        case KEY_OK:
            break;
        case KEY_CANCEL:
            SwitchScreens(MENU_SCREEN);
            return;
        case KEY_UP:
            pageType=(EPageType)((pageType+PT_MAX-1)%PT_MAX);
            break;
        case KEY_DOWN:
            pageType=(EPageType)((pageType+1)%PT_MAX);
            break;
        case KEY_LEFT:
            dayNum--;
            if(dayNum<0 || !CEnvLogger::Inst.GetLogItem(dayNum).IsValid())
            {
                dayNum = LOG_DAYS_NUM;
            }
            break;
        case KEY_RIGHT:
            dayNum=(dayNum+1)%LOG_DAYS_NUM;
            while(!CEnvLogger::Inst.GetLogItem(dayNum).IsValid() && dayNum<LOG_DAYS_NUM-1)
            {
                dayNum++;
            }
            break;
        }
    }
}
