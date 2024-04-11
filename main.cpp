#include "mainwindow.h"

#include <QApplication>
#include<QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //qss文件加载一般在main函数中
    QFile qss(":/style/style.qss");
    if(qss.open(QFile::ReadOnly)){

        QString style = QLatin1String(qss.readAll()); //QLatin1String将读取的字面值常量转换为QString能减少开销
        a.setStyleSheet(style);
    }else{
        qDebug()<<"Open qss failed";
        return 1;
    }
    MainWindow w;
    w.setWindowTitle("ALbum");
    w.showMaximized();
    return a.exec();
}
