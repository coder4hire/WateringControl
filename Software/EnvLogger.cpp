#include "EnvLogger.h"
#include "DS1302RTC.h"

CEnvLogger CEnvLogger::Inst;

byte EnvLogItem::GetMinTemperature()
{
    byte minVal=AvgTemp[0];
    for(int i=1;i<LOG_SEGMENTS_NUM;i++)
    {
        if(AvgTemp[i] && AvgHumi[i] && minVal>AvgTemp[i])
        {
            minVal=AvgTemp[i];
        }
    }
    return minVal;
}

byte EnvLogItem::GetMaxTemperature()
{
    byte maxVal=AvgTemp[0];
    for(int i=1;i<LOG_SEGMENTS_NUM;i++)
    {
        if(AvgTemp[i] && AvgHumi[i] && maxVal<AvgTemp[i])
        {
            maxVal=AvgTemp[i];
        }
    }
    return maxVal;
}

byte EnvLogItem::GetMinHumidity()
{
    byte minVal=AvgHumi[0];
    for(int i=1;i<LOG_SEGMENTS_NUM;i++)
    {
        if(AvgTemp[i] && AvgHumi[i] && minVal>AvgHumi[i])
        {
            minVal=AvgHumi[i];
        }
    }
    return minVal;
}

byte EnvLogItem::GetMaxHumidity()
{
    byte maxVal=AvgHumi[0];
    for(int i=1;i<LOG_SEGMENTS_NUM;i++)
    {
        if(AvgTemp[i] && AvgHumi[i] && maxVal<AvgHumi[i])
        {
            maxVal=AvgHumi[i];
        }
    }
    return maxVal;
}


CEnvLogger::CEnvLogger()
{
    failuresCount=0;
    Temperature=0;
    Humidity=0;
    memset(envLogData,0,sizeof(envLogData));

    numReadouts=0;
    sumTemp=0;
    sumHumi=0;
    prevSegment=0;
}

CEnvLogger::~CEnvLogger()
{
    //dtor
}

bool CEnvLogger::RefreshData()
{
    if(dht11.read11(8)==DHTLIB_OK)
    {
        Temperature=dht11.temperature;
        Humidity=dht11.humidity;
        failuresCount=0;
    }
    else
    {
        failuresCount++;
    }

    if(failuresCount>10)
    {
        Temperature=0;
        Humidity=0;
        failuresCount=10;
        return false;
    }

    StoreData();

    return true;
}

void CEnvLogger::StoreData()
{
    time_t time = DS1302RTC::get();
    short newDate = (short)(time/(3600L*24));
    byte segmentNum = (time/(3600L*3))%8;

    //--- Checking for date change
    if(newDate!=envLogData[LOG_DAYS_NUM-1].Date)
    {
        memmove(envLogData,envLogData+1,sizeof(envLogData[0]));
        memset(envLogData+(LOG_DAYS_NUM-1),0,sizeof(envLogData[0]));
        envLogData[LOG_DAYS_NUM-1].Date = newDate;
    }
    EnvLogItem& item = envLogData[LOG_DAYS_NUM-1];

    // Checking for segment change
    if(segmentNum!=prevSegment)
    {
        prevSegment=segmentNum;
        numReadouts=0;
        sumTemp=0;
        sumHumi=0;
    }

    sumHumi+=Humidity;
    sumTemp+=Temperature;
    numReadouts++;

    item.AvgHumi[segmentNum]=sumHumi/numReadouts;
    item.AvgTemp[segmentNum]=sumTemp/numReadouts;
}
