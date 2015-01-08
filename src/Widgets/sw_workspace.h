#ifndef SW_WORKSPACE_H
#define SW_WORKSPACE_H

#include <QWidget>
//Local
#include "codeeditor.h"
#include "sceneview.h"
#include "timeline.h"

class SW_Workspace : public QWidget
{
    Q_OBJECT
public:
    SW_Workspace(QWidget* parent = 0);
    ~SW_Workspace();
private:

};

#endif // SW_WORKSPACE_H
