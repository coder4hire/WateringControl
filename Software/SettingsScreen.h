#ifndef CSETTINGSSCREEN_H
#define CSETTINGSSCREEN_H

#include "ScreenBase.h"

enum SETTINGS_ITEMS
{
    SETTING_TIME=0,
    SETTING_DATE,
    MAX_SETTINGS_NUM
};

#define SETTINGS_ICONS_NUM MAX_SETTINGS_NUM

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

        bool isInEditMode;
        int settingNum;
        char value[9];

        char blinkPosition;
        bool isBlinkingShown;
        char blinkingData;
        static char cursorPositions[MAX_SETTINGS_NUM][8];
        static char minValues[MAX_SETTINGS_NUM][8];
        static char maxValues[MAX_SETTINGS_NUM][8];
        static char settingsNames[MAX_SETTINGS_NUM][8];


        void SaveData();
        void LoadData();
};

#endif // CSETTINGSSCREEN_H
