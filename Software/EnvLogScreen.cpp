#include "EnvLogScreen.h"
#include "EnvLogger.h"
#include "Time.h"

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

    pageType=PT_DATE;
    dayNum=LOG_DAYS_NUM-1;
    PaintScreen();
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
            pageType = PT_DATE;
            dayNum--;
            if(dayNum<0 || !CEnvLogger::Inst.GetLogItem(dayNum).IsValid())
            {
                dayNum = LOG_DAYS_NUM-1;
            }
            break;
        case KEY_RIGHT:
            pageType = PT_DATE;
            dayNum=(dayNum+1)%LOG_DAYS_NUM;
            while(!CEnvLogger::Inst.GetLogItem(dayNum).IsValid() && dayNum<LOG_DAYS_NUM-1)
            {
                dayNum++;
            }
            break;
        }
        PaintScreen();
    }
}

void CEnvLogScreen::PaintScreen()
{
    const CEnvLogItem& item=CEnvLogger::Inst.GetLogItem(dayNum);
    char outBuf[2][9]={"        ","        "};

    switch(pageType)
    {
    case PT_DATE:
        tmElements_t tm;
        breakTime(item.Date*(3600L*24),tm);
        sprintf(outBuf[0],"%02d.%02d.%02d",tm.Day,tm.Month,(tm.Year+70)%100);
        sprintf(outBuf[1],"%2d\x00",item.GetMinTemperature());
        sprintf(outBuf[1]+3,"  %2d\x00",item.GetMaxTemperature());
        break;
    case PT_TEMP:
        item.FillTemperatureGraph(outBuf[0]);
        sprintf(outBuf[1],"%2d\x00",item.GetMinTemperature());
        sprintf(outBuf[1]+3,"  %2d\x00",item.GetMaxTemperature());
        break;
    case PT_HUMI:
        item.FillHumidityGraph(outBuf[0]);
        sprintf(outBuf[1],"%2d%%  %2d%%",item.GetMinHumidity(),item.GetMaxHumidity());
        break;
    }

    lcd.setCursor(0,0);
    lcd.print(outBuf[0]);
    lcd.setCursor(0,1);
    for(int i=0;i<8;i++) // workaround to print zero symbols
    {
        lcd.print(outBuf[1][i]);
    }
}
