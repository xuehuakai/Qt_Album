#include "removeprodialog.h"
#include "ui_removeprodialog.h"
#include<QCheckBox>
RemoveProDialog::RemoveProDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RemoveProDialog)
{
    ui->setupUi(this);
}

RemoveProDialog::~RemoveProDialog()
{
    delete ui;
}

bool RemoveProDialog::IsRemoved()
{
    return ui->checkBox->isChecked();
}
