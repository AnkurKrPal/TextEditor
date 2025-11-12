#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFontMetrics>
#include <QFileDialog>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private slots:
    void toggleCursorVisibility();

private slots:
    void openFile();
    void saveFile();

private:
    bool cursorVisible;
    QTimer *cursorTimer;

    int cursorRow;
    int cursorCol;

    int cellWidth;
    int cellHeight;

    QString cachedText;

    int scrollOffset = 0;  
    int visibleLines = 0;  

    int hScrollOffset = 0;
    int visibleCols = 0;

    void drawCursor(QPainter &painter);
    void updateCursorPosition();
    int  computeCursorIndexFromMouse(int x, int y);

    void finalizeCursorMove();
    void scrollToCursor();
};

#endif
