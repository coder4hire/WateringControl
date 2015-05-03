#include "KeysParser.h"
#include <Arduino.h>

CKeysParser CKeysParser::Inst;

CKeysParser::CKeysParser()
{
    keyMap=KEY_NONE;

    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
}

CKeysParser::~CKeysParser()
{
}

bool CKeysParser::PerformCheck(EKeys& justPressed, EKeys& justReleased)
{
    EKeys newKeyMap;
    newKeyMap = (EKeys)(VoltageToKey(analogRead(A2))<<3 | VoltageToKey(analogRead(A3)));

    bool isChanged = newKeyMap!=keyMap;
    justPressed = (EKeys)((keyMap^newKeyMap)&newKeyMap);
    justReleased = (EKeys)((keyMap^newKeyMap)&(~newKeyMap));
    keyMap=newKeyMap;
    return isChanged;
}

unsigned int CKeysParser::VoltageToKey(int voltage)
{
    if(voltage>600)
    {
        return KEY_NONE;
    }
    else if(voltage>400)
    {
        return KEY_LEFT;
    }
    else if(voltage>290)
    {
        return KEY_UP;
    }
    else if(voltage>230)
    {
        return KEY_LEFT|KEY_UP;
    }
    else if(voltage>200)
    {
        return KEY_RIGHT;
    }
    else if(voltage>155)
    {
        return KEY_LEFT|KEY_RIGHT;
    }
    else if(voltage>135)
    {
        return KEY_UP|KEY_RIGHT;
    }
    else
    {
        return KEY_UP|KEY_LEFT|KEY_RIGHT;
    }
}
