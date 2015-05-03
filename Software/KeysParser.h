#ifndef CKEYSPARSER_H
#define CKEYSPARSER_H


enum EKeys
{
    KEY_NONE=0,
    KEY_UP=1,
    KEY_LEFT=2,
    KEY_RIGHT=4,
    KEY_OK=8,
    KEY_DOWN=16,
    KEY_CANCEL=32
};

class CKeysParser
{
    public:
        virtual ~CKeysParser();
        EKeys GetKeyMap() { return keyMap; }
        bool IsKeyPressed(EKeys key){ return keyMap&key;}
        bool PerformCheck(EKeys& justPressed, EKeys& justReleased);
        static CKeysParser Inst;

    protected:
        CKeysParser();
        unsigned int VoltageToKey(int voltage);
        EKeys keyMap;
};

#endif // CKEYSPARSER_H
