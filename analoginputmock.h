#ifndef ANALOGINPUTMOCK_H
#define ANALOGINPUTMOCK_H

static double value;

void analogInputMockSetNewValue(double, double, double);

// override
double getAnalogValue();

void *readAnalogInputMock(void *);


#endif // ANALOGINPUTMOCK_H
