#pragma once
#pragma once
#include <QString>
#include <QVector>

enum class NodeType { Directory, File };

struct FileNode {
    QString name;
    NodeType type;
    FileNode* parent;
    QVector<FileNode*> children;

    FileNode(const QString& name, NodeType type, FileNode* parent = nullptr)
        : name(name), type(type), parent(parent) {
    }
    ~FileNode();
};

class FileTree {
public:
    FileTree();
    ~FileTree();

    QString mkdir(const QString& name);
    QString touch(const QString& name);
    QString ls() const;
    QString cd(const QString& name);
    QString rm(const QString& name);
    QString tree() const;
    QString pwd() const;

    FileNode* getRoot() const { return root; }
    FileNode* getCurrent() const { return current; }
    void reset(); // 重新初始化

private:
    FileNode* root;
    FileNode* current;

    int findChild(const QString& name) const;
    void buildSchool(FileNode* root);
    void buildTreeString(FileNode* node, int level, QString& out) const;
};