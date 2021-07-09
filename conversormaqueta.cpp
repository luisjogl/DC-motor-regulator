#include "conversormaqueta.h"


ConversorMaqueta::ConversorMaqueta(int resolucion, QObject *parent) : QObject(parent)
{
    maxBits = resolucion - 1;
    pendientePos = double(MAX_DEGREES/2)/(maxBits/2);
    pendienteVelCCW = double(VEL_MAX_CCW)/(522-338);
    pendienteVelCW = double(VEL_MAX_CW)/(727-532);
}

double ConversorMaqueta::BitsToDegrees(int bits) {
    if (bits < 0 || bits > maxBits) {
        std::cout << "Valor de posición en bits no válido \n";
    }
    else if ((0 <= bits) && (bits < (maxBits/2))) {
        degreesPos = bits * pendientePos + (MAX_DEGREES/2);

        return degreesPos;
    }
    else if (((maxBits/2) <= bits) && (bits < maxBits)) {
        degreesPos = bits * pendientePos - (MAX_DEGREES/2);

        return degreesPos;
    }
    else {
        degreesPos = 0;
    }
}

double ConversorMaqueta::BitsToRPMs(int bits) {
    if (bits <= 338) {
        rpmsVel = -VEL_MAX_CCW;
    }
    else if (bits > 338 && bits < 522) {
        rpmsVel = (pendienteVelCCW * (bits - 338)) - VEL_MAX_CCW;
        if (rpmsVel < -VEL_MAX_CCW) {
            rpmsVel = -VEL_MAX_CCW;
        }
    }
    else if (532 < bits && bits < 727) {
        rpmsVel = pendienteVelCW * (bits - 532);
        if (rpmsVel > VEL_MAX_CW) {
            rpmsVel = VEL_MAX_CW;
        }
    }
    else if (727 <= bits) {
        rpmsVel = VEL_MAX_CW;
    }
    else {
        rpmsVel = 0;
    }

    return rpmsVel;
}

int ConversorMaqueta::DegreesToBits(double degrees) {
    if (degrees >= MAX_DEGREES || degrees <= 0){
        return maxBits/2;
    }
    else if (degrees > 0 && degrees < MAX_DEGREES/2){
        return maxBits/2 + degrees/pendientePos;
    }
    else if (degrees < MAX_DEGREES && degrees >= MAX_DEGREES/2){
        return degrees/pendientePos - maxBits/2;
    }
}

int ConversorMaqueta::RPMsToBits(double rpms) {
    if (rpms < -VEL_MAX_CCW){
        return 338;
    }
    else if (rpms > VEL_MAX_CW){
        return 727;
    }
    else if (rpms < VEL_MAX_CW && rpms > 0){
        return rpms/pendienteVelCW + 532;
    }
    else if (rpms > -VEL_MAX_CCW && rpms < 0){
        return rpms/pendienteVelCCW + 338;
    }
    else{
        return 525;
    }

}
