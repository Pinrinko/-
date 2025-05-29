#include "FileSystem.h"

FileSystem::FileSystem() : afd(-1) {}

int FileSystem::getFileIndex(const QString& name) const {
    for (int i = 0; i < directory.size(); ++i) {
        if (directory[i].name == name)
            return i;
    }
    return -1;
}

QString FileSystem::getActiveFileName() const {
    if (afd >= 0 && afd < directory.size())
        return directory[afd].name;
    return QString();
}

QString FileSystem::createFile(const QString& name) {
    if (directory.size() >= 100)
        return "文件数已达上限！";
    if (getFileIndex(name) != -1)
        return "文件已存在！";
    File f;
    f.name = name;
    f.content = "";
    f.is_open = false;
    f.permission = 1;
    directory.append(f);
    return "文件创建成功！";
}

QString FileSystem::openFile(const QString& name) {
    int idx = getFileIndex(name);
    if (idx == -1)
        return "文件不存在！";
    if (directory[idx].is_open)
        return "文件已打开。";
    directory[idx].is_open = true;
    afd = idx;
    return "文件已打开。";
}

QString FileSystem::closeFile() {
    if (afd == -1)
        return "无活动文件！";
    directory[afd].is_open = false;
    QString msg = QString("文件 %1 已关闭。").arg(directory[afd].name);
    afd = -1;
    return msg;
}

QString FileSystem::deleteFile(const QString& name) {
    int idx = getFileIndex(name);
    if (idx == -1)
        return "文件不存在！";
    if (directory[idx].is_open)
        return "请先关闭文件。";
    directory.remove(idx);
    if (afd == idx) afd = -1;
    return "文件已删除。";
}

QString FileSystem::readFile() {
    if (afd == -1)
        return "无活动文件！请先打开文件。";
    return QString("文件内容：%1").arg(directory[afd].content);
}

QString FileSystem::writeFile(const QString& content) {
    if (afd == -1)
        return "无活动文件！请先打开文件。";
    if (directory[afd].permission == 0)
        return "权限不足，文件为只读！";
    directory[afd].content = content;
    return "写入成功。";
}

QString FileSystem::changePermission() {
    if (afd == -1)
        return "无活动文件！";
    directory[afd].permission = 1 - directory[afd].permission;
    return QString("权限已切换为：%1").arg(directory[afd].permission ? "可读写" : "只读");
}

QString FileSystem::renameFile(const QString& oldName, const QString& newName) {
    int idx = getFileIndex(oldName);
    if (idx == -1)
        return "文件不存在！";
    if (getFileIndex(newName) != -1)
        return "目标文件名已存在！";
    directory[idx].name = newName;
    return "文件重命名成功。";
}

QString FileSystem::listFiles() const {
    if (directory.isEmpty())
        return "当前目录为空。";
    QString res = "文件列表：\n";
    for (const auto& f : directory) {
        res += QString("%1 [%2] %3\n")
            .arg(f.name)
            .arg(f.permission ? "读写" : "只读")
            .arg(f.is_open ? "(打开)" : "");
    }
    return res;
}