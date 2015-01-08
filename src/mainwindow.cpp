#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    initGui();
    editor = new CodeEditor(this);
    setCentralWidget(editor);
}

void MainWindow::initGui(){
    QMenu* fileMenu = menuBar()->addMenu("&File");
    QMenu* editMenu = menuBar()->addMenu("&Edit");
    QMenu* helpMenu = menuBar()->addMenu("&Help");
}

MainWindow::~MainWindow()
{

}
