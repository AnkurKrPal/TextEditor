#include "mainwindow.h"
#include <QKeyEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>

#include "../PieceTable.h"

//External backend objects
PieceTable P;
int currCursor = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      cursorVisible(true),
      cursorRow(0),
      cursorCol(0)
{
    setWindowTitle("Text Editor");
    resize(800, 600);

    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    font.setPointSize(16);
    setFont(font);

    QFontMetrics metrics(font);
    cellWidth  = metrics.horizontalAdvance('M');
    cellHeight = metrics.height();

    cursorTimer = new QTimer(this);
    connect(cursorTimer, &QTimer::timeout, this, &MainWindow::toggleCursorVisibility);
    cursorTimer->start(500);

    cachedText = "";
}

MainWindow::~MainWindow() {}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);
    painter.setPen(Qt::black);
    painter.setFont(font());

    QStringList lines = cachedText.split('\n');
    visibleLines = height() / cellHeight;
    visibleCols  = width()  / cellWidth;

    int total = (int)lines.size();

    for (int i = scrollOffset; i < std::min(scrollOffset + visibleLines, total); ++i) {
        QString line = lines[i];

        // ✅ Only draw the part of the line currently visible horizontally
        QString visiblePart;
        if (hScrollOffset < line.length()) {
            visiblePart = line.mid(hScrollOffset, visibleCols);
        }

        painter.drawText(0, (i - scrollOffset + 1) * cellHeight, visiblePart);
    }

    drawCursor(painter);
}


void MainWindow::drawCursor(QPainter &painter) {
    if (!cursorVisible) return;

    int x = (cursorCol - hScrollOffset) * cellWidth;
    int y = (cursorRow - scrollOffset) * cellHeight;

    // Do not draw cursor if it's outside visible region horizontally
    if (x < 0 || x > width()) return;

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRect(x, y, 2, cellHeight);
}


void MainWindow::toggleCursorVisibility() {
    cursorVisible = !cursorVisible;
    update();
}

void MainWindow::finalizeCursorMove() {
    if (P.state == 2) { P.weightUpdator2(P.head, P.currIndex); P.delCount = 0; P.current_piece = NULL; }
    if (P.state == 1) { P.weightUpdator(P.head, P.currIndex);  P.current_piece = NULL; }
    if (P.length2 > 0) P.undo.push(new laststep(P.type2, P.start2, P.length2, P.cursorStart, P.charStack));
    P.length2 = 0;
    P.state   = 0;
    P.GlobalIndex = currCursor;
}

void MainWindow::updateCursorPosition() {
    cursorRow = cursorCol = 0;
    QStringList lines = cachedText.split('\n');
    int index = currCursor;

    for (int i = 0; i < lines.size(); ++i) {
        if (index <= lines[i].length()) { cursorRow = i; cursorCol = index; return; }
        index -= lines[i].length() + 1;
    }
}

int MainWindow::computeCursorIndexFromMouse(int x, int y) {
    int row = y / cellHeight + scrollOffset;
    int col = x / cellWidth;
    QStringList lines = cachedText.split('\n');

    if (row < 0) row = 0;
    if (row >= lines.size()) row = lines.size() - 1;

    col = std::min(col, (int)lines[row].length());
    int index = 0;
    for (int i = 0; i < row; ++i) index += lines[i].length() + 1;
    return index + col;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    finalizeCursorMove();
    currCursor = computeCursorIndexFromMouse(event->position().x(), event->position().y());
    P.GlobalIndex = currCursor;
    updateCursorPosition();
    scrollToCursor();
    update();
}

void MainWindow::scrollToCursor() {
    visibleLines = height() / cellHeight;
    visibleCols  = width()  / cellWidth;

    //Vertical scroll
    if (cursorRow < scrollOffset)
        scrollOffset = cursorRow;
    if (cursorRow >= scrollOffset + visibleLines)
        scrollOffset = cursorRow - visibleLines + 1;

    // Horizontal scroll
    if (cursorCol < hScrollOffset)
        hScrollOffset = cursorCol;
    if (cursorCol >= hScrollOffset + visibleCols)
        hScrollOffset = cursorCol - visibleCols + 1;

    if (scrollOffset < 0) scrollOffset = 0;
    if (hScrollOffset < 0) hScrollOffset = 0;

    update();
}


void MainWindow::wheelEvent(QWheelEvent *event) {
    scrollOffset -= (event->angleDelta().y() > 0 ? 1 : -1);
    QStringList lines = cachedText.split('\n');
    int total = (int)lines.size();
    visibleLines = height() / cellHeight;

    if (scrollOffset < 0) scrollOffset = 0;
    if (scrollOffset > total - visibleLines) scrollOffset = std::max(0, total - visibleLines);

    update();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Arrow keys
    if (event->key() == Qt::Key_Left) {
        finalizeCursorMove();
        if (currCursor > 0) currCursor--;
        P.GlobalIndex = currCursor;
        updateCursorPosition(); scrollToCursor(); update(); return;
    }
    if (event->key() == Qt::Key_Right) {
        finalizeCursorMove();
        if (currCursor < cachedText.length()) currCursor++;
        P.GlobalIndex = currCursor;
        updateCursorPosition(); scrollToCursor(); update(); return;
    }

    if (event->key() == Qt::Key_Up) {
        finalizeCursorMove();
        QStringList L = cachedText.split('\n');
        if (cursorRow > 0) {
            int newRow = cursorRow - 1;
            int newCol = std::min(cursorCol, (int)L[newRow].length());
            int index = 0; for (int i = 0; i < newRow; ++i) index += L[i].length() + 1;
            currCursor = index + newCol;
            P.GlobalIndex = currCursor;
        }
        updateCursorPosition(); scrollToCursor(); update(); return;
    }

    if (event->key() == Qt::Key_Down) {
        finalizeCursorMove();
        QStringList L = cachedText.split('\n');
        if (cursorRow < L.size() - 1) {
            int newRow = cursorRow + 1;
            int newCol = std::min(cursorCol, (int)L[newRow].length());
            int index = 0; for (int i = 0; i < newRow; ++i) index += L[i].length() + 1;
            currCursor = index + newCol;
            P.GlobalIndex = currCursor;
        }
        updateCursorPosition(); scrollToCursor(); update(); return;
    }

    // Ctrl+Z Undo / Ctrl+Y Redo
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->key() == Qt::Key_Z) performUndo(P, currCursor);
        else if (event->key() == Qt::Key_Y) performRedo(P, currCursor);
        cachedText = QString::fromStdString(P.printTrial(P.head));
        updateCursorPosition(); scrollToCursor(); update(); return;
    }

    // Enter
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        insertChar(P, '\n', currCursor);
        cachedText = QString::fromStdString(P.printTrial(P.head));
        updateCursorPosition(); scrollToCursor(); update(); return;
    }

    // Backspace
    if (event->key() == Qt::Key_Backspace) {
        if (currCursor > 0) { if (P.state != 2) P.delCount = 0; P.delCount++; P.deletion(currCursor, 0); currCursor--; }
        cachedText = QString::fromStdString(P.printTrial(P.head));
        updateCursorPosition(); scrollToCursor(); update(); return;
    }

    // Character input
    QString t = event->text();
    if (t.size() == 1) {
        char c = t.toLatin1()[0];
        if (c >= 32 && c <= 126) {
            insertChar(P, c, currCursor);
            cachedText = QString::fromStdString(P.printTrial(P.head));
            updateCursorPosition(); scrollToCursor(); update(); return;
        }
    }
}