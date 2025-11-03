#include "mainwindow.h"
#include <QTextEdit>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    editor = new QTextEdit(this);
    setCentralWidget(editor);
    resize(800, 600);
    setWindowTitle("Qt Text Editor");

    QMenu *fileMenu = menuBar()->addMenu("&File");

    QAction *openAction = new QAction("Open", this);
    QAction *saveAction = new QAction("Save", this);
    QAction *exitAction = new QAction("Exit", this);

    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    connect(openAction, &QAction::triggered, this, [this]() {
        QString path = QFileDialog::getOpenFileName(this, "Open File");
        if (!path.isEmpty()) {
            QFile file(path);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                editor->setPlainText(in.readAll());
                file.close();
            }
        }
    });

    connect(saveAction, &QAction::triggered, this, [this]() {
        QString path = QFileDialog::getSaveFileName(this, "Save File");
        if (!path.isEmpty()) {
            QFile file(path);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << editor->toPlainText();
                file.close();
            }
        }
    });

    connect(exitAction, &QAction::triggered, this, &MainWindow::close);
}

MainWindow::~MainWindow() {}
