#ifndef CSCHEDULESCREEN_H
#define CSCHEDULESCREEN_H

#include "ScreenBase.h"
#include "ScheduleSymbolsGenerator.h"

#define MAX_SCHEDULE_DATA_NUM 14
#define WEEKDAY_DATA_START 4

class CScheduleScreen : public CScreenBase
{
    public:
        CScheduleScreen();
        virtual ~CScheduleScreen();

        virtual void OnShow();
        virtual void OnClose();
        virtual void Refresh();
        virtual void CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged);

    protected:
        void PrintScreen();
        CScheduleSymbolsGenerator symbolsGenerator;

        int currentChannel;
        int currentItem;
        bool isInEditMode;
        char value[MAX_SCHEDULE_DATA_NUM+1];

        char blinkPosition;
        bool isBlinkingShown;
        char& ValueDigit(){return value[blinkPosition];}
        byte CalculateWeekdayMask();

        static const char cursorPositions[2][MAX_SCHEDULE_DATA_NUM];
        static const char minValues[MAX_SCHEDULE_DATA_NUM];
        static const char maxValues[MAX_SCHEDULE_DATA_NUM];

        void SaveData();
        void LoadData();
};

#endif // CSCHEDULESCREEN_H
