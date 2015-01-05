#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QTextEdit>
#include <QScrollBar>
#include <QTextBlock>
#include <QTextCursor>
#include <QAbstractTextDocumentLayout>
#include <QPainter>
#include <QSyntaxHighlighter>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>

class CodeCanvas : public QTextEdit{
    Q_OBJECT
public:
    explicit CodeCanvas(QWidget* parent = 0);
    void highlightLine();
    int getFirstVisibleBlockID();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
protected:
    void resizeEvent(QResizeEvent *e);
private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(QRectF);
    void updateLineNumberArea(int);
    void updateLineNumberArea();
private:
    QWidget *lineNumberArea;
};

class LineNumberArea : public QWidget{
    Q_OBJECT
public:
    LineNumberArea(CodeCanvas* editor);
    QSize sizeHint() const;
protected:
    void paintEvent(QPaintEvent *event){
        codeEditor->lineNumberAreaPaintEvent(event);
    }
private:
    CodeCanvas* codeEditor;
};


class CPPHighLighter : public QSyntaxHighlighter{
    Q_OBJECT
public:
    CPPHighLighter(QTextDocument* parent = 0);
protected:
    void highlightBlock(const QString& text);
private:
    struct HighlightRule{
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightRule> highlightRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

class LUAHighLighter : public QSyntaxHighlighter{
    Q_OBJECT
public:
    LUAHighLighter(QTextDocument* parent = 0);
protected:
    void highlightBlock(const QString& text);
private:
    struct HighlightRule{
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightRule> highlightRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

class GLSLHighLighter : public QSyntaxHighlighter{
    Q_OBJECT
public:
    GLSLHighLighter(QTextDocument* parent = 0);
protected:
    void highlightBlock(const QString& text);
private:
    struct HighlightRule{
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightRule> highlightRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};

class CodeEditor : public QWidget
{
    Q_OBJECT
public:
    CodeEditor(QWidget* parent = 0);
    ~CodeEditor();
private:
    CodeCanvas* codeCanvas;
    QMenuBar* menuBar;
};

#endif // CODEEDITOR_H
