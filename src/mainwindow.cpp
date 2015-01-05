#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    editor = new CodeEditor(this);
    setCentralWidget(editor);
}

MainWindow::~MainWindow()
{

}
