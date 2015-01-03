#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QTextEdit>
#include <QScrollBar>
#include <QTextBlock>
#include <QTextCursor>
#include <QAbstractTextDocumentLayout>
#include <QPainter>

class CodeEditor
{
public:
    CodeEditor();
    ~CodeEditor();
};

class LineNumberArea : public QWidget{
    Q_OBJECT
public:
    LineNumberArea(QTextEdit* editor);
    QSize sizeHint() const;
private:
    QTextEdit* codeEditor;
};

class CodeCanvas : public QTextEdit{
    Q_OBJECT
public:
    explicit CodeCanvas(QWidget* parent = 0);
    int getFirstVisibleBlockID();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
protected:
    void resizeEvent(QResizeEvent *e);
    void paintEvent(QPaintEvent *e);
private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(QRectF);
    void updateLineNumberArea(int);
    void updateLineNumberArea();
private:
    QWidget *lineNumberArea;
};
#endif // CODEEDITOR_H
