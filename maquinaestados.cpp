#include "maquinaestados.h"
#include <QDebug>
#include "ui_mainwindow.h"
//#include "analoginputmock.h"

#define LED_PIN 2

MaquinaEstados::MaquinaEstados(MainWindow *gui, Boton *pulsEmer, Boton *pulsRearme, QObject *parent) : QObject(parent)
{
    w = gui;
    pEmer = pulsEmer;
    pRearme = pulsRearme;
    machine = new QStateMachine();
    enMarcha = new QState(machine);
    paradaEmergencia = new QState(machine);
    regulandoPosicion = new QState(enMarcha);
    regulandoVelocidad = new QState(enMarcha);
    ultimoEstado = new QHistoryState(enMarcha);
    ultimoEstado->setDefaultState(regulandoVelocidad);

    enMarcha->addTransition(w->ui->botonParadaEmergencia, SIGNAL(clicked()), paradaEmergencia);
    paradaEmergencia->addTransition(w->ui->botonRearme, SIGNAL(clicked()), enMarcha);
    regulandoPosicion->addTransition(w->ui->botonCambiaModo, SIGNAL(clicked()), regulandoVelocidad);
    regulandoVelocidad->addTransition(w->ui->botonCambiaModo, SIGNAL(clicked()), regulandoPosicion);
    enMarcha->addTransition(pEmer, SIGNAL(botonPulsado()), paradaEmergencia);
    paradaEmergencia->addTransition(pRearme, SIGNAL(botonPulsado()), enMarcha);

    connect(paradaEmergencia, SIGNAL(activeChanged(bool)), this, SLOT(enParadaEmergencia()));
    connect(regulandoPosicion, SIGNAL(activeChanged(bool)), this, SLOT(enRegulandoPosicion()));
    connect(regulandoVelocidad, SIGNAL(activeChanged(bool)), this, SLOT(enRegulandoVelocidad()));
    connect(w, SIGNAL(hayCambioReferencia()), this, SLOT(cambiaRefPlot()));
    connect(paradaEmergencia, SIGNAL(entered()), this, SLOT(entraEmergencia()));
    connect(paradaEmergencia, SIGNAL(exited()), this, SLOT(finalizaEmergencia()));

    regulationTimer = new QTimer(this);
    connect(regulationTimer, SIGNAL(timeout()), this, SLOT(calculaAccionControl()));
    regulationTimer->start(100);

    blinkingTimer = new QTimer(this);
    connect(blinkingTimer, SIGNAL(timeout()), this, SLOT(parpadeaLED()));

    wiringPiSetup() ;
    pinMode (LED_PIN, OUTPUT) ;

    machine->setInitialState(enMarcha);
    enMarcha->setInitialState(ultimoEstado);
    machine->start();
}

void MaquinaEstados::enParadaEmergencia(){
    if (paradaEmergencia->active() == true){
        w->ui->labelEstado->setText("EN PARADA DE EMERGENCIA \n"
                                    "(Pulse Rearme para reanudar)");
        w->valorReferencia = 0;
    }
}

void MaquinaEstados::entraEmergencia(){
    blinkingTimer->start(500);
}

void MaquinaEstados::enRegulandoPosicion(){
    if (regulandoPosicion->active() == true){
        reguladorPosicion = new PID(w->P,w->I,w->D,w->T);
        w->ui->labelEstado->setText("REGULANDO POSICIÓN");
        w->valorReferencia = w->refPos;
        w->valorActual = w->valorPosicion;
        w->ui->customPlot->yAxis->setLabel("Posición (º)");
    }
}

void MaquinaEstados::enRegulandoVelocidad(){
    if(regulandoVelocidad->active() == true){
        w->ui->labelEstado->setText("REGULANDO VELOCIDAD");
        reguladorVelocidad = new PID(w->P,w->I,w->D,w->T);
        w->valorReferencia = w->refVel;
        w->valorActual = w->valorVelocidad;
        w->ui->customPlot->yAxis->setLabel("Velocidad (RPM)");
    }
}

void MaquinaEstados::calculaAccionControl(){
    if (regulandoPosicion->active() == true){
        accionControl = reguladorPosicion->calcular(w->valorReferencia, getAnalogValue(MODO_POSICION));
    }
    else if (regulandoVelocidad->active() == true) {
        accionControl = reguladorVelocidad->calcular(w->valorReferencia, getAnalogValue(MODO_VELOCIDAD));
    }
}

void MaquinaEstados::cambiaRefPlot(){
    if(regulandoVelocidad->active() == true){
        w->valorReferencia = w->refVel;
    }
    else if (regulandoPosicion->active() == true){
        w->valorReferencia = w->refPos;
    }
}

void MaquinaEstados::parpadeaLED(){
    if (estadoLED == 0) {
        digitalWrite (LED_PIN,  HIGH) ;
        estadoLED = 1;
    }
    else {
        digitalWrite(LED_PIN, LOW);
        estadoLED = 0;
    }
}

void MaquinaEstados::finalizaEmergencia(){
    blinkingTimer->stop();
    digitalWrite (LED_PIN,  LOW) ;
}
