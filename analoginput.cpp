#include "analoginput.h"

void spiSetup (int spiChannel)
{
    if ((myFd = wiringPiSPISetup (spiChannel, 10000)) < 0)
    {
        fprintf (stderr, "Can't open the SPI bus: %s\n", strerror (errno)) ;
        exit (EXIT_FAILURE) ;
    }
}

int myAnalogRead(int spiChannel,int channelConfig,int analogChannel)
{
    if(analogChannel<0 || analogChannel>7)
        return -1;
    unsigned char buffer[3] = {1}; // start bit
    buffer[1] = (channelConfig+analogChannel) << 4;
    wiringPiSPIDataRW(spiChannel, buffer, 3);
    return ( (buffer[1] & 3 ) << 8 ) + buffer[2]; // get last 10 bits
}

int readAnalogInput(int ADC_channel)
{
    int adc;

    adc = myAnalogRead(SPICHANNEL,CHAN_CONFIG_SINGLE,ADC_channel);

    return adc;
}

void *readEntradasMaqueta(void *){
    if(wiringPiSetup()<0)
    {
        printf("Error setting up lectura ADC\n");
        exit(1);
    }
    spiSetup(SPICHANNEL);

    //TODO: crear clase Recta con metodos bitsToGrados y bitsToRPMs. Atributos son valores max, min e intermedios de recta (private)

    while (1){
        posicionMotorBits = readAnalogInput(POSITION_ADC_CHANNEL);
        velocidadMotorBits = readAnalogInput(VELOCITY_ADC_CHANNEL);
        std::cout << "Pos: " << posicionMotorBits << "\n";
        std::cout << "Vel: " << velocidadMotorBits << "\n";
    }
}

int getAnalogValue(int modo){
    if (modo == 2){
        return velocidadMotorBits;   //por el momento
        //TODO: realizar conversion a velocidadMotorGrados segun recta y retornar esto
    }
    else if (modo == 1){
        return posicionMotorBits;   //por el momento
        //TODO: realizar conversion a posicionMotorGrados segun recta y retornar esto
    }

}
