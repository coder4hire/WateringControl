#ifndef CMAINSCREEN_H
#define CMAINSCREEN_H

#include "ScreenBase.h"

#define MAIN_ICONS_NUM 2

class CMainScreen : public CScreenBase
{
    public:
        CMainScreen();
        virtual ~CMainScreen();

        virtual void Refresh();
        virtual void CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged);

    protected:
        virtual void OnShow();
        static byte icons[MAIN_ICONS_NUM][7];
    private:
        byte channelCheckIndex;
};

#endif // CMAINSCREEN_H
