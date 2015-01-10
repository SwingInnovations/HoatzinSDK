#include "sw_workspace.h"

SW_Workspace::SW_Workspace(QWidget* parent) : QWidget(parent)
{
    mSplitMode = SW_Split::Single;
    mActiveWidget = SW_ActiveWidget::CodeEditor;
}

SW_Workspace::~SW_Workspace()
{

}

void SW_Workspace::setWidget(int){

}

void SW_Workspace::syncCodeEditor(CodeEditor *editor){

}

void SW_Workspace::setSingleView(){

}
