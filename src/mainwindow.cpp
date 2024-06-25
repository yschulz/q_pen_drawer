#include "q_pen_drawer/mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMouseTracking(true);
    connect(ui->spinBox, SIGNAL(valueChanged(int)), ui->graphicsView,  SLOT(updateAverageLength(int)));
    connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), ui->graphicsView,  SLOT(setMinimumRadius(double)));
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    std::cout << "going not good\n";
    QMainWindow::mouseMoveEvent(event);
    
    if(ui->pushButton->underMouse()){
        std::cout << "going good\n";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}