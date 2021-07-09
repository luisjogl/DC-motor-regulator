#ifndef PID_H
#define PID_H


class PID
{
public:
    PID(double kp, double ki, double kd, double dt);
    ~PID(){}

    void setKP(double kp) {_KP = kp;}
    void setKI(double ki) {_KI = ki;}
    void setKD(double kd) {_KD = kd;}
    void setSampleTime(double t) {_dt = t;}

    double getKP() {return _KP;}
    double getKI() {return _KI;}
    double getKD() {return _KD;}
    double getSampleTime() {return _dt;}

    double calcular(double, double);

private:
    double _KP;
    double _KI;
    double _KD;
    double _dt;
    double _error;
    double _integral;
    double _errorPrevio;

};

#endif // PID_H
