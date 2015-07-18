#ifndef ENVLOGITEM_H
#define ENVLOGITEM_H

#define LOG_SEGMENTS_NUM 8

class CEnvLogItem
{
public:
    byte AvgTemp[LOG_SEGMENTS_NUM];
    byte AvgHumi[LOG_SEGMENTS_NUM];
    unsigned short Date; // Number of days sinc UNIX time start (1970)

    byte GetMinTemperature() const;
    byte GetMaxTemperature() const;
    byte GetMinHumidity() const;
    byte GetMaxHumidity() const;

    void FillTemperatureGraph(char* buf) const;
    void FillHumidityGraph(char* buf) const;

    bool IsValid() const{return Date!=0;}
};

#endif // ENVLOGITEM_H
