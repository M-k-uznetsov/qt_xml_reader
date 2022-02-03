#include "parser.h"

Parser::Parser(QObject* parent) : QAbstractItemModel(parent), root(new TreeNode({tr("")})), xmlReader(new QXmlStreamReader()), focused(), focusedFont() {}

Parser::~Parser() {
    delete root;
    delete xmlReader;
}

QVariant Parser::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (role == Qt::FontRole && index == focused) {
        return focusedFont;
    }
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());
    return item->getData().at(index.column());
}

bool Parser::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) {
        return false;
    }
    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());
    if (role == Qt::EditRole) {
        item->getData()[index.column()] = value;
        return true;
    }
    if (role == Qt::FontRole) {
        focused = index;
        focusedFont = value.value<QFont>();
        return true;
    }
    return false;
}

Qt::ItemFlags Parser::flags(const QModelIndex& index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return QAbstractItemModel::flags(index);
}

QVariant Parser::headerData(int sect, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return root->getData().at(sect);
    }

    return QVariant();
}

QModelIndex Parser::index(int row, int column, const QModelIndex& parent) const {
    TreeNode* parentItem;
    if (!parent.isValid()) {
        parentItem = root;
    } else {
        parentItem = static_cast<TreeNode*>(parent.internalPointer());
    }

    TreeNode *childItem;

    if (row < 0 || row >= parentItem->getChild().size()) {
        childItem = nullptr;
    } else {
        childItem = parentItem->getChild().at(row);
    }

    if (childItem) {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QModelIndex Parser::parent(const QModelIndex& child) const {
    TreeNode *childItem = (TreeNode*)(child.internalPointer());
    TreeNode *parent = childItem->getParent();
    if (parent == root || childItem == root) {
        return QModelIndex();
    }

    return createIndex(parent->childParent(), 0, parent);
}


int Parser::rowCount(const QModelIndex& parent) const {
    TreeNode *parentItem;
    if (parent.column() > 0) {
        return 0;
    }
    if (!parent.isValid()) {
        parentItem = root;
    } else {
        parentItem = (TreeNode*)(parent.internalPointer());
    }
    return parentItem->getChild().size();
}

int Parser::columnCount(const QModelIndex& parent) const {
    return root->getData().size();
}

bool Parser::removeRows(int row, int count, const QModelIndex &parent) {
    TreeNode* node;
    QModelIndex correctParent;
    if (parent.isValid()) {
        node = (TreeNode*)(parent.internalPointer());
        correctParent = parent;
    } else {
        node = root;
        correctParent = QModelIndex();
    }
    beginRemoveRows(correctParent, row, row + count - 1);
    node->dropRows(row, row + count);
    endRemoveRows();

    return true;
}

void Parser::getFile(const QString& filePath) {
    QFile file(filePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QStack<int> count;
    QStack<int> lay;

    xmlReader->setDevice(&file);
    QString fileName = filePath.section("/", -1, -1);
    auto fileNode = new TreeNode{{fileName}, root};
    root->setChild(fileNode);
    TreeNode* currentNode = fileNode;

    int currentLayer = 0;
    while (!xmlReader->atEnd() && !xmlReader->hasError()) {
        QXmlStreamReader::TokenType token = xmlReader->readNext();
        if (token == QXmlStreamReader::StartElement) {
            ++currentLayer;
            auto text = xmlReader->name().toString();
                if (lay.isEmpty() || currentLayer != lay.top()) {
                    count.append(0);
                    lay.append(currentLayer);
                }
            auto node = new TreeNode({text}, currentNode);
            currentNode->setChild(node);
            currentNode = node;
        }
        if (token == QXmlStreamReader::Characters) {
            auto node = new TreeNode({xmlReader->text().toString()}, currentNode);
            currentNode->setChild(node);
        }
        if (token == QXmlStreamReader::EndElement) {
            --currentLayer;
            currentNode = currentNode->getParent();
            auto text = xmlReader->name().toString();
            if (text == "array" && currentLayer + 1 != lay.top()) {
                count.pop();
                lay.pop();
            }
        }
    }

    if (xmlReader->hasError()) {
        QMessageBox message(QMessageBox::Critical,
                            tr("Parsing error"),
                            tr("Parsing error"),
                            QMessageBox::Ok);
        message.exec();
        root->popBack();
    }
}

void Parser::modelRemoveRows() {
    removeRows(0, root->getChild().size(), createIndex(-1, -1, nullptr));
}

void Parser::getBranch(TreeNode* root, TreeNode* base){

    for (auto baseChild :base->getChild() ) {
        TreeNode* currentChild = new TreeNode(baseChild->getData());
        currentChild->setParent(root);
        (root)->setChild(currentChild);
        getBranch(currentChild,baseChild);
    }

}
TreeNode* Parser:: getRoot(){
    return root;
}
