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
#include <QComboBox>
#include <QWidgetAction>

class CodeCanvas : public QTextEdit{
    Q_OBJECT
public:
    explicit CodeCanvas(QWidget* parent = 0);
    void highlightLine();
    int getFirstVisibleBlockID();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    QMenu* getEditMenu(){return mEditMenu;}
    QMenu* getLanguageMenu(){return mLanguageMenu;}
signals:
    void undoTriggered();
    void redoTriggered();
    void cutTriggered();
    void copyTriggered();
    void pasteTriggered();
    void refractorTriggered();
private slots:
    void updateUndo(){emit undoTriggered();}
    void updateRedo(){emit redoTriggered();}
    void updateCut(){emit cutTriggered();}
    void updateCopy(){emit copyTriggered();}
    void updatePaste(){emit pasteTriggered();}
    void updateRefractor(){emit refractorTriggered();}
protected:
    void resizeEvent(QResizeEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);
private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(QRectF);
    void updateLineNumberArea(int);
    void updateLineNumberArea();
private:
    QWidget *lineNumberArea;
    QMenu* mEditMenu;
    QMenu* mLanguageMenu;
    QAction *mUndoAct;
    QAction *mRedoAct;
    QAction *mCutAct;
    QAction *mCopyAct;
    QAction *mPasteAct;
    QAction *mRefractorAct;
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
signals:
    void singleViewTriggered(int);
    void splitHorizontalTriggered(int);
    void splitVerticalTriggered(int);
private slots:
    void selectCPP(bool);
    void selectLUA(bool);
    void selectGLSL(bool);
    void singleSelected();
    void splitHorizSelected();
    void splitVertiSelected();
private:
    CodeCanvas* codeCanvas;
    QVector<QString> mFilesOpen;
    QMenuBar* menuBar;
    QComboBox* mFileComboBox;
    QAction* mUndoAct;
    QAction* mRedoAct;
    QAction* mCopyAct;
    QAction* mCutAct;
    QAction* mPasteAct;
    QAction* mRefractorAct;

    QAction* cppLangAct;
    QAction* luaLangAct;
    QAction* glslLangAct;

    QAction* mCodeEditorWdgt;
    QAction* mSceneViewerWgdt;
    QAction* mTimelineWgdt;

    CPPHighLighter* mCPPHighlighter;
    LUAHighLighter* mLUAHighlighter;
    GLSLHighLighter* mGLSLlighter;
    QSyntaxHighlighter* masterHighlight;
};

#endif // CODEEDITOR_H
