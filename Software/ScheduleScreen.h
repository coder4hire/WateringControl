#ifndef CSCHEDULESCREEN_H
#define CSCHEDULESCREEN_H

#include "ScreenBase.h"

#define SCHEDULE_ICONS_NUM 3

class CScheduleScreen : public CScreenBase
{
    public:
        CScheduleScreen();
        virtual ~CScheduleScreen();

        virtual void OnShow();
        virtual void Refresh();
        virtual void CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged);
    protected:
        static byte icons[SCHEDULE_ICONS_NUM][7];
};

#endif // CSCHEDULESCREEN_H
