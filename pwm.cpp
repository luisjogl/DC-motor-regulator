#include "pwm.h"

PWM::PWM(QObject *parent) : QObject(parent)
{
    wiringPiSetup();		/* initialize wiringPi setup */
    pinMode(PWM_PIN,OUTPUT);	/* set GPIO as output */
    softPwmCreate(PWM_PIN,1,100);
}

int PWM::calculaValorPWM(double u){
   double dutyCycle = u/MAX_VEL_CW;
   double dutyCycleMaqueta = dutyCycle + 0.5;
//   int valorPWM = dutyCycleMaqueta*1023;
//   return valorPWM;
   return dutyCycleMaqueta*100;
}

void PWM::generaPWM(int valPWM){
//    if (valPWM > 1023){
//        valPWM = 1023;
//    }
//    else if (valPWM < 0){
//        valPWM = 0;
//    }
//    pwmWrite(PWM_PIN, valPWM);
    softPwmWrite(PWM_PIN, valPWM);
}
