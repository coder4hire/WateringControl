#ifndef CENVLOGGER_H
#define CENVLOGGER_H

#include "DHT11.h"

class CEnvLogger
{
    public:
        static CEnvLogger Inst;
        virtual ~CEnvLogger();
        bool ReadCurrentData(byte& temperature, byte& humidity);
    protected:
        float temp;
        float humi;
        byte failuresCount;
        dht dht11;
    private:
        CEnvLogger();
};

#endif // CENVLOGGER_H
