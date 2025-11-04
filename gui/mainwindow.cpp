#include "mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include "../PieceTable.h"
PieceTable P;
int currCursor=0;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      cursorVisible(true),
      cursorRow(0),
      cursorCol(0)
{
    setWindowTitle("Text Editor");
    resize(800, 600);

    // Use monospace font
    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    font.setPointSize(16);
    setFont(font);

    // Calculate grid size based on font metrics
    QFontMetrics metrics(font);
    cellWidth = metrics.horizontalAdvance('M'); // width of one monospace char
    cellHeight = metrics.height();              // height of one line

    // Set up blinking cursor timer
    cursorTimer = new QTimer(this);
    connect(cursorTimer, &QTimer::timeout, this, &MainWindow::toggleCursorVisibility);
    cursorTimer->start(500); // blink every 500ms
}

MainWindow::~MainWindow() {}

void MainWindow::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white); // background

    drawGrid(painter);
    drawCursor(painter);
    drawCenterText(painter); // <-- Draw "hi" in the center
}

void MainWindow::drawGrid(QPainter &painter) {
    painter.setPen(QColor(255, 255, 255)); // white lines (invisible grid)
    int cols = width() / cellWidth;
    int rows = height() / cellHeight;

    // Draw vertical lines
    for (int c = 0; c <= cols; ++c) {
        int x = c * cellWidth;
        painter.drawLine(x, 0, x, height());
    }

    // Draw horizontal lines
    for (int r = 0; r <= rows; ++r) {
        int y = r * cellHeight;
        painter.drawLine(0, y, width(), y);
    }
}

void MainWindow::drawCursor(QPainter &painter) {
    if (!cursorVisible) return;

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);

    int x = cursorCol * cellWidth;
    int y = cursorRow * cellHeight;

    painter.drawRect(x, y, 2, cellHeight); // thin blinking vertical line
}

void MainWindow::drawCenterText(QPainter &painter) {
    painter.setPen(Qt::black);
    painter.setFont(font());
    //////////////////// INSERTING SOME TEXT //////////////////////
    insertChar(P,'a',currCursor);
    insertChar(P,'b',currCursor);
    insertChar(P,'\n',currCursor);
    insertChar(P,'a',currCursor);
    insertChar(P,'a',currCursor);
    std::string myStdString = P.printTrial(P.head);
    QString text = QString::fromStdString(myStdString);


    QFontMetrics metrics(font());
    int textWidth = metrics.horizontalAdvance("hi"); // width of longest line
    int textHeight = metrics.height();

    // Split text into lines
    QStringList lines = text.split('\n');

    int totalHeight = lines.size() * textHeight;
    int startY = (height() - totalHeight) / 2 + textHeight; // vertical center

    for (int i = 0; i < lines.size(); ++i) {
        int x = (width() - textWidth) / 2;          // horizontal center
        int y = startY + i * textHeight;            // line by line
        painter.drawText(x, y, lines[i]);
    }
}


void MainWindow::toggleCursorVisibility() {
    cursorVisible = !cursorVisible;
    update(); // repaint window
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    int x = event->position().x();
    int y = event->position().y();

    cursorCol = x / cellWidth;
    cursorRow = y / cellHeight;

    qDebug() << "Clicked cell:" << "Row =" << cursorRow << ", Col =" << cursorCol;

    update(); // move cursor and repaint
}
