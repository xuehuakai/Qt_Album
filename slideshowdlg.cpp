#include "slideshowdlg.h"
#include "ui_slideshowdlg.h"

SlideShowDlg::SlideShowDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SlideShowDlg)
{
    ui->setupUi(this);
}

SlideShowDlg::SlideShowDlg(QWidget *parent, QTreeWidgetItem *first_item, QTreeWidgetItem *last_item)
{

}

SlideShowDlg::~SlideShowDlg()
{
    delete ui;
}
