#include "ScheduleScreen.h"

const char CScheduleScreen::cursorPositions[2][MAX_SCHEDULE_DATA_NUM]=
    {{3,4,6,7,0,1,2,3,1,2,3,5,6,7},
     {0,0,0,0,1,1,1,1,1,1,1,1,1,1}};
const char CScheduleScreen::minValues[MAX_SCHEDULE_DATA_NUM]={'0','0','0','0',0,0,0,0,0,0,0,'0','0','0'};
const char CScheduleScreen::maxValues[MAX_SCHEDULE_DATA_NUM]={'2','9','5','9',1,1,1,1,1,1,1,'9','9','h'};

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
    lcd.print(CTimeManager::Inst.IsHighPower[currentChannel] ? (char)6 :' ');
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
                if(value[0]=='-')
                {
                    // In case of item being shown as disabled, we have to set time to 00:00 when going to edit mode
                    memset(value,'0',4);
                }
                blinkPosition=0;
                isInEditMode=true;
                break;
            case KEY_CANCEL:
                SwitchScreens(MENU_SCREEN);
                return;
            case KEY_UP:
                currentChannel=(currentChannel+1)%CHANNELS_NUM;
                LoadData();
//                symbolsGenerator.SetChannel(currentChannel);
                break;
            case KEY_DOWN:
                currentChannel=(currentChannel+CHANNELS_NUM-1)%CHANNELS_NUM;
                LoadData();
//                symbolsGenerator.SetChannel(currentChannel);
                break;
            case KEY_LEFT:
                currentItem=(currentItem+MAX_ITEMS_PER_CHANNEL-1)%MAX_ITEMS_PER_CHANNEL;
                LoadData();
                break;
            case KEY_RIGHT:
                currentItem=(currentItem+1)%MAX_ITEMS_PER_CHANNEL;
                LoadData();
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
                    if(blinkPosition==WEEKDAY_DATA_START+9)
                    {
                        // Special case for duration m,h symbols
                        if(ValueDigit()=='9')
                        {
                            ValueDigit()='m';
                            break;
                        }
                        else if(ValueDigit()=='m')
                        {
                            ValueDigit()='h';
                            break;
                        }
                    }

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
                    if(blinkPosition==WEEKDAY_DATA_START+9)
                    {
                        // Special case for duration m,h symbols
                        if(ValueDigit()=='h')
                        {
                            ValueDigit()='m';
                            break;
                        }
                        else if(ValueDigit()=='m')
                        {
                            ValueDigit()='9';
                            break;
                        }
                    }

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
    CScheduleItem& item = CTimeManager::Inst.Schedule[currentChannel][currentItem];

    int hour = (value[0]-'0')*10+(value[1]-'0');
    int minute = (value[2]-'0')*10+(value[3]-'0');
    if(hour>=0 && hour<24 && minute>=0 && minute<60)
    {
        item.PresetTimeMinutes=hour*60+minute;
        item.WeekDayMask=0;
        for(int i=0;i<7;i++)
        {
            item.WeekDayMask |= value[WEEKDAY_DATA_START+i]?(1<<i):0;
        }

        switch(value[WEEKDAY_DATA_START+9])
        {
        case 'h':
            item.Duration=((value[WEEKDAY_DATA_START+7]-'0')*10+(value[WEEKDAY_DATA_START+8]-'0'))*3600l;
            break;
        case 'm':
            item.Duration=((value[WEEKDAY_DATA_START+7]-'0')*10+(value[WEEKDAY_DATA_START+8]-'0'))*60l;
            break;
        default:
            item.Duration=(value[WEEKDAY_DATA_START+7]-'0')*100+(value[WEEKDAY_DATA_START+8]-'0')*10+(value[WEEKDAY_DATA_START+9]-'0');
            break;
        }
    }
    if(item.Duration==0)
    {
        memset(value,'-',4);
        memset(value+WEEKDAY_DATA_START,0,7);
    }
    CTimeManager::Inst.SaveSchedule();
//    int bytesWritten = CTimeManager::Inst.SaveSchedule();
//    lcd.setCursor(0,1);
//    lcd.print(bytesWritten);
//    delay(2000);
}

void CScheduleScreen::LoadData()
{
    CScheduleItem& item = CTimeManager::Inst.Schedule[currentChannel][currentItem];

    if(item.Duration>0)
    {
        sprintf(value,"%02d%02d",item.PresetTimeMinutes/60,item.PresetTimeMinutes%60);

        for(int i=0;i<7;i++)
        {
            value[WEEKDAY_DATA_START+i]=item.WeekDayMask&(1<<i) ? 1:0;
        }

        if(item.Duration>99l*3600)
        {
            strcpy(value+WEEKDAY_DATA_START+7,"99h");
        }
        else if(item.Duration>99*60)
        {
            sprintf(value+WEEKDAY_DATA_START+7,"%02dh",item.Duration/3600);
        }
        else if(item.Duration>999)
        {
            sprintf(value+WEEKDAY_DATA_START+7,"%02dm",item.Duration/60);
        }
        else
        {
            sprintf(value+WEEKDAY_DATA_START+7,"%03d",item.Duration);
        }
    }
    else
    {
        memset(value,'-',4);
        memset(value+WEEKDAY_DATA_START,0,7);
        memset(value+WEEKDAY_DATA_START+7,'0',3);
        value[sizeof(value)-1]=0;
    }
}
