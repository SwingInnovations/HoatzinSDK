#include "codeeditor.h"

CodeEditor::CodeEditor(QWidget* parent) : QWidget(parent)
{
    codeCanvas = new CodeCanvas(this);
    QVBoxLayout* centerLayout = new QVBoxLayout;

    menuBar = new QMenuBar(this);
    QMenu *fileMenu = menuBar->addMenu("&File");
    QMenu *editMenu = menuBar->addMenu("&Edit");
    QMenu *languageMenu = menuBar->addMenu("&Language");
    QMenu *panelMenu = menuBar->addMenu("&Panel");

    mUndoAct = new QAction("&Undo",this);
    mRedoAct = new QAction("&Redo", this);
    mCopyAct = new QAction("&Cut", this);
    mCutAct = new QAction("&Cut", this);
    mPasteAct = new QAction("&Paste", this);

    QAction* cppLangAct = new QAction("&C++", this);
    QAction* luaLangAct = new QAction("&Lua", this);
    QAction* glslLangAct = new QAction("&GLSL", this);

    //QComboBox* documentSelect = new QComboBox(this);

    languageMenu->addAction(cppLangAct);
    languageMenu->addAction(luaLangAct);
    languageMenu->addAction(glslLangAct);

    centerLayout->addWidget(menuBar);
    centerLayout->addWidget(codeCanvas);
    setLayout(centerLayout);
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
    connect(this->document(), SIGNAL(blockCountChanged(int)), SLOT(updateLineNumberAreaWidth(int)));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), SLOT(updateLineNumberArea(int)));
    connect(this, SIGNAL(textChanged()), SLOT(updateLineNumberArea()));
    connect(this, SIGNAL(cursorPositionChanged()), SLOT(updateLineNumberArea()));
    updateLineNumberAreaWidth(0);
    highlightLine();
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

/*oid CodeCanvas::paintEvent(QPaintEvent *e){
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    QPainter painter(lineNumberArea);
    painter.fillRect(e->rect(), Qt::lightGray);
    int blockNumber = this->getFirstVisibleBlockID();
    QTextBlock block = this->document()->findBlockByNumber(blockNumber);
    //QTextBlock prevBlock = (blockNumber > 0)? this->document()->findBlockByNumber(blockNumber-1).position() : 0;
    int translate_y = (blockNumber > 0) ? this->verticalScrollBar()->sliderPosition() : 0;
    int top = this->viewport()->geometry().top();
    int bottom = top + (int)this->document()->documentLayout()->blockBoundingRect(block).height();
    while(block.isValid() && top <= e->rect().top()){
        if(block.isVisible() && bottom >= e->rect().top()){
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + (int)this->document()->documentLayout()->blockBoundingRect(block).height();
        blockNumber++;
    }
}*/

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
                    << "\\bvoid\\b" << "\\bvolatile\\b";
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

