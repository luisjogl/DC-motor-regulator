#ifndef PWM_H
#define PWM_H

#include <QObject>
#include <wiringPi.h>
#include <stdio.h>
#include <softPwm.h>

#define PWM_PIN 7
#define MAX_VEL_CW 47

class PWM : public QObject
{
    Q_OBJECT
public:
    explicit PWM(QObject *parent = nullptr);

    int calculaValorPWM(double u);
    void generaPWM(int valPWM);

signals:

public slots:
};

#endif // PWM_H
