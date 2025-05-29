#include "FileTree.h"

FileNode::~FileNode() {
    for (auto child : children) delete child;
}

FileTree::FileTree() {
    root = new FileNode("学校", NodeType::Directory);
    buildSchool(root);
    current = root;
}

FileTree::~FileTree() { delete root; }

void FileTree::reset() {
    delete root;
    root = new FileNode("学校", NodeType::Directory);
    buildSchool(root);
    current = root;
}

void FileTree::buildSchool(FileNode* root) {
    // 教务处
    auto jiaowuchu = new FileNode("教务处", NodeType::Directory, root);
    root->children.append(jiaowuchu);
    jiaowuchu->children.append(new FileNode("教务科", NodeType::Directory, jiaowuchu));
    jiaowuchu->children.append(new FileNode("学籍管理", NodeType::Directory, jiaowuchu));
    // 学院
    auto xueyuan = new FileNode("学院", NodeType::Directory, root);
    root->children.append(xueyuan);
    auto jisuanji = new FileNode("计算机系", NodeType::Directory, xueyuan);
    xueyuan->children.append(jisuanji);
    jisuanji->children.append(new FileNode("教师名单.txt", NodeType::File, jisuanji));
    jisuanji->children.append(new FileNode("课程安排.xlsx", NodeType::File, jisuanji));
    xueyuan->children.append(new FileNode("数学系", NodeType::Directory, xueyuan));
    // 后勤处
    root->children.append(new FileNode("后勤处", NodeType::Directory, root));
}

int FileTree::findChild(const QString& name) const {
    for (int i = 0; i < current->children.size(); ++i) {
        if (current->children[i]->name == name) return i;
    }
    return -1;
}

QString FileTree::mkdir(const QString& name) {
    if (findChild(name) != -1) return "目录或文件已存在: " + name;
    auto node = new FileNode(name, NodeType::Directory, current);
    current->children.append(node);
    return "目录创建成功: " + name;
}

QString FileTree::touch(const QString& name) {
    if (findChild(name) != -1) return "目录或文件已存在: " + name;
    auto node = new FileNode(name, NodeType::File, current);
    current->children.append(node);
    return "文件创建成功: " + name;
}

QString FileTree::ls() const {
    if (current->children.isEmpty()) return "(空目录)";
    QString res;
    for (auto child : current->children)
        res += child->name + (child->type == NodeType::Directory ? "/" : "") + "\n";
    return res.trimmed();
}

QString FileTree::cd(const QString& name) {
    if (name == "..") {
        if (current->parent) {
            current = current->parent;
            return "已返回上级目录";
        }
        return "已在根目录";
    }
    int idx = findChild(name);
    if (idx == -1) return "目录不存在: " + name;
    auto target = current->children[idx];
    if (target->type != NodeType::Directory) return name + " 不是目录";
    current = target;
    return "进入目录: " + name;
}

QString FileTree::rm(const QString& name) {
    int idx = findChild(name);
    if (idx == -1) return "未找到文件或目录: " + name;
    auto target = current->children[idx];
    if (target->type == NodeType::Directory && !target->children.isEmpty())
        return "目录非空，无法删除: " + name;
    delete target;
    current->children.remove(idx);
    return "已删除: " + name;
}

void FileTree::buildTreeString(FileNode* node, int level, QString& out) const {
    out += QString(level * 2, ' ') + node->name + (node->type == NodeType::Directory ? "/" : "") + "\n";
    for (auto child : node->children)
        buildTreeString(child, level + 1, out);
}

QString FileTree::tree() const {
    QString out;
    buildTreeString(current, 0, out);
    return out.trimmed();
}

QString FileTree::pwd() const {
    QStringList path;
    FileNode* p = current;
    while (p) {
        path.prepend(p->name);
        p = p->parent;
    }
    return "/" + path.join("/");
}