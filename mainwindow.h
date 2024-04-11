#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWidget* _protree; //为什么不用ProTree 防止互引用
    QWidget* _picshow; //图片展示（用基类来编程的话会好一些）
private slots:
    void SlotCreatePro(bool);
    void SlotOpenPro(bool);
signals:
    void SigOpenPro(const  QString & path);
};
#endif // MAINWINDOW_H
