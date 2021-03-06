#ifndef CTIMEMANAGER_H
#define CTIMEMANAGER_H

#include "DS1302RTC.h"

#define CHANNELS_NUM 4
#define MAX_ITEMS_PER_CHANNEL 6
#define QUICK_LAUNCH_DURATION 30

enum EChannelPins
{
    CHAN_PIN1 = 10,
    CHAN_PIN2,
    CHAN_PIN3,
    CHAN_PIN4
};

enum EWeekDaysMasks
{
    WDM_NONE=0,
    WDM_SUNDAY=1,
    WDM_MONDAY=2,
    WDM_TUESDAY=4,
    WDM_WEDNESDAY=8,
    WDM_THURSDAY=16,
    WDM_FRIDAY=32,
    WDM_SATURDAY=64
};

#define CSCHEDULEITEM_SAVEABLE_SIZE 7
struct CScheduleItem
{
    byte WeekDayMask;
    short PresetTimeMinutes;
    long Duration;
    time_t ActualStartTime;
};

class CTimeManager
{
    public:
        static CTimeManager Inst;
        virtual ~CTimeManager();

        void GetTimeString(char* out);
        void GetDateString(char* out);
        void SetTime(int mm, int hh);
        void SetDate(int day, int month, int year);
        char* GetWeekDayString();

        CScheduleItem Schedule[CHANNELS_NUM][MAX_ITEMS_PER_CHANNEL];
        byte IsHighPower[CHANNELS_NUM];

        void LoadSchedule();
        int SaveSchedule();
        void LoadLowPowerSettings();
        void SaveLowPowerSettings();

        void CheckEvents();
        bool IsHPChannelBusy(int chanNum){return (chanBusyBitfield&(1<<(chanNum-1)))!=0;}
        bool IsLPChannelBusy(int chanNum){return (chanBusyBitfield&(0x10<<(chanNum-1)))!=0;}
        bool IsAnyHPChannelBusy(){return chanBusyBitfield&0xF;}

        void ResetAllSchedules();
        void StartQuickLaunch(byte channelNum);
        void StopQuickLaunch();

    protected:
        CTimeManager();
        DS1302RTC rtcClock;

        void StartEvent(int channelNum);
        void StopEvent(int channelNum);

        static char weekDayNames[7][3];

        byte chanBusyBitfield;

        time_t quickLaunchStartTime;
        byte quickLaunchChannel;
    private:
};

#endif // CTIMEMANAGER_H
