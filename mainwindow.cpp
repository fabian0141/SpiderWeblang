#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "webpage/webpage.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Webpage* page = new Webpage(this);
    ui->centralWidget->layout()->addWidget(page);
}

MainWindow::~MainWindow()
{
    delete ui;
}
