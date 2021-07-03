#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "analoginputmock.cpp"
#include "maquinaestados.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::setupRealTimePlot();

    pthread_t analogInputThread;
    pthread_create(&analogInputThread, NULL, readAnalogInputMock, NULL);

    QTimer *plotTimer = new QTimer(this);
    connect(plotTimer, SIGNAL(timeout()), this, SLOT(updatePlot()));
    plotTimer->start(50);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupRealTimePlot(){
    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->customPlot->addGraph();
    ui->customPlot->graph(1)->setPen(QPen(Qt::red));

    ui->customPlot->xAxis->setLabel("Tiempo (s)");
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setLabel("Velocidad (rad/s)");
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setFont(QFont("Helvetica",9));
    ui->customPlot->graph(0)->setName("Valor actual");
    ui->customPlot->graph(1)->setName("Valor referencia");
}

void MainWindow::updatePlot(){
    //std::cout << getAnalogValue();
    static QTime time(QTime::currentTime());
    double now = time.elapsed()/1000.0;

    double valorActual = getAnalogValue();

    double valorReferencia = refVel;

    static float minAxeY=0;
    static float maxAxeY=0;

    if (valorReferencia > maxAxeY) maxAxeY= valorReferencia;
    if (valorActual > maxAxeY) maxAxeY= valorActual;

    if (valorReferencia <  minAxeY) minAxeY= valorReferencia;
    if (valorActual < minAxeY) minAxeY= valorActual;

    ui->customPlot->graph(0)->addData(now, valorActual);
    ui->customPlot->graph(1)->addData(now, valorReferencia);

    ui->customPlot->graph(0)->rescaleAxes(true);
    ui->customPlot->graph(1)->rescaleAxes(true);

    ui->customPlot->graph(0)->rescaleKeyAxis(true);
    ui->customPlot->graph(0)->rescaleKeyAxis(true);

    ui->customPlot->xAxis->setRange(now,10, Qt::AlignRight);
    ui->customPlot->yAxis->setRange (minAxeY*1.1,maxAxeY*1.1);

    ui->customPlot->replot();
}



void MainWindow::on_botonModificarRefs_clicked()
{
    refVel = ui->spinBoxRefVel->value();
}
