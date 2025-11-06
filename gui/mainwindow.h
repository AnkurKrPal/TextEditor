#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFontMetrics>

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

private:
    bool cursorVisible;
    QTimer *cursorTimer;

    int cursorRow;
    int cursorCol;

    int cellWidth;
    int cellHeight;

    QString cachedText;

    // Vertical scrolling
    int scrollOffset = 0;  
    int visibleLines = 0;  

    // ✅ Horizontal scrolling
    int hScrollOffset = 0;
    int visibleCols = 0;

    void drawCursor(QPainter &painter);
    void updateCursorPosition();
    int  computeCursorIndexFromMouse(int x, int y);

    void finalizeCursorMove();
    void scrollToCursor();   // ✅ Will now handle both directions
};

#endif
