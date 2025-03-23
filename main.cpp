#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 配置应用程序信息（必须在创建QSettings对象前设置）
    QCoreApplication::setOrganizationName("FuliTech");
    QCoreApplication::setApplicationName("FuliVideoMerger");

    MainWindow w;
    w.show();
    return a.exec();
}
