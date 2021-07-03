#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"

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

//private:
    Ui::MainWindow *ui;
    double refVel = 0;
    double refPos = 0;
    double valorReferencia = 0;

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
