#include "codeeditor.h"
#include <QPainter>
#include <QTextBlock>
#include <QTimer>
#include <QDesktopServices>
#include <QDebug>

/**
 * @brief CodeEditor::CodeEditor 实现显示行号及当前行高亮、语法高亮
 * @param parent
 */
CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    this->setFont(font);

    highlighter = new Highlighter(this->document());

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    hide_flag = false; //是否选择隐藏
}


int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    if(!hide_flag){
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    }
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::hideLineNumberArea(bool flag)
{
    if(!flag){
        lineNumberArea->hide();
        setViewportMargins(0, 0, 0, 0);
        hide_flag = !flag;
    }else{
        lineNumberArea->show();
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    }
}

void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

//处理点击事件
void CodeEditor::mousePressEvent(QMouseEvent *event)
{
    QPlainTextEdit::mousePressEvent(event);

    if (event->button() == Qt::LeftButton) {
        qDebug() << "左键";
        int col = 0;
        int row = 0;
        int flag = -1;
        int pos = this->textCursor().position();
        QString text = this->toPlainText();

        for(int i = 0;i < pos;i++){
            if(text[i] == '\n'){
                row++;
                flag = i;
            }
        }
        flag++;
        col = pos - flag;
        int curPos = col+1;

        QTextCursor cursor = cursorForPosition(event->pos());
        QTextBlock block = cursor.block();
        QString textBlock = block.text();
        int l,r;
        l = col,r = col;
        while(textBlock[l] != " " && l > 0){
            l--;
        }
        while(textBlock[r] != " " && r < textBlock.length()){
            r++;
        }
        QString href = textBlock.mid(l,r-l).trimmed();
        qDebug() << href;

        if (href.startsWith("http://") || href.startsWith("https://")) {
            qDebug() << "超链接";
            QDesktopServices::openUrl(href);
        }
    }
}

void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::cyan).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::gray);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    while (block.isValid() && top <= event->rect().bottom()) {
           if (block.isVisible() && bottom >= event->rect().top()) {
               QString number = QString::number(blockNumber + 1);
               painter.setPen(Qt::black);
               painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                                Qt::AlignRight, number);
           }

           block = block.next();
           top = bottom;
           bottom = top + qRound(blockBoundingRect(block).height());
           ++blockNumber;
       }
}


/**
 * @brief Highlighter::Highlighter 实现语法高亮
 * @param parent
 */
Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns //C++ 关键字
                    << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                    << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b"
                       // Java 关键字
                    << "\\babstract\\b" << "\\bcontinue\\b" << "\\bfor\\b"
                    << "\\bpackage\\b" << "\\bsynchronized\\b" << "\\bassert\\b"
                    << "\\bboolean\\b" << "\\bsuper\\b" << "\\bimport\\b"
                    << "\\bextends\\b" << "\\bimplements\\b" << "\\bthrows\\b"
                    << "\\bthrows\\b" << "\\bvinstanceof\\b" << "\\btransient\\b"
                    << "\\bextends\\b" << "\\bstrictfp\\b" << "\\bfinal\\b"
                    << "\\binterface\\b" << "\\bfinally\\b" << "\\bnative\\b"
                    << "\\bthrow\\b" << "\\bString\\b"
                       //python 关键字
                    << "\\band\\b" << "\\bFalse\\b" << "\\bNone\\b"
                    << "\\bTrue\\b" << "\\bas\\b" << "\\bbreak\\b"
                    << "\\bdef\\b" << "\\bdel\\b" << "\\belif\\b"
                    << "\\belse\\b" << "\\bexcept\\b" << "\\bfrom\\b"
                    << "\\bglobal\\b" << "\\bif\\b" << "\\bin\\b"
                    << "\\bis\\b" << "\\blambda\\b" << "\\bnonlocal\\b"
                    << "\\bnot\\b" << "\\bor\\b" << "\\bpass\\b"
                    << "\\braise\\b" << "\\breturn\\b" << "\\btry\\b"
                    << "\\bwhile\\b" << "\\bwith\\b" << "\\byield\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    hyperHerfFormat.setForeground(Qt::blue);
    hyperHerfFormat.setFontUnderline(true);
    hyperHerfFormat.setAnchor(true);
    rule.pattern = QRegularExpression("(?<=\\s|^)((?:https?://|www\\d{0,3}[.]|[a-z0-9.\\-]+[.][a-z]{2,4}/)(?:[^\\s()<>]+|\\(([^\\s()<>]+|(\\([^\\s()<>]+\\)))*\\))+(?:\\(([^\\s()<>]+|(\\([^\\s()<>]+\\)))*\\)|[^\\s`!()\\[\\]{};:'\".,<>?«»“”‘’]))(?=\\s|$)");
    rule.format = hyperHerfFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}


void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}

