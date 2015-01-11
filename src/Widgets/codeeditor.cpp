#include "codeeditor.h"

CodeEditor::CodeEditor(QWidget* parent) : QWidget(parent)
{
    codeCanvas = new CodeCanvas(this);
    codeCanvas->setTabStopWidth(10);
    codeCanvas->setFont(QFont("Consolas"));
    codeCanvas->setLineWrapMode(QTextEdit::NoWrap);
    QVBoxLayout* centerLayout = new QVBoxLayout;

    menuBar = new QMenuBar(this);
    QMenu *fileMenu = menuBar->addMenu("&File");
    QMenu *editMenu = menuBar->addMenu("&Edit");
    QMenu *languageMenu = menuBar->addMenu("&Language");
    QMenu *panelMenu = menuBar->addMenu("&Panel");
    menuBar->addSeparator();

    mFileComboBox = new QComboBox(menuBar);
//    QWidgetAction* comboWidget = new QWidgetAction(menuBar);
//    comboWidget->setDefaultWidget(mFileComboBox);
    menuBar->setCornerWidget(mFileComboBox, Qt::TopRightCorner);

    mUndoAct = new QAction("&Undo",this);
    mRedoAct = new QAction("&Redo", this);
    mCopyAct = new QAction("&Cut", this);
    mCutAct = new QAction("&Cut", this);
    mPasteAct = new QAction("&Paste", this);
    mRefractorAct = new QAction("&Refractor", this);

    cppLangAct = new QAction("&C++", this);
    cppLangAct->setCheckable(true);
    cppLangAct->setChecked(true);
    luaLangAct = new QAction("&Lua", this);
    luaLangAct->setCheckable(true);
    glslLangAct = new QAction("&GLSL", this);
    glslLangAct->setCheckable(true);

    mCodeEditorWdgt = new QAction("&Text Editor", this);
    mSceneViewerWgdt = new QAction("&3D View", this);
    mTimelineWgdt = new QAction("&Timeline", this);

    QAction* singleViewAct = new QAction("Single", this);
    QAction* splitHorizonalAct = new QAction("Split Horizontal", this);
    QAction* splitVerticalAct = new QAction("Split Vertical", this);

    editMenu->addAction(mUndoAct);
    editMenu->addAction(mRedoAct);
    editMenu->addSeparator();
    editMenu->addAction(mCutAct);
    editMenu->addAction(mCopyAct);
    editMenu->addAction(mPasteAct);
    editMenu->addSeparator();
    editMenu->addAction(mRefractorAct);

    languageMenu->addAction(cppLangAct);
    languageMenu->addAction(luaLangAct);
    languageMenu->addAction(glslLangAct);

    QMenu* layoutMenu = panelMenu->addMenu("&Layouts");
    layoutMenu->addAction(singleViewAct);
    layoutMenu->addAction(splitHorizonalAct);
    layoutMenu->addAction(splitVerticalAct);
    panelMenu->addSeparator();
    panelMenu->addAction(mCodeEditorWdgt);
    panelMenu->addAction(mSceneViewerWgdt);
    panelMenu->addAction(mTimelineWgdt);

    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->addWidget(menuBar);
    centerLayout->addWidget(codeCanvas);
    setLayout(centerLayout);

    masterHighlight = new CPPHighLighter(codeCanvas->document());

    connect(cppLangAct, SIGNAL(triggered(bool)), SLOT(selectCPP(bool)));
    connect(luaLangAct, SIGNAL(triggered(bool)), SLOT(selectLUA(bool)));
    connect(glslLangAct, SIGNAL(triggered(bool)), SLOT(selectGLSL(bool)));
    connect(mCopyAct, SIGNAL(triggered()), codeCanvas, SLOT(copy()));
    connect(mCutAct, SIGNAL(triggered()), codeCanvas, SLOT(cut()));
    connect(mPasteAct, SIGNAL(triggered()), codeCanvas, SLOT(paste()));
    connect(codeCanvas, SIGNAL(copyTriggered()), codeCanvas, SLOT(copy()));
    connect(codeCanvas, SIGNAL(cutTriggered()), codeCanvas, SLOT(cut()));
    connect(codeCanvas, SIGNAL(pasteTriggered()), codeCanvas, SLOT(paste()));
    connect(singleViewAct, SIGNAL(triggered()), SLOT(singleSelected()));
    connect(splitHorizonalAct, SIGNAL(triggered()), SLOT(splitHorizSelected()));
    connect(splitVerticalAct, SIGNAL(triggered()), SLOT(splitVertiSelected()));
}

void CodeEditor::selectCPP(bool val){
    cppLangAct->setChecked(val);
    luaLangAct->setChecked(!val);
    glslLangAct->setChecked(!val);
}

void CodeEditor::selectGLSL(bool val){
    cppLangAct->setChecked(!val);
    luaLangAct->setChecked(!val);
    glslLangAct->setChecked(val);
}

void CodeEditor::selectLUA(bool val){
    cppLangAct->setChecked(!val);
    luaLangAct->setChecked(val);
    glslLangAct->setChecked(!val);
}

void CodeEditor::singleSelected(){
    emit singleViewTriggered(0);
}

void CodeEditor::splitHorizSelected(){
    emit splitHorizontalTriggered(0);
}

void CodeEditor::splitVertiSelected(){
    emit splitVerticalTriggered(0);
}

CodeEditor::~CodeEditor()
{

}

LineNumberArea::LineNumberArea(CodeCanvas *editor) : QWidget(editor){
    codeEditor = editor;
}

QSize LineNumberArea::sizeHint() const{
    return QSize(((CodeCanvas *)codeEditor)->lineNumberAreaWidth(), 0);
}

CodeCanvas::CodeCanvas(QWidget* parent) : QTextEdit(parent){
    lineNumberArea = new LineNumberArea(this);
    mUndoAct = new QAction("&Undo", this);
    mRedoAct = new QAction("&Redo", this);
    mCopyAct = new QAction("&Copy", this);
    mCutAct = new QAction("&Cut", this);
    mPasteAct = new QAction("&Paste", this);
    mRefractorAct = new QAction("&Refractor", this);

    connect(this->document(), SIGNAL(blockCountChanged(int)), SLOT(updateLineNumberAreaWidth(int)));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(updateLineNumberArea(int)));
    connect(this, SIGNAL(textChanged()), SLOT(updateLineNumberArea()));
    connect(this, SIGNAL(cursorPositionChanged()), SLOT(updateLineNumberArea()));
    connect(mUndoAct, SIGNAL(triggered()), SLOT(updateUndo()));
    connect(mRedoAct, SIGNAL(triggered()), SLOT(updateRedo()));
    connect(mCopyAct, SIGNAL(triggered()), SLOT(updateCopy()));
    connect(mCutAct, SIGNAL(triggered()), SLOT(updateCut()));
    connect(mPasteAct, SIGNAL(triggered()), SLOT(updatePaste()));
    connect(mRefractorAct, SIGNAL(triggered()), SLOT(updateRefractor()));

    updateLineNumberAreaWidth(0);
    highlightLine();
}

void CodeCanvas::contextMenuEvent(QContextMenuEvent *e){
    QMenu* menu = new QMenu(this);
    menu->addAction(mUndoAct);
    menu->addAction(mRedoAct);
    menu->addSeparator();
    menu->addAction(mCutAct);
    menu->addAction(mCopyAct);
    menu->addAction(mPasteAct);
    menu->addSeparator();
    menu->addAction(mRefractorAct);
    menu->exec(viewport()->mapToGlobal(e->pos()));
    delete menu;
}

int CodeCanvas::lineNumberAreaWidth(){
    int digits = 1;
    int max = qMax(1, this->document()->blockCount());
    while(max >= 10){
        max /= 19;
        digits++;
    }
    int space = 13 + fontMetrics().width(QLatin1Char('9'))*digits;
    return space;
}

void CodeCanvas::updateLineNumberAreaWidth(int newBlockCount){
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeCanvas::updateLineNumberArea(QRectF rect){
    CodeCanvas::updateLineNumberArea();
}

void CodeCanvas::updateLineNumberArea(int sliderPos){
    CodeCanvas::updateLineNumberArea();
}

void CodeCanvas::updateLineNumberArea(){
   this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    QRect rect = this->contentsRect();
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    updateLineNumberAreaWidth(0);
    int dY = this->verticalScrollBar()->sliderPosition();
    if(dY > -1){
        lineNumberArea->scroll(0, dY);
    }

    int firstBlockID = getFirstVisibleBlockID();
    if(firstBlockID == 0 || this->textCursor().block().blockNumber() == firstBlockID -1){
        this->verticalScrollBar()->setSliderPosition(dY-this->document()->documentMargin());
    }
    highlightLine();
}

void CodeCanvas::lineNumberAreaPaintEvent(QPaintEvent *event){
       this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::darkGray);
    int blockNumber = getFirstVisibleBlockID();
    QTextBlock block = document()->findBlockByNumber(blockNumber);
    int offset = 0;
    if(blockNumber > 0) offset = this->verticalScrollBar()->sliderPosition(); else offset = 0;
    int top = (int)document()->documentLayout()->blockBoundingRect(block).translated(QPoint(0, -offset)).top();
    int bottom = top + (int)document()->documentLayout()->blockBoundingRect(block).height();
    while(block.isValid() && top <= event->rect().bottom()){
        if(block.isVisible() && bottom >= event->rect().top()){
            QString number = QString::number(blockNumber+1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + (int)document()->documentLayout()->blockBoundingRect(block).height();
        blockNumber++;
    }
}

void CodeCanvas::highlightLine(){
    QList<QTextEdit::ExtraSelection> extraSelection;
    if(!isReadOnly()){
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelection.append(selection);
    }
    setExtraSelections(extraSelection);
}

void CodeCanvas::resizeEvent(QResizeEvent *e){
    QTextEdit::resizeEvent(e);

    QRect cr = this->contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

int CodeCanvas::getFirstVisibleBlockID(){
    QTextCursor curs = QTextCursor(this->document());
    curs.movePosition(QTextCursor::Start);
    for(int i = 0; i < this->document()->blockCount(); i++){
        QTextBlock block = curs.block();
        QRect r1 = this->viewport()->geometry();
        QRect r2 = this->document()->documentLayout()->blockBoundingRect(block).translated(
                    this->viewport()->geometry().x(), this->viewport()->geometry().y() - (
                      this->verticalScrollBar()->sliderPosition())).toRect();
        if(r1.contains(r2, true)){return i;}
        curs.movePosition(QTextCursor::NextBlock);
    }
    return 0;
}

CPPHighLighter::CPPHighLighter(QTextDocument *parent) : QSyntaxHighlighter(parent){
    HighlightRule rule;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                    << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b";
    foreach(const QString& pattern, keywordPatterns){
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightRules.append(rule);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightRules.append(rule);

   functionFormat.setFontItalic(true);
   functionFormat.setForeground(Qt::blue);
   rule.pattern = QRegExp("\\b[A-za-z0-9_]+(?=\\()");
   rule.format = functionFormat;
   highlightRules.append(rule);

   singleLineCommentFormat.setForeground(Qt::red);
   rule.pattern = QRegExp("//[^\n]*");
   rule.format = singleLineCommentFormat;
   highlightRules.append(rule);

   multiLineCommentFormat.setForeground(Qt::red);

   commentStartExpression = QRegExp("/\\*");
   commentEndExpression = QRegExp("\\*/");
}

void CPPHighLighter::highlightBlock(const QString &text){
    foreach(const HighlightRule &rule, highlightRules){
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while(index >= 0){
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
        setCurrentBlockState(0);
        int startIndex = 0;
        if(previousBlockState() != 1){
            startIndex = commentStartExpression.indexIn(text);
        }

        while(startIndex >= 0){
            int endIndex = commentEndExpression.indexIn(text, startIndex);
            int commentLength;
            if(endIndex == -1){
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            }else{
                commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
            }

            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
        }
    }
}

LUAHighLighter::LUAHighLighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{

}

void LUAHighLighter::highlightBlock(const QString &text){
    foreach(const HighlightRule &rule, highlightRules){
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while(index > 0){
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
        setCurrentBlockState(0);
        int startIndex = 0;
        if(previousBlockState() != 1){
            startIndex = commentStartExpression.indexIn(text);
        }

        while(startIndex >= 0){
            int endIndex = commentEndExpression.indexIn(text, startIndex);
            int commentLength;
            if(endIndex == -1){
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            }else{
                commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
            }

            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
        }
    }
}

GLSLHighLighter::GLSLHighLighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{

}

void GLSLHighLighter::highlightBlock(const QString &text){
    foreach(const HighlightRule &rule, highlightRules){
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while(index > 0){
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
        setCurrentBlockState(0);
        int startIndex = 0;
        if(previousBlockState() != 1){
            startIndex = commentStartExpression.indexIn(text);
        }

        while(startIndex >= 0){
            int endIndex = commentEndExpression.indexIn(text, startIndex);
            int commentLength;
            if(endIndex == -1){
                setCurrentBlockState(1);
                commentLength = text.length() - startIndex;
            }else{
                commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
            }

            setFormat(startIndex, commentLength, multiLineCommentFormat);
            startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
        }
    }
}

