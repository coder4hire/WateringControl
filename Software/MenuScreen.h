#ifndef CMENUSCREEN_H
#define CMENUSCREEN_H

#include "ScreenBase.h"
#include <Arduino.h>

#define ICONS_NUM 5
#define ICONS_NUM_DISPLAYED 3

class CMenuScreen : public CScreenBase
{
    public:
        CMenuScreen();
        virtual ~CMenuScreen();

        virtual void OnShow();
        virtual void Refresh();
        virtual void CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged);
    protected:
        static byte icons[ICONS_NUM][8];
        static char description[ICONS_NUM_DISPLAYED][9];
        int currentItem;
        int oldItem;
    private:
};

#endif // CMENUSCREEN_H
