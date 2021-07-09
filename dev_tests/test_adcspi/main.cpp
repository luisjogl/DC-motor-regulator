#include <iostream>
#include <iomanip>
#include <errno.h>
#include <string.h>
//#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "adcspi.h"

#define CANAL_ADC_V 0
//#define CANAL_ADC_POS 1

using namespace std;

int main()
{
    if ( wiringPiSetup() == -1)    {
        cerr << "error al inicializar wiringPi " <<  strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

	float velocidad;
    AdcSpi adcMCP3008Single;
    adcMCP3008Single.setSpiChannel(0);

	for (int i =0; i < 1000; i++) {
		velocidad = adcMCP3008Single.adcReadSingle(CANAL_ADC_V);
		delay(100);
		cout << velocidad << "\n";
	}

}


