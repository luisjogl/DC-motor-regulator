#ifndef MAQUINAESTADOS_H
#define MAQUINAESTADOS_H

#include <QObject>
#include <QStateMachine>
#include "mainwindow.h"
#include <QHistoryState>
#include "pid.h"
#include "pwm.h"

#define MODO_POSICION  1
#define MODO_VELOCIDAD 0

class MaquinaEstados : public QObject
{
    Q_OBJECT

public:
    explicit MaquinaEstados(MainWindow *gui, Boton *pulsEmer, Boton *pulsRearme, QObject *parent = nullptr);

private:
    QStateMachine *machine;
    QState *enMarcha;
    QHistoryState *ultimoEstado;
    QState *controlVelocidad;
    QState *controlPosicion;
    QState *paradaEmergencia;

    MainWindow *w;
    Boton *pEmer;
    Boton *pRearme;

    PID *reguladorPosicion;
    PID *reguladorVelocidad;

    int modoControl;

    QTimer *regulationTimer;
    QTimer *blinkingTimer;

    int estadoLED = 0;

    PWM *pwm;

signals:

public slots:
    void realizaControl();
    //generaPWM()
    void cambiaRefPlot();
    void parpadeaLED();
    void modificaParamsReguladores();

private slots:
    void saleControlVelocidad();
    void finalizaEmergencia();
    void entraEmergencia();
    void entraControlVelocidad();
    void entraControlPosicion();
    void saleControlPosicion();
};

#endif // MAQUINAESTADOS_H
