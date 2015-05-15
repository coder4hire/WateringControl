#include "SettingsScreen.h"

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
    }
};

CSettingsScreen::CSettingsScreen()
{
}

CSettingsScreen::~CSettingsScreen()
{
    //dtor
}

void CSettingsScreen::OnShow()
{
    for(int i=0;i<SETTINGS_ICONS_NUM;i++)
    {
        lcd.createChar(i,icons[i]);
    }
    PaintMenu();
}

void CSettingsScreen::PaintMenu()
{
    lcd.setCursor(0,0);
    lcd.print((char)0);
    lcd.print("Time   ");
    lcd.setCursor(0,1);
    lcd.print(" 07:40  ");
}

void CSettingsScreen::Refresh()
{
};

void CSettingsScreen::CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged)
{
    if(justPressed&KEY_CANCEL)
    {
        SwitchScreens(MENU_SCREEN);
    }
}
