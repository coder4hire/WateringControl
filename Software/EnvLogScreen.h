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
};

#endif // CENVLOGSCREEN_H
