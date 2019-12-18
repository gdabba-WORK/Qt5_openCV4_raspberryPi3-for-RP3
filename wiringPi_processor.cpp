#include "wiringPi_processor.hpp"

void WiringPi::run()
{
    while(isCDS || isLED)
    {
        isRun = true;
        if (isCDS)
            read_mcp3008_adc();
        if (isLED)
            bling();
        msleep(500);
    }
    isRun = false;
}

WiringPi::WiringPi(QObject *parent)
    : QThread(parent),
      adcChannel(0), adcValue(0), isRun(false), isCDS(false), isLED(false)
{
    cout << "WiringPi Object Created!" << endl;
    if(wiringPiSetup() == -1)
    {
        cout << "Unable to start wiringPi" << endl;
        exit(1);
    }
    if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1)
    {
        cout << "wiringPiSPISetup Failed" << endl;
        return;
    }
    pinMode(CS_MCP3008, OUTPUT);
    pinMode(LED, OUTPUT);
}

WiringPi::~WiringPi()
{
    cout << "Color Object deleted!" << endl;
}

void WiringPi::read_mcp3008_adc()
{
    unsigned char buff[3];
    adcValue = 0;
    adcChannel = 3;

    buff[0] = 0x01;
    buff[1] = 0x80|((adcChannel & 0x07)<<4);
    buff[2] = 0x00;
    digitalWrite(CS_MCP3008, 0);
    wiringPiSPIDataRW(SPI_CHANNEL, buff, 3);
    buff[1] = 0x03 & buff[1];
    adcValue = (buff[1]<<8) | buff[2];
    digitalWrite(CS_MCP3008, 1);

    if (adcValue > 300)
        cout << "Darkness : " << adcValue << endl;
    else if(adcValue < 50)
        cout << "Brightness : " << adcValue << endl;
    else
        cout << "Normal : " << adcValue << endl;

    emit cdsReady(adcValue);
}

void WiringPi::bling()
{
    static bool flag = false;
    flag = !flag;
    digitalWrite(LED, flag);
}

void WiringPi::setIsCDS(bool value)
{
    isCDS = value;
}

void WiringPi::setIsLED(bool value)
{
    isLED = value;
}

bool WiringPi::getIsRun()
{
    return isRun;
}
