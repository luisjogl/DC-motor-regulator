#include "mainwindow.h"
#include <QApplication>
#include "maquinaestados.h"
#include "wiringPi.h"
#include <iostream>
#include "boton.h"

#define BUTTON_PIN_1 0

Boton *pulsadorEmergencia = new Boton(BUTTON_PIN_1);

void myInterrupt(void) {
    emit pulsadorEmergencia->botonPulsado();
}
void *readBotones(void *) {
    // sets up the wiringPi library
    if(wiringPiSetup () < 0) {
        fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
    }
    // set Pin 17/0 generate an interrupt on high-to-low transitions
    // and attach myInterrupt() to the interrupt
    if( wiringPiISR (BUTTON_PIN_1, INT_EDGE_FALLING, &myInterrupt) < 0 ) {
        fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    pthread_t hiloBotones;
    pthread_create(&hiloBotones, NULL, readBotones, NULL);

    MainWindow w;
    MaquinaEstados sm(&w, pulsadorEmergencia);

    w.show();

    return a.exec();
}
