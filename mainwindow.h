#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "boton.h"
#include "wiringPi.h"
#include "analoginput.h"

#define MODO_POSICION  1
#define MODO_VELOCIDAD 0

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupRealTimePlot();
    void myInterrupt();

//private:
    Ui::MainWindow *ui;
    Boton *pulsadorEmergencia;
    double refVel = 0;
    double refPos = 0;
    double valorReferencia = 0;
    double valorActual;
    double valorVelocidad;
    double valorPosicion;

    double P;
    double I;
    double D;
    double T;

signals:
    void hayCambioReferencia();

public slots:
    void updatePlot();

private slots:
    void on_botonModificarRefs_clicked();
    void actualizaControllerParams();
};

#endif // MAINWINDOW_H
