#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "analoginputmock.cpp"

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
    ui->customPlot->xAxis->setLabel("Tiempo (s)");
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setLabel("Velocidad (rad/s)");
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->legend->setFont(QFont("Helvetica",9));
    ui->customPlot->graph(0)->setName("Valor actual");
}

void MainWindow::updatePlot(){
    //std::cout << getAnalogValue();
    static QTime time(QTime::currentTime());
    double now = time.elapsed()/1000.0;

    double y1 = getAnalogValue();

    static float minAxeY=0;
    static float maxAxeY=0;

    //if (ref1> maxAxeY) maxAxeY= ref1;
    if (y1> maxAxeY) maxAxeY= y1;

    //if (ref1 <  minAxeY) minAxeY= ref1;
    if (y1 < minAxeY) minAxeY= y1;

    // añadir nuevo par de datos al gráfico:

    ui->customPlot->graph(0)->addData(now, y1);  // dibujar ref


        // Sin Reescalado de los ejes, la curva se comprime porque se
        // aumenta la ventana temporal
        // Si se quitan los comentarios la ventana temporal cambia sus límtes
        // pero no su tamaño.La curva se desplaza, aparece por la dcha y se va por la izq.
    //
    ui->customPlot->graph(0)->rescaleAxes(true);

    ui->customPlot->graph(0)->rescaleKeyAxis(true);

    ui->customPlot->xAxis->setRange(now,10, Qt::AlignRight);
    ui->customPlot->yAxis->setRange (minAxeY*1.1,maxAxeY*1.1);

    ui->customPlot->replot();
}


