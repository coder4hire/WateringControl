#ifndef CMAINSCREEN_H
#define CMAINSCREEN_H

#include "ScreenBase.h"
#include "DS1302RTC.h"

class CMainScreen : public CScreenBase
{
    public:
        CMainScreen();
        virtual ~CMainScreen();

        virtual void Refresh();
        virtual void CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged);
    protected:
    private:
};

#endif // CMAINSCREEN_H
