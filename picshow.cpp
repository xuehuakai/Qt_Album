#include "picshow.h"
#include "ui_picshow.h"

PicShow::PicShow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PicShow)
{
    ui->setupUi(this);
}

PicShow::~PicShow()
{
    delete ui;
}
