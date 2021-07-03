#include "mainwindow.h"
#include <QApplication>
#include "maquinaestados.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    MaquinaEstados sm(&w);

    w.show();

    return a.exec();
}
