#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QAction>
#include <QMenu>
#include "Widgets/sw_workspace.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    void initGui();
    SW_Workspace* mWorkspace;
};

#endif // MAINWINDOW_H
