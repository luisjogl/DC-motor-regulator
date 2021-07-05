#include "wiringPi.h"
#include <wiringPiSPi.h>
#include <iostream>

double setUp(){
    if(wiringPiSetup() == -1){
            printf("setup wiringPi failed!");
            return 1;
        }
    return 0;
}

//SPI
double SPIsetUp(){
    if ((wiringPiSPISetup (SPICHANNEL, 10000)) < 0){
        fprintf (stderr, "Can't open the SPI bus: %s\n", strerror (errno)) ;
        return 1;
    }
    return 0;
}

//Leer velocidad
int leeVel(){
    unsigned char buffer[3] = {1}; // start bit
    buffer[1] = (CHAN_CONFIG_SINGLE+ANALOGCHANNEL0) << 4;
    wiringPiSPIDataRW(SPICHANNEL, buffer, 3);
    return ((buffer[1] & 3 ) << 8 ) + buffer[2];
}

int main(){
    setUp();
    SPIsetUp();
    for(;;){
        std::cout << leeVel() << "\n";
    }
