#ifndef CENVLOGSCREEN_H
#define CENVLOGSCREEN_H

#include "ScreenBase.h"

#define ENVLOG_ICONS_NUM 8

class CEnvLogScreen : public CScreenBase
{
    public:
        CEnvLogScreen();
        virtual ~CEnvLogScreen();

        virtual void OnShow();
        virtual void Refresh();
        virtual void CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged);
    protected:
        static byte icons[ENVLOG_ICONS_NUM][7];

        enum EPageType
        {
            PT_DATE,
            PT_TEMP,
            PT_HUMI,
            PT_MAX
        };

        EPageType pageType;
        int dayNum;
};

#endif // CENVLOGSCREEN_H
