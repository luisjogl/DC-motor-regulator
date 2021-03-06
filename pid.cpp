#include "pid.h"
#include <iostream>

PID::PID(double kp, double ki, double kd, double dt) {
    _KP = kp;
    _KI = ki;
    _KD = kd;
    _dt = dt;
}

double PID::calcular(double valorObjetivo, double valorActual){
    _error = valorObjetivo - valorActual;

    double resultadoP = _KP * _error;

    _integral += _error * _dt;
    double resultadoI = _KI * _integral;

    double _diferencial = (_error - _errorPrevio) / _dt;
    double resultadoD = _KD * _diferencial;

    double accionControl = resultadoP + resultadoI + resultadoD;
    _errorPrevio = _error;

    //std::cout << accionControl << "\n";
    return accionControl;
}

