#include "EnvLogger.h"
#include "DS1302RTC.h"

CEnvLogger CEnvLogger::Inst;

CEnvLogger::CEnvLogger()
{
    failuresCount=0;
    Temperature=0;
    Humidity=0;
    memset(envLogData,0,sizeof(envLogData));

//    for(int i=0;i<sizeof(envLogData);i++)
//    {
//        ((char*)&envLogData)[i] = rand()%256;
//    }

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

    if(Humidity && Temperature)
    {
        //--- Update data only if we have valid readings

        //--- Checking for date change
        if(newDate!=envLogData[LOG_DAYS_NUM-1].Date)
        {
            memmove(envLogData,envLogData+1,sizeof(envLogData[0]));
            memset(envLogData+(LOG_DAYS_NUM-1),0,sizeof(envLogData[0]));
            envLogData[LOG_DAYS_NUM-1].Date = newDate;
        }
        CEnvLogItem& item = envLogData[LOG_DAYS_NUM-1];

        //--- Checking for segment change
        if(segmentNum!=prevSegment)
        {
            prevSegment=segmentNum;
            numReadouts=0;
            sumTemp=0;
            sumHumi=0;
        }

        //--- Filling in data
        sumHumi+=Humidity;
        sumTemp+=Temperature;
        numReadouts++;

        item.AvgHumi[segmentNum]=sumHumi/numReadouts;
        if(item.AvgHumi[segmentNum]>99)
        {
            item.AvgHumi[segmentNum]=99;
        }
        item.AvgTemp[segmentNum]=sumTemp/numReadouts;
        if(item.AvgTemp[segmentNum]>99)
        {
            item.AvgTemp[segmentNum]=99;
        }

    }
}

