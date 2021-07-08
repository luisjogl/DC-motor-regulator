#ifndef CONVERSORMAQUETA_H
#define CONVERSORMAQUETA_H

#include <QObject>
#include <iostream>

#define MAX_DEGREES 359
#define VEL_MAX_CCW 45
#define VEL_MAX_CW  47

class ConversorMaqueta : public QObject
{
    Q_OBJECT
public:
    explicit ConversorMaqueta(int resolucion, QObject *parent = nullptr);

    double BitsToDegrees(int bits);

    double BitsToRPMs(int bits);
private:
    int maxBits;

    double pendientePos;
    double pendienteVelCW;
    double pendienteVelCCW;

    double degreesPos;
    double rpmsVel;

signals:

public slots:
};

#endif // CONVERSORMAQUETA_H
