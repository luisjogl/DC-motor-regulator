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
    //std::thread hiloLectorAnalog(&AnalogInput::readEntradasMaqueta, this);


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

//void AnalogInput::readEntradasMaqueta(){
//    //TODO: crear clase Recta con metodos bitsToGrados y bitsToRPMs. Atributos son valores max, min e intermedios de recta (private)

//    //while (1){
//        posicionMotorBits = readAnalogInput(POSITION_ADC_CHANNEL);
//        velocidadMotorBits = readAnalogInput(VELOCITY_ADC_CHANNEL);
//        std::cout << "Pos: " << posicionMotorBits << "\n";
//        std::cout << "Vel: " << velocidadMotorBits << "\n";
//    //}
//}

//int AnalogInput::getAnalogValue(int modo){
//    if (modo == 0){
//        std::cout << "Velocidad: " <<velocidadMotorBits << "\n";
//        return velocidadMotorBits;   //por el momento
//        //TODO: realizar conversion a velocidadMotorGrados segun recta y retornar esto
//    }
//    else if (modo == 1){
//        return posicionMotorBits;   //por el momento
//        //TODO: realizar conversion a posicionMotorGrados segun recta y retornar esto
//    }
//    else {
//        std::cout << "Sólo hay entradas analógicas en los canales 0 y 1 del ADC \n";
//    }

//}


