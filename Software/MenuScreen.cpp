#include "MenuScreen.h"

byte CMenuScreen::icons[ICONS_NUM][8] =
{
    {
        B01110,
        B10101,
        B10101,
        B10101,
        B11001,
        B10001,
        B01110,
    },
    {
        B00000,
        B10101,
        B01110,
        B11111,
        B01110,
        B10101,
        B00000,
    },
    {
        B11110,
        B11111,
        B00100,
        B00100,
        B00100,
        B00100,
        B00100,
    },
    {
        B00001,
        B00011,
        B00111,
        B00111,
        B00111,
        B00011,
        B00001,
    },
    {
        B10000,
        B11000,
        B11100,
        B11100,
        B11100,
        B11000,
        B10000,
    }
};

char cursorPatterns[ICONS_NUM_DISPLAYED][5]={"\x03\x04  "," \x03\x04 ","  \x03\x04"};

char CMenuScreen::description[ICONS_NUM_DISPLAYED][9]={"SCHEDULE","ENV.LOGS","SETTINGS"};


CMenuScreen::CMenuScreen()
{
}

CMenuScreen::~CMenuScreen()
{
    //dtor
}

void CMenuScreen::OnShow()
{
    for(int i=0;i<ICONS_NUM;i++)
    {
        lcd.createChar(i,icons[i]);
    }

    currentItem=0;
    oldItem=-1;

    lcd.setCursor(0,0);
    lcd.print("  ");
    for(int i=0;i<ICONS_NUM;i++)
    {
        lcd.print((char)(i));
        lcd.print(' ');
    }
    lcd.print(' ');
    Refresh();
}

void CMenuScreen::Refresh()
{
    if(oldItem!=currentItem)
    {
        oldItem=currentItem;
        for(int i=0;i<4;i++)
        {
            lcd.setCursor(i*2+1,0);
            lcd.print((char)(cursorPatterns[currentItem][i]));
        }
        lcd.setCursor(0,1);
        lcd.print(description[currentItem]);
    }
};

void CMenuScreen::CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged)
{
    if(justPressed&KEY_CANCEL)
    {
        SwitchScreens(MAIN_SCREEN);
    }

    if(justPressed&KEY_RIGHT)
    {
        currentItem=(currentItem+1)%ICONS_NUM_DISPLAYED;
        Refresh();
    }
    if(justPressed&KEY_LEFT)
    {
        currentItem=(currentItem-1+ICONS_NUM_DISPLAYED)%ICONS_NUM_DISPLAYED;
        Refresh();
    }
    if(justPressed&KEY_OK)
    {
    }
}
