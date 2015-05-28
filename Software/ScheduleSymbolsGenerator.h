#ifndef CSCHEDULESYMBOLSGENERATOR_H
#define CSCHEDULESYMBOLSGENERATOR_H

#include <Arduino.h>
#include "TimeManager.h"

enum SCHEDULE_SYMBOLS
{
    SSYM_NONE=0,
    SSYM_CHAN,
    SSYM_S,
    SSYM_MT,
    SSYM_TF,
    SSYM_WS,
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
        static const byte iconNoPowerRestrictions[7];
        static const byte emptySpaces[2][7];
        static const byte iconsChannelNums[CHANNELS_NUM][7];
        static const byte iconsWeekDays[12][7];
        byte currentChannel;
        byte currentWeekdayIcons[4];
    private:
};

#endif // CSCHEDULESYMBOLSGENERATOR_H
