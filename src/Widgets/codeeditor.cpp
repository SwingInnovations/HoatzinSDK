#include "codeeditor.h"

CodeEditor::CodeEditor()
{

}

CodeEditor::~CodeEditor()
{

}

LineNumberArea::LineNumberArea(QTextEdit *editor) : QWidget(editor){
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
}

void CodeCanvas::paintEvent(QPaintEvent *e){
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    QPainter painter(lineNumberArea);
    painter.fillRect(e->rect(), Qt::lightGray);
    int blockNumber = this->getFirstVisibleBlockID();
    QTextBlock block = this->document()->findBlockByNumber(blockNumber);
    QTextBlock prevBlock = (blockNumber > 0)? this->document()->findBlockByNumber(blockNumber-1) : 0;
    int translate_y = (blockNumber > 0) ? this->verticalScrollBar()->sliderPosition() : 0;
    int top = this->viewport()->geometry().top();

    int addition_margin;
    if(blockNumber == 0){
        addition_margin = (int)this->document()->documentMargin() - 1 - this->verticalScrollBar()->sliderPosition();
    }else{
        addition_margin = (int)this->document()->documentLayout()->blockBoundingRect(prevBlock).translated(0, translate_y).intersects(this->viewport()->geometry()).height();
    }
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
