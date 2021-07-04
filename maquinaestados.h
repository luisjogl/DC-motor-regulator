#ifndef MAQUINAESTADOS_H
#define MAQUINAESTADOS_H

#include <QObject>
#include <QStateMachine>
#include "mainwindow.h"
#include <QHistoryState>
#include "pid.h"


class MaquinaEstados : public QObject
{
    Q_OBJECT

public:
    explicit MaquinaEstados(MainWindow *gui, Boton *pulsEmer, Boton *pulsRearme, QObject *parent = nullptr);

private:
    QStateMachine *machine;
    QState *enMarcha;
    QHistoryState *ultimoEstado;
    QState *regulandoVelocidad;
    QState *regulandoPosicion;
    QState *paradaEmergencia;

    MainWindow *w;
    Boton *pEmer;
    Boton *pRearme;

    PID *reguladorPosicion;
    PID *reguladorVelocidad;

    QTimer *regulationTimer;
    QTimer *blinkingTimer;

    int estadoLED = 0;

    double valorReferencia;
    double accionControl;

signals:

public slots:
    void enParadaEmergencia();
    void enRegulandoVelocidad();
    void enRegulandoPosicion();
    void calculaAccionControl();
    //generaPWM()
    void cambiaRefPlot();
    void parpadeaLED();
    void finalizaEmergencia();
    void entraEmergencia();
};

#endif // MAQUINAESTADOS_H
