#include "MainWindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();  // 显示主窗
    return a.exec();
}
