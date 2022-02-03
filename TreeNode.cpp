#include<TreeNode.h>
TreeNode::TreeNode(const QVector<QVariant> &data, TreeNode *parent) : data(data), parent(parent) {}

TreeNode::~TreeNode() {
    for (auto node : child) {
        parent = nullptr;
        delete node;
    }
}

QVector<TreeNode *> TreeNode::getChild() {
    return child;
}

QVector<QVariant> TreeNode::getData() {
    return data;
}

void TreeNode::setParent(TreeNode *p) {
    parent = p;
}

TreeNode *TreeNode::getParent() {
    return parent;
}

void TreeNode::setChild(TreeNode *fileNode) {
    child.append(fileNode);
}

void TreeNode::popBack() {
    child.pop_back();
}

int TreeNode::childParent() {
    if (!parent) {
        return 0;
    }
    return parent->child.indexOf(this);
}

void TreeNode::dropRows(int begin, int end) {
    for (int i = begin; i < end; ++i) {
        delete child[begin];
        child.remove(begin);
    }
}
