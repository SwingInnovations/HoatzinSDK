#include "sw_workspace.h"

SW_Workspace::SW_Workspace(QWidget* parent) : QWidget(parent)
{
    mSplitMode = SW_Split::Single;
    mActiveWidget = SW_ActiveWidget::CE;
    centralLayout = new QVBoxLayout;
    mCodeEditor = new CodeEditor(this);
    centralLayout->addWidget(mCodeEditor);
    centralLayout->setMargin(4);
    connect(mCodeEditor, SIGNAL(singleViewTriggered(int)), SLOT(setSingleView(int)));
    connect(mCodeEditor, SIGNAL(splitHorizontalTriggered(int)), SLOT(splitHorizView(int)));
    connect(mCodeEditor, SIGNAL(splitVerticalTriggered(int)), SLOT(splitVertiView(int)));
    setLayout(centralLayout);
}

SW_Workspace::~SW_Workspace()
{

}

void SW_Workspace::setWidget(int val){
    centralLayout->removeItem(centralLayout->itemAt(0));
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
    SW_Workspace* adjust2 = (SW_Workspace*)centralSplit->widget(0);
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
    SW_Workspace* adjust2 = (SW_Workspace*)centralSplit->widget(0);
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

}
