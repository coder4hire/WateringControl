#ifndef CMENUSCREEN_H
#define CMENUSCREEN_H

#include "ScreenBase.h"

#define MENU_ICONS_NUM 5
#define MENU_ICONS_NUM_DISPLAYED 3

class CMenuScreen : public CScreenBase
{
    public:
        CMenuScreen();
        virtual ~CMenuScreen();

        virtual void OnShow();
        virtual void Refresh();
        virtual void CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged);
    protected:
        static byte icons[MENU_ICONS_NUM][7];
        static char description[MENU_ICONS_NUM_DISPLAYED][9];
        static byte screenDescriptors[MENU_ICONS_NUM_DISPLAYED];
        int currentItem;
        int oldItem;
    private:
};

#endif // CMENUSCREEN_H
