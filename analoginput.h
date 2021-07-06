#ifndef ANALOGINPUT_H
#define ANALOGINPUT_H

#include <QObject>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPiSPI.h>
#include <iostream>
#include <thread>

#define CHAN_CONFIG_SINGLE  8
#define CHAN_CONFIG_DIFF    0
#define SPICHANNEL          0

#define POSITION_ADC_CHANNEL    1
#define VELOCITY_ADC_CHANNEL    0


class AnalogInput : public QObject
{
    Q_OBJECT
public:
    explicit AnalogInput(QObject *parent = nullptr);

    void spiSetup(int);

    int myAnalogRead(int,int,int);

    int readAnalogInput(int);

//    void readEntradasMaqueta();

//    int getAnalogValue(int);

//    int getPosicionMotorBits(){ return posicionMotorBits;}
//    int getVelocidadMotorBits(){ return velocidadMotorBits;}

private:

//    volatile int posicionMotorBits;
//    volatile int velocidadMotorBits;

signals:

public slots:
};

#endif // ANALOGINPUT_H
