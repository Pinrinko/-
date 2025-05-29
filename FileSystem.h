#pragma once
#pragma once
#include <QString>
#include <QVector>

struct File {
    QString name;
    QString content;
    bool is_open;
    int permission; // 0:只读, 1:可读写
};

class FileSystem {
public:
    FileSystem();

    QString createFile(const QString& name);
    QString openFile(const QString& name);
    QString closeFile();
    QString deleteFile(const QString& name);
    QString readFile();
    QString writeFile(const QString& content);
    QString changePermission();
    QString renameFile(const QString& oldName, const QString& newName);
    QString listFiles() const;

    int getActiveFileIndex() const { return afd; }
    QString getActiveFileName() const;
    int getFileCount() const { return directory.size(); }
    int getFileIndex(const QString& name) const;

private:
    QVector<File> directory;
    int afd; // 当前活动文件索引
};