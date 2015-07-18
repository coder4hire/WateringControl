
#include "arduino.h"
#include "EnvLogItem.h"

byte CEnvLogItem::GetMinTemperature() const
{
    byte minVal=255;
    for(int i=0;i<LOG_SEGMENTS_NUM;i++)
    {
        if(AvgTemp[i] && AvgHumi[i] && minVal>AvgTemp[i])
        {
            minVal=AvgTemp[i];
        }
    }
    return minVal%100;
}

byte CEnvLogItem::GetMaxTemperature() const
{
    byte maxVal=0;
    for(int i=0;i<LOG_SEGMENTS_NUM;i++)
    {
        if(AvgTemp[i] && AvgHumi[i] && maxVal<AvgTemp[i])
        {
            maxVal=AvgTemp[i];
        }
    }
    return maxVal%100;
}

byte CEnvLogItem::GetMinHumidity() const
{
    byte minVal=255;
    for(int i=0;i<LOG_SEGMENTS_NUM;i++)
    {
        if(AvgTemp[i] && AvgHumi[i] && minVal>AvgHumi[i])
        {
            minVal=AvgHumi[i];
        }
    }
    return minVal%100;
}

byte CEnvLogItem::GetMaxHumidity() const
{
    byte maxVal=0;
    for(int i=0;i<LOG_SEGMENTS_NUM;i++)
    {
        if(AvgTemp[i] && AvgHumi[i] && maxVal<AvgHumi[i])
        {
            maxVal=AvgHumi[i];
        }
    }
    return maxVal%100;
}

void CEnvLogItem::FillTemperatureGraph(char* buf) const
{
    int minTemp = GetMinTemperature();
    int deltaTemp = GetMaxTemperature()-minTemp;

    if(!deltaTemp)
    {
        for(int i=0;i<LOG_SEGMENTS_NUM;i++)
        {
            buf[i]= AvgTemp[i] && AvgHumi[i] ? 4 : '.';
        }
        buf[LOG_SEGMENTS_NUM]=0;
        return;
    }
    for(int i=0;i<LOG_SEGMENTS_NUM;i++)
    {
        if(AvgTemp[i]!=0 && AvgHumi[i]!=0)
        {
            //Valid item, calculate proper symbol num for it
            int val = (AvgTemp[i]-minTemp)*8/deltaTemp;
            if(val>7)
            {
                val=7;
            }
            buf[i]=val ? (char)val : ' ';
        }
        else
        {
            buf[i]='.';
        }
    }
    buf[LOG_SEGMENTS_NUM]=0;
}

void CEnvLogItem::FillHumidityGraph(char* buf) const
{
    int minHumi = GetMinHumidity();
    int deltaHumi = GetMaxHumidity()-minHumi;

    if(!deltaHumi)
    {
        for(int i=0;i<LOG_SEGMENTS_NUM;i++)
        {
            buf[i]= AvgTemp[i] && AvgHumi[i] ? 4 : '.';
        }
        buf[LOG_SEGMENTS_NUM]=0;
        return;
    }

    for(int i=0;i<LOG_SEGMENTS_NUM;i++)
    {
        if(AvgTemp[i]!=0 && AvgHumi[i]!=0)
        {
            //Valid item, calculate proper symbol num for it
            int val = (AvgHumi[i]-minHumi)*8/deltaHumi;
            if(val>7)
            {
                val=7;
            }
            buf[i]=val ? (char)val : ' ';
        }
        else
        {
            buf[i]='.';
        }
    }
    buf[LOG_SEGMENTS_NUM]=0;
}
