#include "adjust_processor.hpp"

void Adjust::run()
{
    if(wiringPiSetup() == -1)
    {
        cout << "Unable to start wiringPi" << endl;
        return;
    }
    pinMode(CS_MCP3008, OUTPUT);
    if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1)
    {
        cout << "wiringPiSPISetup Failed" << endl;
        return;
    }
    while(flag)
    {
        adcChannel = 3;
        adcValue = read_mcp3008_adc(adcChannel);
        if (adcValue > 300)
            cout << "Darkness : " << adcValue << endl;
        else if(adcValue < 50)
            cout << "Brightness : " << adcValue << endl;
        else
            cout << "Normal : " << adcValue << endl;
        msleep(500);
    }
}

Adjust::Adjust(QObject *parent)
    : QThread(parent), adcChannel(0), adcValue(0), flag(false)
{
    cout << "Adjust Object Created!" << endl;
}

Adjust::~Adjust()
{
    cout << "Color Object deleted!" << endl;
}

int Adjust::read_mcp3008_adc(unsigned char adcChannel)
{
    unsigned char buff[3];
    int adcValue = 0;
    buff[0] = 0x01;
    buff[1] = 0x80|((adcChannel & 0x07)<<4);
    buff[2] = 0x00;
    digitalWrite(CS_MCP3008, 0);
    wiringPiSPIDataRW(SPI_CHANNEL, buff, 3);
    buff[1] = 0x03 & buff[1];
    adcValue = (buff[1]<<8) | buff[2];
    digitalWrite(CS_MCP3008, 1);
    return adcValue;
}

void Adjust::setFlag(bool value)
{
    flag = value;
}
