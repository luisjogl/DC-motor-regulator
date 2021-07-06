#ifndef ANALOGINPUT_H
#define ANALOGINPUT_H

#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPiSPI.h>
#include <iostream>

#define CHAN_CONFIG_SINGLE  8
#define CHAN_CONFIG_DIFF    0
#define SPICHANNEL          0

#define POSITION_ADC_CHANNEL    1
#define VELOCITY_ADC_CHANNEL    0

static int myFd ;

volatile int posicionMotorBits;
volatile int velocidadMotorBits;

void spiSetup(int);

int myAnalogRead(int,int,int);

int readAnalogInput(int);

void *readEntradasMaqueta(void *);

int getAnalogValue(int);

#endif // ANALOGINPUT_H
