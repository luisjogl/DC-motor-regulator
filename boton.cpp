#include "boton.h"

Boton::Boton(int pin, QObject *parent) : QObject(parent)
{
    numeroPin = pin;
}
