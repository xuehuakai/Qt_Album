#include "wizard.h"
#include "ui_wizard.h"
#include<QDebug>
Wizard::Wizard(QWidget *parent)
    : QWizard(parent)
    , ui(new Ui::Wizard)
{
    ui->setupUi(this);
}

Wizard::~Wizard()
{
    delete ui;
}

void Wizard::done(int result)
{
    if(result == QDialog::Rejected){
        return QWizard::done(result);
    }

    QString name,path;

    ui->wizardPage1->GetProSettings(name,path);

    qDebug()<<"【wizard.cpp[done]：name and path is 】"+name+"----"+path;

    emit SinProSettings(name,path); //发送给主界面，完成一个目录树

    QWizard::done(result); //使用基类的基本功能

}

