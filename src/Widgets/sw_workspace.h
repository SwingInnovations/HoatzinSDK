#ifndef SW_WORKSPACE_H
#define SW_WORKSPACE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QSplitter>
//Local
#include "codeeditor.h"
#include "outliner.h"
#include "sceneview.h"
#include "timeline.h"

enum SW_Split{
    Single = 0,
    Split_Horizontal = 1,
    Split_Vertical = 2
};

enum SW_ActiveWidget{
    CE = 0,
    SV = 1,
    TL = 2,
    OT = 3
};

class SW_Workspace : public QWidget
{
    Q_OBJECT
public:
    SW_Workspace(QWidget* parent = 0);
    ~SW_Workspace();
    void syncCodeEditor(CodeEditor* editor);
    void setWidget(int);
signals:
    void singleEmitted();
public slots:
    void splitHorizView(int);
    void splitVertiView(int);
    void setSingleView(int);
    void setSingleView(){emit singleEmitted();}
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
