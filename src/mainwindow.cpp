#include "q_pen_drawer/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->graphicsView,  SLOT(updateAverageLength(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}