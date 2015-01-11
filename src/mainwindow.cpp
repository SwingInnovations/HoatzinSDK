#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initGui();
    mWorkspace = new SW_Workspace(this);
    setCentralWidget(mWorkspace);
}

void MainWindow::initGui(){
    QMenu* fileMenu = menuBar()->addMenu("&File");
    QMenu* editMenu = menuBar()->addMenu("&Edit");
    QMenu* helpMenu = menuBar()->addMenu("&Help");
}

MainWindow::~MainWindow()
{

}
