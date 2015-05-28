#include "ScheduleScreen.h"

const char CScheduleScreen::cursorPositions[2][MAX_SCHEDULE_DATA_NUM]=
    {{3,4,6,7,0,1,2,3,1,2,3,5,6,7},
     {0,0,0,0,1,1,1,1,1,1,1,1,1,1}};
const char CScheduleScreen::minValues[MAX_SCHEDULE_DATA_NUM]={'0','0','0','0',0,0,0,0,0,0,0,'0','0','0'};
const char CScheduleScreen::maxValues[MAX_SCHEDULE_DATA_NUM]={'2','9','5','9',1,1,1,1,1,1,1,'9','9','9'};

CScheduleScreen::CScheduleScreen()
{
    currentChannel=0;
    currentItem=0;
    isInEditMode=false;
    memset(value,0,sizeof(value));
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
    LoadData();
    PrintScreen();
    RefreshMultiplier=2;
    isBlinkingShown=false;
}

void CScheduleScreen::OnClose()
{
    RefreshMultiplier=REFRESH_MULTIPLIER_DEFAULT;
}

byte CScheduleScreen::CalculateWeekdayMask()
{
    byte retVal = 0;
    for(int i=0;i<7;i++)
    {
        retVal|=value[WEEKDAY_DATA_START+i]<<i;
    }
    return retVal;
}

void CScheduleScreen::PrintScreen()
{
    char strTime[6];
    lcd.setCursor(0,0);
    lcd.print((char)('1'+currentChannel));
    lcd.print((char)('a'+currentItem));

    sprintf(strTime,"%c%c:%c%c",value[0],value[1],value[2],value[3]);
    lcd.print((char)6);
    lcd.print(strTime);

    symbolsGenerator.SetWeekday(CalculateWeekdayMask());
    lcd.setCursor(0,1);
    lcd.print("\x02""\x03""\x04""\x05 ");
    lcd.print(value+(WEEKDAY_DATA_START+7));
}

void CScheduleScreen::Refresh()
{
    if(isInEditMode)
    {
        lcd.setCursor(cursorPositions[0][blinkPosition],cursorPositions[1][blinkPosition]);

        if(blinkPosition>=WEEKDAY_DATA_START && blinkPosition<WEEKDAY_DATA_START+7)
        {
            lcd.print(isBlinkingShown ? (char)(2+cursorPositions[0][blinkPosition]) : ' ');
        }
        else
        {
            lcd.print(isBlinkingShown? ValueDigit():' ');
        }
        isBlinkingShown=!isBlinkingShown;
    }
};

void CScheduleScreen::CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged)
{
    if(justPressed)
    {
        if(!isInEditMode)
        {
            switch(justPressed)
            {
            case KEY_OK:
                LoadData();
                blinkPosition=0;
                isInEditMode=true;
                break;
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
        }
        else
        {
            switch(justPressed)
            {
            case KEY_LEFT:
                blinkPosition--;
                if(blinkPosition<0)
                {
                    blinkPosition=MAX_SCHEDULE_DATA_NUM-1;
                }
                break;
            case KEY_RIGHT:
                blinkPosition++;
                if(blinkPosition>=MAX_SCHEDULE_DATA_NUM)
                {
                    blinkPosition=0;
                }
                break;
            case KEY_UP:
                if(ValueDigit()==maxValues[blinkPosition])
                {
                    ValueDigit()=minValues[blinkPosition];
                }
                else
                {
                    ValueDigit()++;
                }
                if(blinkPosition>=WEEKDAY_DATA_START && blinkPosition<WEEKDAY_DATA_START+7)
                {
                    symbolsGenerator.SetWeekday(CalculateWeekdayMask());
                }

                break;
            case KEY_DOWN:
                if(ValueDigit()==minValues[blinkPosition])
                {
                    ValueDigit()=maxValues[blinkPosition];
                }
                else
                {
                    ValueDigit()--;
                }
                if(blinkPosition>=WEEKDAY_DATA_START && blinkPosition<WEEKDAY_DATA_START+7)
                {
                    symbolsGenerator.SetWeekday(CalculateWeekdayMask());
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
        PrintScreen();
    }
}

void CScheduleScreen::SaveData()
{
}

void CScheduleScreen::LoadData()
{
    strcpy(value,"0123");
    memcpy(value+WEEKDAY_DATA_START+7,"456",3);
}
