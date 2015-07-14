#ifndef CENVLOGGER_H
#define CENVLOGGER_H

#include "DHT11.h"

#define LOG_SEGMENTS_NUM 8
#define LOG_DAYS_NUM 7
struct EnvLogItem
{
    byte AvgTemp[LOG_SEGMENTS_NUM];
    byte AvgHumi[LOG_SEGMENTS_NUM];
    unsigned short Date; // Number of days sinc UNIX time start (1970)

    byte GetMinTemperature();
    byte GetMaxTemperature();
    byte GetMinHumidity();
    byte GetMaxHumidity();

    bool IsValid() const{return Date!=0;}
};

class CEnvLogger
{
    public:
        static CEnvLogger Inst;
        virtual ~CEnvLogger();

        bool RefreshData();

        byte Temperature;
        byte Humidity;
        const EnvLogItem& GetLogItem(int index){return envLogData[index];}
    protected:
        byte failuresCount;
        dht dht11;
    private:
        EnvLogItem envLogData[LOG_DAYS_NUM];
        short numReadouts;
        unsigned long sumTemp;
        unsigned long sumHumi;
        byte prevSegment;

        CEnvLogger();
        void StoreData();
};

#endif // CENVLOGGER_H
