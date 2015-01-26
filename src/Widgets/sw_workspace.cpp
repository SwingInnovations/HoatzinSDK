#include "sw_workspace.h"

SW_Workspace::SW_Workspace(QWidget* parent) : QWidget(parent)
{
    mSplitMode = SW_Split::Single;
    mActiveWidget = SW_ActiveWidget::CE;
    centralLayout = new QVBoxLayout;
    mCodeEditor = new CodeEditor(this);

    QTabWidget* tabWidget = new QTabWidget(this);
    tabWidget->setTabPosition(QTabWidget::North);
    tabWidget->setTabShape(QTabWidget::Triangular);
    tabWidget->addTab(mCodeEditor, "Document");

    centralLayout->addWidget(tabWidget);
    centralLayout->setMargin(4);
    setLayout(centralLayout);
}

SW_Workspace::~SW_Workspace()
{

}

void SW_Workspace::setWidget(int val){
    //centralLayout->removeItem(centralLayout->itemAt(0));
    switch(val){
    case SW_ActiveWidget::CE:
        centralLayout->addWidget(mCodeEditor);
        break;
    case SW_ActiveWidget::SV:
        break;
    case SW_ActiveWidget::TL:
        break;
    case SW_ActiveWidget::OT:
        break;
    default:
        centralLayout->addWidget(mCodeEditor);
    }
}

void SW_Workspace::syncCodeEditor(CodeEditor *editor){

}

void SW_Workspace::splitHorizView(int val){
    centralSplit = new QSplitter(Qt::Horizontal, this);
    centralSplit->setHandleWidth(4);
    centralSplit->addWidget(new SW_Workspace(centralSplit));
    centralSplit->addWidget(new SW_Workspace(centralSplit));
    SW_Workspace* adjust1 = (SW_Workspace*)centralSplit->widget(0);
    SW_Workspace* adjust2 = (SW_Workspace*)centralSplit->widget(1);
    adjust1->setWidget(val);
    adjust2->setWidget(val);
    delete centralLayout;
    mCodeEditor->hide();
    centralLayout = new QVBoxLayout;
    centralLayout->addWidget(centralSplit);
    centralLayout->setMargin(4);
    setLayout(centralLayout);
    repaint();
}

void SW_Workspace::splitVertiView(int val){
    centralSplit = new QSplitter(Qt::Vertical, this);
    centralSplit->addWidget(new SW_Workspace(centralSplit));
    centralSplit->addWidget(new SW_Workspace(centralSplit));
    SW_Workspace* adjust1 = (SW_Workspace*)centralSplit->widget(0);
    SW_Workspace* adjust2 = (SW_Workspace*)centralSplit->widget(1);
    adjust1->setWidget(val);
    adjust2->setWidget(val);
    delete centralLayout;
    mCodeEditor->hide();
    centralLayout = new QVBoxLayout;
    centralLayout->setMargin(4);
    centralLayout->addWidget(centralSplit);
    setLayout(centralLayout);
    repaint();
}

void SW_Workspace::setSingleView(int val){
    if(this->parent()!=0){
        SW_Workspace* tempHandle = (SW_Workspace*)this->parentWidget();
        //tempHandle->hide();
        //delete tempHandle->centralSplit;
        //delete tempHandle->centralLayout;
        tempHandle->centralLayout = new QVBoxLayout;
        tempHandle->setWidget(val);
        tempHandle->setLayout(tempHandle->centralLayout);
    }
}
