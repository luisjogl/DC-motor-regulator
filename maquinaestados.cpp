#include "maquinaestados.h"
#include <QDebug>
#include "ui_mainwindow.h"
#include "pid.h"
#include "analoginputmock.h"
//#include "boton.h"
//#include "readBotones.cpp"

MaquinaEstados::MaquinaEstados(MainWindow *gui, Boton *pulsEmer, QObject *parent) : QObject(parent)
{
    w = gui;
    pEmer = pulsEmer;
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
    //añadir transicion al pulsar pulsador de parada de emergencia para cambiar a estado de emergencia estando en marcha
    //añadir transicion al pulsar pulsador de rearme para cambiar a en marcha estando en emergencia

    connect(paradaEmergencia, SIGNAL(activeChanged(bool)), this, SLOT(enParadaEmergencia()));
    connect(regulandoPosicion, SIGNAL(activeChanged(bool)), this, SLOT(enRegulandoPosicion()));
    connect(regulandoVelocidad, SIGNAL(activeChanged(bool)), this, SLOT(enRegulandoVelocidad()));
    connect(w, SIGNAL(hayCambioReferencia()), this, SLOT(cambiaRefPlot()));

    QTimer *regulationTimer = new QTimer(this);
    connect(regulationTimer, SIGNAL(timeout()), this, SLOT(calculaAccionControl()));
    regulationTimer->start(100);

    enMarcha->addTransition(w->pulsadorEmergencia, SIGNAL(botonPulsado()), paradaEmergencia);

    machine->setInitialState(enMarcha);
    enMarcha->setInitialState(ultimoEstado);
    machine->start();
}

void MaquinaEstados::enParadaEmergencia(){
    if (paradaEmergencia->active() == true){
        w->ui->labelEstado->setText("EN PARADA DE EMERGENCIA \n"
                                    "(Pulse Rearme para reanudar)");
        //enciendeLED()
        w->valorReferencia = 0;
    }
}

/*
void MaquinaEstados::enEstadoMarcha(){
    if (enMarcha->active() == true){
    qDebug() << "En Estado Marcha";
    }

}
*/

void MaquinaEstados::enRegulandoPosicion(){
    if (regulandoPosicion->active() == true){
        reguladorPosicion = new PID(w->P,w->I,w->D,w->T);
        w->ui->labelEstado->setText("REGULANDO POSICIÓN");
        w->valorReferencia = w->refPos;
        w->ui->customPlot->yAxis->setLabel("Posición (º)");
    }
}

void MaquinaEstados::enRegulandoVelocidad(){
    if(regulandoVelocidad->active() == true){
        w->ui->labelEstado->setText("REGULANDO VELOCIDAD");
        reguladorVelocidad = new PID(w->P,w->I,w->D,w->T);
        w->valorReferencia = w->refVel;
        w->ui->customPlot->yAxis->setLabel("Velocidad (RPM)");
    }
}

void MaquinaEstados::calculaAccionControl(){
    if (regulandoPosicion->active() == true){
        accionControl = reguladorPosicion->calcular(w->valorReferencia, getAnalogValue());
    }
    else if (regulandoVelocidad->active() == true) {
        accionControl = reguladorVelocidad->calcular(w->valorReferencia, getAnalogValue());
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

