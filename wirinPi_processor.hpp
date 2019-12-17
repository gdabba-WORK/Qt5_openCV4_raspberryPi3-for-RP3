#ifndef ADJUST_PROCESSOR_HPP
#define ADJUST_PROCESSOR_HPP

#include "includes.hpp"



class WiringPi : public QThread
{
    Q_OBJECT

private:
    int adcChannel;
    int adcValue;
    bool flag;

private:
    void run() override;

public:
    WiringPi(QObject *parent = nullptr);
    ~WiringPi() override;
    int read_mcp3008_adc(unsigned char adcChannel);
    void setFlag(bool);

signals:
    void cdsReady(const int& cdsValue);
};

#endif // ADJUST_PROCESSOR_HPP
