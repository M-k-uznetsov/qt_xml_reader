#ifndef TREENODE_H
#define TREENODE_H

#include <QAbstractItemModel>
#include <QFile>
#include <QXmlStreamReader>
#include <QMessageBox>
#include <QModelIndex>
#include <QStack>
#include <QPersistentModelIndex>
#include <QErrorMessage>
#include <QVector>

class TreeNode {
    TreeNode* parent;
    QVector<TreeNode*> child;
    QVector<QVariant> data;
public:
    explicit TreeNode(const QVector<QVariant>& data, TreeNode* parentItem = nullptr);
    ~TreeNode();
    void setChild(TreeNode* fileNode);
    void popBack();
    int childParent();
    void dropRows(int begin, int end);
    QVector<TreeNode*> getChild();
    QVector<QVariant> getData();
    void setParent(TreeNode *);
    TreeNode* getParent();

};

#endif // TREENODE_H
