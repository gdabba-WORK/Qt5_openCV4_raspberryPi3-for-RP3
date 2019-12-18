#ifndef ADJUST_PROCESSOR_HPP
#define ADJUST_PROCESSOR_HPP

#include "includes.hpp"

class WiringPi : public QThread
{
    Q_OBJECT

private:
    int adcChannel;
    int adcValue;
    bool isRun, isCDS, isLED;

private:
    void run() override;

public:
    WiringPi(QObject *parent = nullptr);
    ~WiringPi() override;
    void read_mcp3008_adc();
    void bling();
    void setIsCDS(bool);
    void setIsLED(bool);
    bool getIsRun();

signals:
    void cdsReady(const int& cdsValue);
};

#endif // ADJUST_PROCESSOR_HPP
