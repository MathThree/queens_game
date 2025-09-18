#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::debug(QString newText, bool keep)
{
    debugText = !keep ? newText : debugText + newText;
    ui->debugText->setText(debugText);
}

