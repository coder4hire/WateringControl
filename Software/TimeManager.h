#ifndef CTIMEMANAGER_H
#define CTIMEMANAGER_H

#include "DS1302RTC.h"

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

    protected:
        CTimeManager();
        DS1302RTC rtcClock;

        static char weekDayNames[7][3];
    private:
};

#endif // CTIMEMANAGER_H
