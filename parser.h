#ifndef XMLMODEL_H
#define XMLMODEL_H

#include <QAbstractItemModel>
#include <QFile>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QModelIndex>
#include <QStack>
#include <QPersistentModelIndex>
#include <QErrorMessage>

#include<TreeNode.h>

class Parser : public QAbstractItemModel {
    Q_OBJECT
public:
    explicit Parser(QObject *parent = nullptr);
    ~Parser();
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& p_idx) const override;
    QVariant headerData(int sect, Qt::Orientation p_orient, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    void getFile(const QString& fileName);
    void getBranch(TreeNode* root, TreeNode* base);
    void modelRemoveRows();
    TreeNode* getRoot();
private:
    TreeNode* root;
    QXmlStreamReader* xmlReader;
    QPersistentModelIndex focused;
    QFont focusedFont;
};

#endif // XMLMODEL_H
