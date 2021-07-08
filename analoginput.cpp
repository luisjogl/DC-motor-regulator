#include "analoginput.h"

static int myFd ;

AnalogInput::AnalogInput(QObject *parent) : QObject(parent)
{
    if(wiringPiSetup()<0)
    {
        printf("Error setting up lectura ADC\n");
        exit(1);
    }
    spiSetup(SPICHANNEL);
}

void AnalogInput::spiSetup (int spiChannel)
{
    if ((myFd = wiringPiSPISetup (spiChannel, 10000) < 0))
    {
        fprintf (stderr, "Can't open the SPI bus: %s\n", strerror (errno)) ;
        exit (EXIT_FAILURE) ;
    }
}

int AnalogInput::myAnalogRead(int spiChannel,int channelConfig,int analogChannel)
{
    if(analogChannel<0 || analogChannel>7)
        return -1;
    unsigned char buffer[3] = {1}; // start bit
    buffer[1] = (channelConfig+analogChannel) << 4;
    wiringPiSPIDataRW(spiChannel, buffer, 3);
    return ( (buffer[1] & 3 ) << 8 ) + buffer[2]; // get last 10 bits
}

int AnalogInput::readAnalogInput(int ADC_channel)
{
    int adc;

    adc = myAnalogRead(SPICHANNEL,CHAN_CONFIG_SINGLE,ADC_channel);

    return adc;
}


