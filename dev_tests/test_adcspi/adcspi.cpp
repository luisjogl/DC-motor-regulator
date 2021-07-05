#include "adcspi.h"
#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace  std;


AdcSpi::AdcSpi(int mode)
{
    for (int i=0; i < 2; i++){
        if ( wiringPiSPISetup(i,10000)==-1) {
            cerr << "error al abrir el canal "<<i<<" SPI " <<  strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }
    }
    switch (mode){
    case 0 :    setSingleEndded();
        break;
    case 8 :    setDiffEndded();
        cout << "SPI mode="<< mode <<" single, correctamente inicializado"<<endl;
        break;
    default : cerr << "error al establecer el modo SINGLE o DIFF " <<  strerror(errno) << endl;
    }
}

void AdcSpi::setSingleEndded() {
    channelConfigMode= CHAN_CONFIG_SINGLE;
}

void AdcSpi::setDiffEndded() {
    channelConfigMode= CHAN_CONFIG_DIFF;
}

void AdcSpi:: setSpiChannel(int channel){
    spiChannel= channel;
}

int AdcSpi::adcReadSingle(int ADCInputChannel){
    try {
        if ( ADCInputChannel  < 0 || ADCInputChannel >7){
            throw (ADCInputChannel );
        }

        bufferCom[0]=1;     //start bit
        bufferCom[1]=static_cast <unsigned char>  ( (channelConfigMode + ADCInputChannel) << 4);
        bufferCom[2]=0;
        wiringPiSPIDataRW(spiChannel,bufferCom,3);
        return ( ( (bufferCom[1] & 3) << 8 ) + bufferCom[2] );
    }
    catch    (int ADCInputChannel ) {
        cerr << "El canal elegido es "<< ADCInputChannel<< " es erróneo, debe estar entre 0 y 7" <<  strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}
