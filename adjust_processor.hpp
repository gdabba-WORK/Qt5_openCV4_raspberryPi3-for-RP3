#ifndef ADJUST_PROCESSOR_HPP
#define ADJUST_PROCESSOR_HPP

#include "includes.hpp"
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define CS_MCP3008  10
#define SPI_CHANNEL 0
#define SPI_SPEED   1000000

class Adjust : public QThread
{
    Q_OBJECT

private:
    int adcChannel;
    int adcValue;
    bool flag;

private:
    void run() override;

public:
    Adjust(QObject *parent = nullptr);
    ~Adjust() override;
    int read_mcp3008_adc(unsigned char adcChannel);
    void setFlag(bool);
};

#endif // ADJUST_PROCESSOR_HPP
