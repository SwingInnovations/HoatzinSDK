#ifndef SW_WORKSPACE_H
#define SW_WORKSPACE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>
//Local
#include "sw_base.h"
#include "codeeditor.h"
#include "sceneview.h"
#include "timeline.h"

class SW_Workspace : public QWidget
{
    Q_OBJECT
public:
    SW_Workspace(QWidget* parent = 0);
    ~SW_Workspace();
    void syncCodeEditor(CodeEditor* editor);
    void setWidget(int);
private slots:
    void setSingleView();
private:
    SW_ActiveWidget mActiveWidget;
    SW_Split mSplitMode;
    CodeEditor* mCodeEditor;
    Timeline* mTimeLine;
    SceneView* mSceneView;
    QVBoxLayout* centralLayout;
    QSplitter* centralSplit;
};

#endif // SW_WORKSPACE_H
