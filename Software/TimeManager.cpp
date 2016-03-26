#include "TimeManager.h"
#include <EEPROM.h>

CTimeManager CTimeManager::Inst;

char CTimeManager::weekDayNames[7][3]={"Su","Mo","Tu","We","Th","Fr","Sa"};

CTimeManager::CTimeManager():
rtcClock(7,6,5),
chanBusyBitfield(0),
quickLaunchStartTime(0),
quickLaunchChannel(0)
{
    if(rtcClock.haltRTC())
    {
        rtcClock.haltRTC(0);
    }

    LoadSchedule();
    LoadLowPowerSettings();
}

CTimeManager::~CTimeManager()
{
    //dtor
}

void CTimeManager::GetTimeString(char* out)
{
    tmElements_t tm;
    rtcClock.read(tm);
    sprintf(out,"%02d:%02d",tm.Hour,tm.Minute);
}

void CTimeManager::GetDateString(char* out)
{
    tmElements_t tm;
    rtcClock.read(tm);
    sprintf(out,"%02d.%02d.%02d",tm.Day,tm.Month,(tm.Year+70)%100);
}

char* CTimeManager::GetWeekDayString()
{
    tmElements_t tm;
    rtcClock.read(tm);
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
    memset(Schedule,0,sizeof(Schedule));
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
    //--- Saving Schedule
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

void CTimeManager::LoadLowPowerSettings()
{
    //--- Loading Low Power settings
    int lowPowerEEPROM = CHANNELS_NUM*MAX_ITEMS_PER_CHANNEL*CSCHEDULEITEM_SAVEABLE_SIZE;
    for(int i=0;i<CHANNELS_NUM;i++)
    {
        IsHighPower[i]= EEPROM.read(lowPowerEEPROM+i);
    }
}

void CTimeManager::SaveLowPowerSettings()
{
    //--- Loading Low Power settings
    int lowPowerEEPROM = CHANNELS_NUM*MAX_ITEMS_PER_CHANNEL*CSCHEDULEITEM_SAVEABLE_SIZE;
    for(int i=0;i<CHANNELS_NUM;i++)
    {
        byte lp = EEPROM.read(lowPowerEEPROM+i);
        if(lp!=IsHighPower[i])
        {
            EEPROM.write(lowPowerEEPROM+i,IsHighPower[i]);
        }
    }
}

void CTimeManager::CheckEvents()
{
    tmElements_t tm;
    rtcClock.read(tm);
    time_t now = rtcClock.get();

    long nowTime = tm.Hour*3600l + tm.Minute*60l + tm.Second;
    byte todayWeekdayMask = 1<<(tm.Wday-1);
    for(int channelNum=1;channelNum<=CHANNELS_NUM;channelNum++)
    {
        for(int itemNum=0;itemNum<MAX_ITEMS_PER_CHANNEL;itemNum++)
        {
            CScheduleItem& item = Schedule[channelNum-1][itemNum];
            if(item.ActualStartTime==0)
            {
                // Event has not been fired yet
                if((item.WeekDayMask & todayWeekdayMask) && nowTime==item.PresetTimeMinutes*60l)
                {
                    // If quick launch event is ongoing, cancel it
                    StopQuickLaunch();

                    // Launching timed event
                    // TODO: add check if channel is not dependent on other enabled channels
                    if(!IsAnyHPChannelBusy() || !IsHighPower[channelNum])
                    {
                        item.ActualStartTime = now;
                        StartEvent(channelNum);
                    }
                }
            }
            else
            {
                // Event is fired, looking for the end time
                if(now > item.ActualStartTime+item.Duration)
                {
                    item.ActualStartTime = 0;
                    StopEvent(channelNum);
                }
            }
        }
    }

    // Looking for the end time of quick launch event
    if(quickLaunchStartTime && now>(quickLaunchStartTime+QUICK_LAUNCH_DURATION))
    {
        StopQuickLaunch();
    }
}

void CTimeManager::StartQuickLaunch(byte channelNum)
{
    if(channelNum>0 && channelNum<=CHANNELS_NUM &&
       !quickLaunchChannel && !IsAnyHPChannelBusy())
    {
        quickLaunchChannel=channelNum;
        quickLaunchStartTime=rtcClock.get();
        StartEvent(quickLaunchChannel);
    }
}

void CTimeManager::StopQuickLaunch()
{
    if(quickLaunchChannel)
    {
        StopEvent(quickLaunchChannel);
        quickLaunchChannel = 0;
    }
    quickLaunchStartTime = 0;
}

void CTimeManager::StartEvent(int channelNum)
{
    if(IsHighPower[channelNum])
    {
        chanBusyBitfield |= 1<<(channelNum-1);
    }
    else
    {
        chanBusyBitfield |= 0x10<<(channelNum-1);
    }
    digitalWrite(CHAN_PIN1+channelNum-1,1);
}

void CTimeManager::StopEvent(int channelNum)
{
    digitalWrite(CHAN_PIN1+channelNum-1,0);
    chanBusyBitfield &= ~(1<<(channelNum-1));
    chanBusyBitfield &= ~(0x10<<(channelNum-1));
}

void CTimeManager::ResetAllSchedules()
{
    StopEvent(1);
    StopEvent(2);
    StopEvent(3);
    StopEvent(4);

    memset(Schedule,0,sizeof(Schedule));
    SaveSchedule();
}
