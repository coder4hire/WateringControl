#ifndef CENVLOGGER_H
#define CENVLOGGER_H

#include "DHT11.h"
#include "EnvLogItem.h"

#define LOG_DAYS_NUM 7

class CEnvLogger
{
    public:
        static CEnvLogger Inst;
        virtual ~CEnvLogger();

        bool RefreshData();

        byte Temperature;
        byte Humidity;
        const CEnvLogItem& GetLogItem(int index){return envLogData[index];}
    protected:
        byte failuresCount;
        dht dht11;
    private:
        CEnvLogItem envLogData[LOG_DAYS_NUM];
        short numReadouts;
        unsigned long sumTemp;
        unsigned long sumHumi;
        byte prevSegment;

        CEnvLogger();
        void StoreData();
};

#endif // CENVLOGGER_H
