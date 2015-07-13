#include "EnvLogger.h"

CEnvLogger CEnvLogger::Inst;

CEnvLogger::CEnvLogger()
{
    failuresCount=0;
    temp=0;
    humi=0;
}

CEnvLogger::~CEnvLogger()
{
    //dtor
}

bool CEnvLogger::ReadCurrentData(byte& temperature, byte& humidity)
{
    if(dht11.read11(8)==DHTLIB_OK)
    {
        temp=dht11.temperature;
        humi=dht11.humidity;
        failuresCount=0;
    }
    else
    {
        failuresCount++;
    }

    if(failuresCount>10)
    {
        temperature=temp=0;
        humidity=humi=0;
        failuresCount=10;
        return false;
    }

    temperature=temp;
    humidity=humi;
    return true;
}
