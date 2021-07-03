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
    explicit MaquinaEstados(MainWindow *gui, QObject *parent = nullptr);

private:
    QStateMachine *machine;
    QState *enMarcha;
    QHistoryState *ultimoEstado;
    QState *regulandoVelocidad;
    QState *regulandoPosicion;
    QState *paradaEmergencia;

    MainWindow *w;

    PID *reguladorPosicion;
    PID *reguladorVelocidad;

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
};

#endif // MAQUINAESTADOS_H
