#include <iostream>
#include "analoginputmock.h"
#include <cmath>

void analogInputMockSetNewValue(double angle, double offset, double amplitude){
    value = amplitude * sin(angle) + offset;
}

// override for mocks
double getAnalogValue(){
    return value;
}

void *readAnalogInputMock(void *){
    double angle = 0.0;
    double offset = 50;
    double amplitude = 10;
    while(1){
        analogInputMockSetNewValue(angle, offset, amplitude);
        angle += 10;
        //std::cout << getAnalogValue() << "\n" ;
    }
}
