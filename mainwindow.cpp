#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    auto file = new QMenu(tr("File"));
    auto open = new QAction(tr("Open"), this);
    auto closeAll = new QAction(tr("Close all"), this);
    auto exit = new QAction(tr("Exit"), this);

    menuBar()->addMenu(file);
    file->addAction(open);
    file->addAction(closeAll);
    file->addAction(exit);

    connect(open, SIGNAL(triggered()), this, SLOT(open()));
    connect(closeAll, SIGNAL(triggered()), this, SLOT(closeAll()));
    connect(exit, SIGNAL(triggered()), this, SLOT(close()));
    windowModel = new Parser{};
    widget = new QTreeView{};
    model = new Parser{};
    tree = new QTreeView{};
    tree->setModel(model);

    tree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tree, SIGNAL(customContextMenuRequested(QPoint)), SLOT(customActiveMenuRequested(QPoint)));
    setCentralWidget(tree);
}

MainWindow::~MainWindow() {
    delete windowModel;
    delete model;
    delete tree;
    delete widget;
}

void MainWindow::active() {
    auto currentIndex = tree->currentIndex();
    QFont bold;
    bold.setBold(true);
    model->setData(currentIndex, bold, Qt::FontRole);
}

void MainWindow::customActiveMenuRequested(QPoint pos) {
    if (tree->currentIndex().parent() == QModelIndex() && tree->indexAt(pos).isValid()) {
            QMenu *menu = new QMenu(this);
            auto active = new QAction(tr("Active"), this);
            auto closeFile = new QAction(tr("open in new window"), this);
            menu->addAction(active);
            menu->addAction(closeFile);
            int i = 0;
            for (; model->index(i, 0) != tree->currentIndex(); ++i) {}
            connect(active, &QAction::triggered, this, &MainWindow::active);
            connect(closeFile, &QAction::triggered, this, [this, pos, i](){
                widget = new QTreeView;
                windowModel = new Parser;

                TreeNode* root = new TreeNode(model->getRoot()->getChild()[i]->getData());
                windowModel->getRoot()->setChild(root);
                root->setParent(windowModel->getRoot());
                windowModel->getBranch(root,model->getRoot()->getChild()[i]);

                widget->setModel(windowModel);
                widget->show();
                model->removeRow(i, tree->indexAt(pos).parent());
            });
            menu->popup(tree->viewport()->mapToGlobal(pos));
        }

}

void MainWindow::open() {
    QString file = QFileDialog::getOpenFileName(this, tr("Choose xml file"), QDir::homePath(), tr("*.xml"));
    model->getFile(file);
    tree->reset();
    QFont font;
    font.setBold(true);
    model->setData(model->index(0, 0), font, Qt::FontRole);
}

void MainWindow::closeAll() {
    model->modelRemoveRows();
    tree->reset();
}
