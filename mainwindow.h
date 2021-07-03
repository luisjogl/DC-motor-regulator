#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

public slots:
    void updatePlot();

private slots:
    void on_botonModificarRefs_clicked();
};

#endif // MAINWINDOW_H
