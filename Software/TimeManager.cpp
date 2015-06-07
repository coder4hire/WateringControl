#include "TimeManager.h"
#include <EEPROM.h>

CTimeManager CTimeManager::Inst;

char CTimeManager::weekDayNames[7][3]={"Su","Mo","Tu","We","Th","Fr","Sa"};

CTimeManager::CTimeManager():
rtcClock(7,6,5)
{
    if(rtcClock.haltRTC())
    {
        rtcClock.haltRTC(0);
    }

    LoadSchedule();
}

CTimeManager::~CTimeManager()
{
    //dtor
}

void CTimeManager::GetTimeString(char* out)
{
    tmElements_t tm;
    uint8_t stat = rtcClock.read(tm);
    sprintf(out,"%02d:%02d",tm.Hour,tm.Minute);
}

void CTimeManager::GetDateString(char* out)
{
    tmElements_t tm;
    uint8_t stat = rtcClock.read(tm);
    sprintf(out,"%02d.%02d.%02d",tm.Day,tm.Month,(tm.Year+70)%100);
}

char* CTimeManager::GetWeekDayString()
{
    tmElements_t tm;
    uint8_t stat = rtcClock.read(tm);
    return weekDayNames[tm.Wday-1];
}

void CTimeManager::SetTime(int hh, int mm)
{
    tmElements_t tm;
    uint8_t stat = rtcClock.read(tm);
    tm.Hour=hh;
    tm.Minute=mm;
    rtcClock.write(tm);
}

void CTimeManager::SetDate(int day, int month, int year)
{
    tmElements_t tm;
    uint8_t stat = rtcClock.read(tm);
    tm.Day=day;
    tm.Month=month;
    year = year<70 ? year+30 : year-70;
    tm.Year=year;
    time_t tt = makeTime(tm);
    tm.Wday = (((tt/24/3600) + 4) % 7) + 1;
    rtcClock.write(tm);
}

void CTimeManager::LoadSchedule()
{
    for(int i=0;i<CHANNELS_NUM;i++)
    {
        for(int j=0;j<MAX_ITEMS_PER_CHANNEL;j++)
        {
            int startAddr = (i*MAX_ITEMS_PER_CHANNEL+j)*CSCHEDULEITEM_SAVEABLE_SIZE;
            for(int k=0;k<CSCHEDULEITEM_SAVEABLE_SIZE;k++)
            {
                ((uint8_t*)(&Schedule[i][j]))[k]= EEPROM.read(startAddr+k);
            }
        }
    }
}

int CTimeManager::SaveSchedule()
{
    int bytesWritten=0;
    for(int i=0;i<CHANNELS_NUM;i++)
    {
        for(int j=0;j<MAX_ITEMS_PER_CHANNEL;j++)
        {
            int startAddr = (i*MAX_ITEMS_PER_CHANNEL+j)*CSCHEDULEITEM_SAVEABLE_SIZE;
            for(int k=0;k<CSCHEDULEITEM_SAVEABLE_SIZE;k++)
            {
                uint8_t byteFromEEPROM = EEPROM.read(startAddr+k);
                uint8_t byteFromArray = ((uint8_t*)(&Schedule[i][j]))[k];
                if(byteFromEEPROM!=byteFromArray)
                {
                    EEPROM.write(startAddr+k,byteFromArray);
                    bytesWritten++;
                }
            }
        }
    }
    return bytesWritten;
}
