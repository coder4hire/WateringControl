#ifndef CSETTINGSSCREEN_H
#define CSETTINGSSCREEN_H

#include "ScreenBase.h"

enum SETTINGS_ITEMS
{
    SETTING_TIME=0,
    SETTING_DATE,
    SETTING_HIGH_POWER,
    SETTING_RESET_SCHED,
    MAX_SETTINGS_NUM
};

#define SETTINGS_ICONS_NUM MAX_SETTINGS_NUM

class CSettingsScreen : public CScreenBase
{
    public:
        CSettingsScreen();
        virtual ~CSettingsScreen();

        virtual void OnShow();
        virtual void OnClose();
        virtual void Refresh();
        virtual void CheckKeys(EKeys keys, EKeys justPressed, EKeys justReleased, bool isChanged);
    protected:
        static const byte icons[SETTINGS_ICONS_NUM][7];
        void PaintMenu();

        bool isInEditMode;
        int settingNum;
        char value[9];

        char blinkPosition;
        bool isBlinkingShown;
        char& ValueDigit(){return value[cursorPositions[settingNum][blinkPosition]];}

        static const char cursorPositions[MAX_SETTINGS_NUM][8];
        static const char minValues[MAX_SETTINGS_NUM][8];
        static const char maxValues[MAX_SETTINGS_NUM][8];
        static const char settingsNames[MAX_SETTINGS_NUM][9];
        static const char maxDays[12];

        void SaveData();
        void LoadData();
};

#endif // CSETTINGSSCREEN_H
