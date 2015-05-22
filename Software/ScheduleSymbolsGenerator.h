#ifndef CSCHEDULESYMBOLSGENERATOR_H
#define CSCHEDULESYMBOLSGENERATOR_H

#include <Arduino.h>
#include "TimeManager.h"

enum SCHEDULE_SYMBOLS
{
    SSYM_NONE=0,
    SSYM_CHAN,
    SSYM_ST,
    SSYM_MF,
    SSYM_TS,
    SSYM_W,
    SSYM_NO_POWER_RESTRICTION,
    SSYM_MAX
};

class CScheduleSymbolsGenerator
{
    public:
        CScheduleSymbolsGenerator();
        virtual ~CScheduleSymbolsGenerator();
        void Start();
        void SetChannel(int chanNum);
        void SetWeekday(byte weekDayMask);
    protected:
        static byte iconNoPowerRestrictions[7];
        static byte emptySpace[7];
        static byte iconsChannelNums[CHANNELS_NUM][7];
        static byte iconsWeekDays[12][7];
    private:
};

#endif // CSCHEDULESYMBOLSGENERATOR_H
