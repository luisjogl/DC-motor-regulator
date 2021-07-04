#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <boton.h>
#include "mainwindow.h"
#include "maquinaestados.h"

// Use GPIO Pin 17, which is Pin 0 for wiringPi library

//#define BUTTON_PIN 0

// the event counter
//volatile int eventCounter = 0;

// -------------------------------------------------------------------------

//Boton *pulsadorEmergencia = new Boton(0);

//void interruptEmergencia(Boton *pulsEmer) {
void myInterrupt(void) {
    emit pulsadorEmergencia->botonPulsado();
}

// -------------------------------------------------------------------------
void readBotones(Boton *pulsEmer) {
    // sets up the wiringPi library
    if(wiringPiSetup () < 0) {
        fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno));
    }
    // set Pin 17/0 generate an interrupt on high-to-low transitions
    // and attach myInterrupt() to the interrupt
    if( wiringPiISR (pulsEmer->numeroPin, INT_EDGE_FALLING, &interruptEmergencia) < 0 ) {
        fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
    }
}
