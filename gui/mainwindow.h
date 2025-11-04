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

private slots:
    void toggleCursorVisibility();

private:
    bool cursorVisible;
    QTimer *cursorTimer;
    int cursorRow, cursorCol;

    int cellWidth;
    int cellHeight;

    void drawGrid(QPainter &painter);
    void drawCursor(QPainter &painter);
    void drawCenterText(QPainter &painter);   // <-- Added for center text
};

#endif // MAINWINDOW_H
