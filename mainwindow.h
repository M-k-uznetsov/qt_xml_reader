#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeView>
#include <QAction>
#include <QSortFilterProxyModel>
#include "parser.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void active();
    void customActiveMenuRequested(QPoint pos);
    void open();
    void closeAll();
private:
    Parser*windowModel;
    Parser* model;
    QTreeView* tree;
    QTreeView* widget;
};

#endif // MAINWINDOW_H
