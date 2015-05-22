#include "ScheduleScreen.h"

//byte CScheduleScreen::icons[SCHEDULE_ICONS_NUM][7] =
//{
//    {
//        B00100,
//        B01110,
//        B00100,
//        B10101,
//        B10101,
//        B10001,
//        B11111,
//    },
//    {
//        B00000,
//        B00000,
//        B01001,
//        B01101,
//        B01011,
//        B01001,
//        B01001,
//    },
//    {
//        B00010,
//        B00111,
//        B00010,
//        B00010,
//        B00010,
//        B00111,
//        B00010,
//    },
//};

CScheduleScreen::CScheduleScreen()
{
    currentChannel=0;
    currentItem=0;
    isInEditMode=false;
}

CScheduleScreen::~CScheduleScreen()
{
    //dtor
}

void CScheduleScreen::OnShow()
{
//    for(int i=0;i<SCHEDULE_ICONS_NUM;i++)
//    {
//        lcd.createChar(i,icons[i]);
//    }

    symbolsGenerator.Start();
    //symbolsGenerator.SetChannel(1);
    symbolsGenerator.SetWeekday(0x7f);
    PrintScreen();
}

void CScheduleScreen::PrintScreen()
{
    lcd.setCursor(0,0);
    lcd.print((char)('1'+currentChannel));
    lcd.print((char)('a'+currentItem));
    lcd.print("\x06""01:25");
    lcd.setCursor(0,1);
    lcd.print("\x02""\x03""\x04""\x05 333");
}

void CScheduleScreen::Refresh()
{
};

void CScheduleScreen::CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged)
{
    if(justPressed)
    {
        if(!isInEditMode)
        {
            switch(justPressed)
            {
            case KEY_CANCEL:
                SwitchScreens(MENU_SCREEN);
                return;
            case KEY_UP:
                currentChannel=(currentChannel+1)%CHANNELS_NUM;
//                symbolsGenerator.SetChannel(currentChannel);
                break;
            case KEY_DOWN:
                currentChannel=(currentChannel+CHANNELS_NUM-1)%CHANNELS_NUM;
//                symbolsGenerator.SetChannel(currentChannel);
                break;
            case KEY_LEFT:
                currentItem=(currentItem+MAX_ITEMS_PER_CHANNEL-1)%MAX_ITEMS_PER_CHANNEL;
                break;
            case KEY_RIGHT:
                currentItem=(currentItem+1)%MAX_ITEMS_PER_CHANNEL;
                break;
            }
            PrintScreen();
        }
    }
}
