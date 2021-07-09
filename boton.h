#ifndef BOTON_H
#define BOTON_H

#include <QObject>

class Boton : public QObject
{
    Q_OBJECT
public:
    explicit Boton(int pin, QObject *parent = nullptr);

int numeroPin;

signals:
    void botonPulsado();

public slots:
};

#endif // BOTON_H
