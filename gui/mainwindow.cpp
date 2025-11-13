#include "mainwindow.h"
#include <QKeyEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>

#include "../PieceTable.h"

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

    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    menuBar->setStyleSheet(
    "QMenuBar { background-color: #f0f0f0; color: #202020; border-bottom: 1px solid #ccc; }"
    "QMenuBar::item:selected { background-color: #dcdcdc; }"
    "QMenu { background-color: white; color: black; }"
    );


    QMenu *fileMenu = menuBar->addMenu("File");

    QAction *openAction = new QAction("Open", this);
    QAction *saveAction = new QAction("Save", this);

    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);

}

MainWindow::~MainWindow() {}

void MainWindow::paintEvent(QPaintEvent *event) {

    // draws visible text and cursor

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

        QString visiblePart;
        if (hScrollOffset < line.length()) {
            visiblePart = line.mid(hScrollOffset, visibleCols);
        }

        int topMargin = menuBar()->height() + cellHeight; 
        painter.drawText(0, topMargin + (i - scrollOffset) * cellHeight, visiblePart);


    }

    drawCursor(painter);
}


void MainWindow::drawCursor(QPainter &painter) {

    // draws blinking cursor block

    if (!cursorVisible) return;

    QStringList lines = cachedText.split('\n');
    if (cursorRow < 0 || cursorRow >= lines.size()) return;

    QString line = lines[cursorRow];
    QFontMetrics metrics(font());

    int x = metrics.horizontalAdvance(line.left(cursorCol - hScrollOffset));
    int y = menuBar()->height() + cellHeight + (cursorRow - scrollOffset) * cellHeight;


    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawRect(x, y - cellHeight, 2, cellHeight);
}



void MainWindow::toggleCursorVisibility() {

    // toggles cursor blink state

    cursorVisible = !cursorVisible;
    update();
}

void MainWindow::finalizeCursorMove() {

    // finalizes ongoing piece-table edit state

    if (P.state == 2) { P.weightUpdator2(P.head, P.currIndex); P.delCount = 0; P.current_piece = NULL; }
    if (P.state == 1) { P.weightUpdator(P.head, P.currIndex);  P.current_piece = NULL; }
    if (P.lastStepLength > 0) P.undo.push(new laststep(P.undoType,  P.lastStepLength, P.cursorStart, P.charStack));
    P.lastStepLength = 0;
    P.state   = 0;
    P.GlobalIndex = currCursor;
}

void MainWindow::updateCursorPosition() {

    // updates cursor row/col from global index

    cursorRow = cursorCol = 0;
    QStringList lines = cachedText.split('\n');
    int index = currCursor;

    for (int i = 0; i < lines.size(); ++i) {
        if (index <= lines[i].length()) { cursorRow = i; cursorCol = index; return; }
        index -= lines[i].length() + 1;
    }
}

int MainWindow::computeCursorIndexFromMouse(int x, int y) {

    // maps mouse click position to text index

    int topMargin = menuBar()->height();
    int row = (y - topMargin) / cellHeight + scrollOffset;

    QStringList lines = cachedText.split('\n');
    if (row < 0) row = 0;
    if (row >= lines.size()) row = lines.size() - 1;

    QFontMetrics metrics(font());
    int col = (x + metrics.horizontalAdvance('M') / 2) / cellWidth;  // ← small +½ cell correction

    col = std::min(col, (int)lines[row].length());

    int index = 0;
    for (int i = 0; i < row; ++i)
        index += lines[i].length() + 1;
    return index + col;
}



void MainWindow::mousePressEvent(QMouseEvent *event) {

    // handles cursor movement on mouse click

    finalizeCursorMove();
    currCursor = computeCursorIndexFromMouse(event->position().x(), event->position().y());
    P.GlobalIndex = currCursor;
    updateCursorPosition();
    scrollToCursor();
    update();
}

void MainWindow::scrollToCursor() {

    // auto-scrolls viewport to keep cursor visible

    visibleLines = height() / cellHeight;
    visibleCols  = width()  / cellWidth;

    // vertical auto-scroll
    if (cursorRow < scrollOffset)
        scrollOffset = cursorRow;
    if (cursorRow >= scrollOffset + visibleLines)
        scrollOffset = cursorRow - visibleLines + 1;

    // horizontal auto-scroll
    if (cursorCol < hScrollOffset)
        hScrollOffset = cursorCol;
    if (cursorCol >= hScrollOffset + visibleCols)
        hScrollOffset = cursorCol - visibleCols + 1;

    if (scrollOffset < 0) scrollOffset = 0;
    if (hScrollOffset < 0) hScrollOffset = 0;

    update();
}


void MainWindow::wheelEvent(QWheelEvent *event) {

    // vertical scroll using mouse wheel

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
    // handles typing, navigation, undo/redo

    if (event->key() == Qt::Key_Left) {
        if (currCursor > 0){ currCursor--;P.GlobalIndex--;}
        finalizeCursorMove();
        updateCursorPosition(); scrollToCursor(); update(); return;
    }
    if (event->key() == Qt::Key_Right) {
        if (currCursor < cachedText.length()) {currCursor++;P.GlobalIndex--;}
        finalizeCursorMove();
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

    // --- undo/redo ---
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->key() == Qt::Key_Z) performUndo(P, currCursor);
        else if (event->key() == Qt::Key_Y) performRedo(P, currCursor);
        cachedText = QString::fromStdString(P.printTrial(P.head));
        updateCursorPosition(); scrollToCursor(); update(); return;
    }

    // --- enter key ---
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        insertChar(P, '\n', currCursor);
        cachedText = QString::fromStdString(P.printTrial(P.head));
        updateCursorPosition(); scrollToCursor(); update(); return;
    }

    // --- backspace ---
    if (event->key() == Qt::Key_Backspace) {
        if (currCursor > 0) { if (P.state != 2) P.delCount = 0; P.delCount++; P.deletion(currCursor, 0); currCursor--; }
        cachedText = QString::fromStdString(P.printTrial(P.head));
        updateCursorPosition(); scrollToCursor(); update(); return;
    }

    // --- printable characters ---
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

void MainWindow::openFile() {

    // loads file into editor and rebuilds piece table

    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open file.");
        return;
    }

    QTextStream in(&file);
    cachedText = in.readAll();
    file.close();

    std::string content = cachedText.toStdString();
    P.originalString = content;
    P.addString.clear();
    P.undo = std::stack<laststep*>();
    P.redo = std::stack<laststep*>();
    P.state = 0;
    P.GlobalIndex = content.length();
    P.currIndex = P.GlobalIndex;
    P.delCount = 0;
    P.lastStepLength = 0;
    P.current_piece = nullptr;

    if (!content.empty()) {
        pieceNode* root = new pieceNode(ORIGINAL, 0, content.length());
        root->weight = 0;
        root->height = 1;
        root->left = root->right = nullptr;
        P.head = root;
    } else {
        P.head = nullptr;
    }

    currCursor = content.length();
    updateCursorPosition();
    update();
}


void MainWindow::saveFile() {

    // saves current text buffer to file
    
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot save file.");
        return;
    }

    QTextStream out(&file);
    out << cachedText;
    file.close();

    QMessageBox::information(this, "Saved", "File saved successfully!");
}
