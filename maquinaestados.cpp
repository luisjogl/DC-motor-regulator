#include "maquinaestados.h"
#include <QDebug>
#include "ui_mainwindow.h"
//#include "analoginputmock.h"

#define LED_PIN 2

#define ESTADO_VELOCIDAD    0
#define ESTADO_POSICION     1
#define ESTADO_EMERGENCIA   2

MaquinaEstados::MaquinaEstados(MainWindow *gui, Boton *pulsEmer, Boton *pulsRearme, QObject *parent) : QObject(parent)
{
    w = gui;
    pEmer = pulsEmer;
    pRearme = pulsRearme;
    machine = new QStateMachine();
    enMarcha = new QState(machine);
    paradaEmergencia = new QState(machine);
    controlPosicion = new QState(enMarcha);
    controlVelocidad = new QState(enMarcha);
    ultimoEstado = new QHistoryState(enMarcha);
    ultimoEstado->setDefaultState(controlVelocidad);

    enMarcha->addTransition(w->ui->botonParadaEmergencia, SIGNAL(clicked()), paradaEmergencia);
    paradaEmergencia->addTransition(w->ui->botonRearme, SIGNAL(clicked()), enMarcha);
    controlPosicion->addTransition(w->ui->botonCambiaModo, SIGNAL(clicked()), controlVelocidad);
    controlVelocidad->addTransition(w->ui->botonCambiaModo, SIGNAL(clicked()), controlPosicion);
    enMarcha->addTransition(pEmer, SIGNAL(botonPulsado()), paradaEmergencia);
    paradaEmergencia->addTransition(pRearme, SIGNAL(botonPulsado()), enMarcha);

    connect(controlPosicion, SIGNAL(entered()), this, SLOT(entraControlPosicion()));
    connect(controlPosicion, SIGNAL(exited()), this, SLOT(saleControlPosicion()));
    connect(controlVelocidad, SIGNAL(entered()), this, SLOT(entraControlVelocidad()));
    connect(controlVelocidad, SIGNAL(exited()), this, SLOT(saleControlVelocidad()));
    connect(w, SIGNAL(hayCambioReferencia()), this, SLOT(cambiaRefPlot()));
    connect(paradaEmergencia, SIGNAL(entered()), this, SLOT(entraEmergencia()));
    connect(paradaEmergencia, SIGNAL(exited()), this, SLOT(finalizaEmergencia()));
    connect(w, SIGNAL(hayCambioParamsControllerUI()), this, SLOT(modificaParamsReguladores()));

    reguladorPosicion = new PID(w->rPosP,w->rPosI,w->rPosD,w->rPosT);
    reguladorVelocidad = new PID(w->rVelP,w->rVelI,w->rVelD,w->rVelT);

    regulationTimer = new QTimer(this);
    connect(regulationTimer, SIGNAL(timeout()), this, SLOT(realizaControl()));
    regulationTimer->start(100);

    blinkingTimer = new QTimer(this);
    connect(blinkingTimer, SIGNAL(timeout()), this, SLOT(parpadeaLED()));

    wiringPiSetup() ;
    pinMode (LED_PIN, OUTPUT) ;

    machine->setInitialState(enMarcha);
    enMarcha->setInitialState(ultimoEstado);
    machine->start();
}

void MaquinaEstados::entraEmergencia(){
    w->ui->labelEstado->setText("EN PARADA DE EMERGENCIA \n"
                                "(Pulse Rearme para reanudar)");
    w->valorReferencia = 0;
    blinkingTimer->start(500);
}

void MaquinaEstados::entraControlPosicion(){
    modoControl = ESTADO_POSICION;
    w->ui->labelEstado->setText("REGULANDO POSICIÓN");
    w->valorReferencia = w->refPos;
    w->valorActual = w->valorPosicion;
    w->ui->customPlot->yAxis->setLabel("Posición (º)");
    w->setControllerParams(w->rPosP,w->rPosI,w->rPosD,w->rPosT);
}

void MaquinaEstados::entraControlVelocidad(){
    modoControl = ESTADO_VELOCIDAD;
    w->ui->labelEstado->setText("REGULANDO VELOCIDAD");
    w->valorReferencia = w->refVel;
    w->ui->customPlot->yAxis->setLabel("Velocidad (RPMs)");
    w->setControllerParams(w->rVelP,w->rVelI,w->rVelD,w->rVelT);
}

void MaquinaEstados::saleControlVelocidad(){
    w->rVelP = w->P;
    w->rVelI = w->I;
    w->rVelD = w->D;
    w->rVelT = w->T;
}

void MaquinaEstados::saleControlPosicion(){
    w->rPosP = w->P;
    w->rPosI = w->I;
    w->rPosD = w->D;
    w->rPosT = w->T;
}

void MaquinaEstados::realizaControl(){
    if (modoControl == ESTADO_VELOCIDAD){
        double valorActual = w->ADC->readAnalogInput(ESTADO_VELOCIDAD);
        w->valorActual = valorActual;
        double valorReferencia = w->valorReferencia;
        double accionControl = reguladorVelocidad->calcular(valorReferencia, valorActual);
        std::cout << "Accion Control VEL: " << accionControl << "\n";
        // a partir de esta accion de control genera PWM
    }
    else if (modoControl == ESTADO_POSICION){
        double valorActual = w->ADC->readAnalogInput(ESTADO_POSICION);
        w->valorActual = valorActual;
        double valorReferencia = w->valorReferencia;
        double accionControl = reguladorVelocidad->calcular(valorReferencia, valorActual);
        std::cout << "Accion Control POS: " << accionControl << "\n";
        // a partir de esta accion de control genera PWM
    }
    else {
        double valorActual = w->ADC->readAnalogInput(ESTADO_VELOCIDAD);
        w->valorActual = valorActual;
        double valorReferencia = w->valorReferencia;
        double accionControl = reguladorVelocidad->calcular(valorReferencia, valorActual);
    }
}

void MaquinaEstados::cambiaRefPlot(){
    if(controlVelocidad->active() == true){
        w->valorReferencia = w->refVel;
    }
    else if (controlPosicion->active() == true){
        w->valorReferencia = w->refPos;
    }
    else {
        w->valorReferencia = 0;
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

void MaquinaEstados::modificaParamsReguladores(){
    if(controlVelocidad->active() == true){
        reguladorVelocidad->setKP(w->P);
        reguladorVelocidad->setKI(w->I);
        reguladorVelocidad->setKD(w->D);
        reguladorVelocidad->setSampleTime(w->T);
    }
    else if (controlPosicion->active() == true){
        w->valorReferencia = w->refPos;
        reguladorPosicion->setKP(w->P);
        reguladorPosicion->setKI(w->I);
        reguladorPosicion->setKD(w->D);
        reguladorPosicion->setSampleTime(w->T);
    }
    else {
        w->valorReferencia = 0;
        reguladorVelocidad->setKP(w->P);
        reguladorVelocidad->setKI(w->I);
        reguladorVelocidad->setKD(w->D);
        reguladorVelocidad->setSampleTime(w->T);
    }
}
