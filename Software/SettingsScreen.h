#ifndef CSETTINGSSCREEN_H
#define CSETTINGSSCREEN_H

#include "ScreenBase.h"

#define SETTINGS_ICONS_NUM 1

class CSettingsScreen : public CScreenBase
{
    public:
        CSettingsScreen();
        virtual ~CSettingsScreen();

        virtual void OnShow();
        virtual void Refresh();
        virtual void CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged);
    protected:
        static byte icons[SETTINGS_ICONS_NUM][7];
        void PaintMenu();
};

#endif // CSETTINGSSCREEN_H
