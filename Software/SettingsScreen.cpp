#include "SettingsScreen.h"

char CSettingsScreen::cursorPositions[MAX_SETTINGS_NUM][8]={{0,1,3,4,-1},{0,1,3,4,7,8,-1}};
char CSettingsScreen::minValues[MAX_SETTINGS_NUM][8]={{'0','0','0','0',-1},{'0','0','0','0','0','0',-1}};
char CSettingsScreen::maxValues[MAX_SETTINGS_NUM][8]={{'2','9','5','9',-1},{'3','9','1','9','9','9',-1}};
char CSettingsScreen::settingsNames[MAX_SETTINGS_NUM][8]={"Time   ","Date   "};

byte CSettingsScreen::icons[SETTINGS_ICONS_NUM][7] =
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
        B10101,
        B10001,
        B01110,
        B11111,
        B01110,
        B01010,
        B10101,
    }
};

CSettingsScreen::CSettingsScreen()
{
    isInEditMode=false;
    blinkingData=0;
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
    for(int i=0;i<SETTINGS_ICONS_NUM;i++)
    {
        lcd.createChar(i,icons[i]);
    }
    LoadData();
    PaintMenu();
}

void CSettingsScreen::PaintMenu()
{
    lcd.setCursor(0,0);
    lcd.print((char)settingNum);
    lcd.print(settingsNames[settingNum]);
    lcd.setCursor(0,1);
    lcd.print(value);
}

void CSettingsScreen::Refresh()
{
    if(isInEditMode)
    {
        lcd.setCursor(cursorPositions[settingNum][blinkPosition],1);
        lcd.print(isBlinkingShown? blinkingData:' ');
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
                break;
            case KEY_RIGHT:
                settingNum=(settingNum+1)%MAX_SETTINGS_NUM;
                break;
            case KEY_OK:
                LoadData();
                isInEditMode=true;
            case KEY_CANCEL:
                SwitchScreens(MENU_SCREEN);
                break;
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
                blinkingData=value[cursorPositions[settingNum][blinkPosition]];
                break;
            case KEY_RIGHT:
                blinkPosition++;
                if(cursorPositions[settingNum][blinkPosition])
                {
                    blinkPosition=0;
                }
                blinkingData=value[cursorPositions[settingNum][blinkPosition]];
                break;
            case KEY_UP:
                if(blinkingData==maxValues[settingNum][blinkPosition])
                {
                    blinkingData==minValues[settingNum][blinkPosition];
                }
                else
                {
                    blinkingData++;
                }
                break;
            case KEY_DOWN:
                if(blinkingData==minValues[settingNum][blinkPosition])
                {
                    blinkingData==maxValues[settingNum][blinkPosition];
                }
                else
                {
                    blinkingData--;
                }
                break;
            case KEY_OK:
                SaveData();
                isInEditMode=false;
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

}

void CSettingsScreen::LoadData()
{
    switch(settingNum)
    {
    case SETTING_TIME:
        strcpy(value,"00:00");
        break;
    case SETTING_DATE:
        strcpy(value,"01-01-15");
        break;
    }
}
