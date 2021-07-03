#include "pid.h"

PID::PID(double kp = 0, double ki = 0, double kd = 0, double dt = 1) {
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
    return accionControl;
}

