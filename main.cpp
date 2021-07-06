#include "mainwindow.h"
#include <QApplication>
#include "maquinaestados.h"
#include "wiringPi.h"
#include <iostream>
#include "boton.h"

#define BUTTON_PIN_1 0  // GPIO17
#define BUTTON_PIN_2 1  // GPIO18

Boton *pulsadorEmergencia = new Boton(BUTTON_PIN_1);
Boton *pulsadorRearme = new Boton(BUTTON_PIN_2);


void myInterruptEmergencia(void) {
    emit pulsadorEmergencia->botonPulsado();
}

void myInterruptRearme(void) {
    emit pulsadorRearme->botonPulsado();
}

void *readBotones(void *) {
    // sets up the wiringPi library
    if(wiringPiSetup () < 0) {
        fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
    }
    // set Pin 17/0 generate an interrupt on high-to-low transitions
    // and attach myInterrupt() to the interrupt
    if( wiringPiISR (BUTTON_PIN_1, INT_EDGE_FALLING, &myInterruptEmergencia) < 0 ) {
        fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
    }
    else if (wiringPiISR(BUTTON_PIN_2, INT_EDGE_FALLING, &myInterruptRearme) < 0) {
        fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    pthread_t hiloBotones;
    pthread_create(&hiloBotones, nullptr, readBotones, nullptr);

    MainWindow w; //(currVel,currPos)
    MaquinaEstados sm(&w, pulsadorEmergencia, pulsadorRearme);

    w.show();

    return a.exec();
}
