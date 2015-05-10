#ifndef CSCREENBASE_H
#define CSCREENBASE_H

#include "KeysParser.h"
#include "LiquidCrystal2.h" // include updated library code:
extern LiquidCrystal lcd;

enum EScreenIDs
{
    MAIN_SCREEN=0,
    MENU_SCREEN,
    TOTAL_SCREENS_NUM
};

class CScreenBase
{
    public:
        CScreenBase();
        virtual ~CScreenBase();

        virtual void Refresh()=0;
        virtual void CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged)=0;

        static CScreenBase* GetCurrentScreen(){return pCurrentScreen;}
        static void SwitchScreens(EScreenIDs newScreenID);

    protected:
        static CScreenBase* pScreens[TOTAL_SCREENS_NUM];
        static CScreenBase* pCurrentScreen;
        virtual void OnShow(){};
        virtual void OnClose(){};
    private:
};

#endif // CSCREENBASE_H
