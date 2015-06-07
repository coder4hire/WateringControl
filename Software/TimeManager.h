#ifndef CTIMEMANAGER_H
#define CTIMEMANAGER_H

#include "DS1302RTC.h"

#define CHANNELS_NUM 4
#define MAX_ITEMS_PER_CHANNEL 10

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

        void LoadSchedule();
        int SaveSchedule();

    protected:
        CTimeManager();
        DS1302RTC rtcClock;

        static char weekDayNames[7][3];
    private:
};

#endif // CTIMEMANAGER_H
