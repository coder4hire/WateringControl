#include "SettingsScreen.h"
#include "TimeManager.h"

const char CSettingsScreen::cursorPositions[MAX_SETTINGS_NUM][8]={{0,1,3,4,-1},{0,1,3,4,6,7,-1},{4,5,6,7,-1},{7,-1}};
const char CSettingsScreen::minValues[MAX_SETTINGS_NUM][8]={{'0','0','0','0',-1},{'0','0','0','0','0','0',-1},{'N','N','N','N',-1},{'N',-1}};
const char CSettingsScreen::maxValues[MAX_SETTINGS_NUM][8]={{'2','9','5','9',-1},{'3','9','1','9','9','9',-1},{'Y','Y','Y','Y',-1},{'Y',-1}};
const char CSettingsScreen::settingsNames[MAX_SETTINGS_NUM][9]={"Time    ","Date    ","HP:1234 ","Reset   "};
const char CSettingsScreen::maxDays[12]={31,28,31,30,31,30,31,31,30,31,30,31};

const byte CSettingsScreen::icons[SETTINGS_ICONS_NUM][7] =
{
    {
        B01110,
        B10101,
        B10101,
        B10101,
        B10011,
        B10001,
        B01110,
    },
    {
        B11111,
        B11011,
        B10011,
        B11011,
        B11011,
        B10001,
        B11111,
    },
    {
        B00010,
        B00100,
        B01000,
        B11110,
        B00100,
        B01000,
        B10000,
    }
};

CSettingsScreen::CSettingsScreen()
{
    isInEditMode=false;
    blinkPosition=0;
    isBlinkingShown=0;

    value[0]=0;
}

CSettingsScreen::~CSettingsScreen()
{
    //dtor
}

void CSettingsScreen::OnShow()
{
    settingNum=0;
    isInEditMode=false;
    for(int i=0;i<SETTINGS_ICONS_NUM;i++)
    {
        lcd.createChar(i,(uint8_t*)icons[i]);
    }
    LoadData();
    PaintMenu();

    RefreshMultiplier=2;
}

void CSettingsScreen::OnClose()
{
    RefreshMultiplier=REFRESH_MULTIPLIER_DEFAULT;
}

void CSettingsScreen::PaintMenu()
{
    lcd.setCursor(0,0);
    lcd.print((char)settingNum);
    lcd.print(settingsNames[settingNum]);
    lcd.setCursor(0,1);
    lcd.print("        ");
    lcd.setCursor(0,1);
    lcd.print(value);
}

void CSettingsScreen::Refresh()
{
    if(isInEditMode)
    {
        lcd.setCursor(cursorPositions[settingNum][blinkPosition],1);
        lcd.print(isBlinkingShown? ValueDigit() : ' ');
        isBlinkingShown=!isBlinkingShown;
    }
};

void CSettingsScreen::CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged)
{
    if(justPressed)
    {
        if(!isInEditMode)
        {
            switch(justPressed)
            {
            case KEY_LEFT:
                settingNum=(settingNum+MAX_SETTINGS_NUM-1)%MAX_SETTINGS_NUM;
                LoadData();
                break;
            case KEY_RIGHT:
                settingNum=(settingNum+1)%MAX_SETTINGS_NUM;
                LoadData();
                break;
            case KEY_OK:
                LoadData();
                blinkPosition=0;
                isInEditMode=true;
                break;
            case KEY_CANCEL:
                SwitchScreens(MENU_SCREEN);
                return;
            }
        }
        else
        {
            switch(justPressed)
            {
            case KEY_LEFT:
                blinkPosition--;
                if(blinkPosition<0)
                {
                    int i=0;
                    for(;i<8 && cursorPositions[settingNum][i]!=-1;i++);
                    blinkPosition=i-1;
                }
                break;
            case KEY_RIGHT:
                blinkPosition++;
                if(cursorPositions[settingNum][blinkPosition]==-1)
                {
                    blinkPosition=0;
                }
                break;
            case KEY_UP:
                if(settingNum==SETTING_RESET_SCHED || settingNum==SETTING_HIGH_POWER)
                {
                    ValueDigit()= ValueDigit()==maxValues[settingNum][blinkPosition] ? minValues[settingNum][blinkPosition] : maxValues[settingNum][blinkPosition];
                    break;
                }

                if(ValueDigit()==maxValues[settingNum][blinkPosition])
                {
                    ValueDigit()=minValues[settingNum][blinkPosition];
                }
                else
                {
                    ValueDigit()++;
                }
                break;
            case KEY_DOWN:
                if(settingNum==SETTING_RESET_SCHED || settingNum==SETTING_HIGH_POWER)
                {
                    ValueDigit()= ValueDigit()==maxValues[settingNum][blinkPosition] ? minValues[settingNum][blinkPosition] : maxValues[settingNum][blinkPosition];
                    break;
                }

                if(ValueDigit()==minValues[settingNum][blinkPosition])
                {
                    ValueDigit()=maxValues[settingNum][blinkPosition];
                }
                else
                {
                    ValueDigit()--;
                }
                break;
            case KEY_OK:
                SaveData();
                isInEditMode=false;
                break;
            case KEY_CANCEL:
                LoadData();
                isInEditMode=false;
                break;
            }
        }
        PaintMenu();
    }
}

void CSettingsScreen::SaveData()
{
    switch(settingNum)
    {
    case SETTING_TIME:
        {
            value[2]=0;
            int hour = atoi(value);
            int minute = atoi(value+3);
            value[2]=':';
            if(hour>=0 && hour<24 && minute>=0 && minute<60)
            {
                CTimeManager::Inst.SetTime(hour,minute);
            }
            else
            {
                LoadData();
            }
        }
        break;
    case SETTING_DATE:
        {
            value[2]=0;
            value[5]=0;
            int day = atoi(value);
            int month = atoi(value+3);
            int year = atoi(value+6);
            value[2]='.';
            value[5]='.';
            bool isLeapYearFebruary = month==2 && !(year%4); // 2000 is a Leap Year because it is divisible by 400
            if(month>=1 && month<=12 && day>=1 && (day<=maxDays[month-1] || (isLeapYearFebruary && day<=29)))
            {
                CTimeManager::Inst.SetDate(day,month,year);
            }
            else
            {
                LoadData();
            }
        }
        break;
    case SETTING_HIGH_POWER:
        for(int i=0;i<4;i++)
        {
            CTimeManager::Inst.IsHighPower[i]=(value[4+i]=='Y');
        }
        CTimeManager::Inst.SaveLowPowerSettings();
        break;
    case SETTING_RESET_SCHED:
        if(value[7]=='Y')
        {
            CTimeManager::Inst.ResetAllSchedules();
        }
        break;
    }
}

void CSettingsScreen::LoadData()
{
    switch(settingNum)
    {
    case SETTING_TIME:
        CTimeManager::Inst.GetTimeString(value);
        break;
    case SETTING_DATE:
        CTimeManager::Inst.GetDateString(value);
        break;
    case SETTING_HIGH_POWER:
        memset(value,' ',8);
        value[8]=0;
        for(int i=0;i<4;i++)
        {
            value[4+i] = CTimeManager::Inst.IsHighPower[i] ? 'Y' : 'N';
        }
        break;
    case SETTING_RESET_SCHED:
        strcpy(value,"Sched: N");
        break;
    }
}
