#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QAction>
#include <QMenu>
#include "Widgets/codeeditor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void initGui();
    CodeEditor* editor;
};

#endif // MAINWINDOW_H
