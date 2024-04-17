#include "slideshowdlg.h"
#include "ui_slideshowdlg.h"

SlideShowDlg::SlideShowDlg(QWidget *parent,QTreeWidgetItem* first_item,
                           QTreeWidgetItem* last_item)
    : QDialog(parent),ui(new Ui::SlideShowDlg),_first_item(first_item)
    , _last_item(last_item)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint); //干掉最大化最小化
    ui->slidepreBtn->SetIcons(":/icon/previous.png",
                              ":/icon/previous_hover.png",
                              ":/icon/previous_press.png");

    ui->slidenextBtn->SetIcons(":/icon/next.png",
                              ":/icon/next_hover.png",
                              ":/icon/next_press.png");

    ui->closeBtn->SetIcons(":/icon/closeshow.png",
                           ":/icon/closeshow_hover.png",
                           ":/icon/closeshow_press.png");

    ui->playBtn->SetIcons(":/icon/play.png",
                          ":/icon/play_hover.png",
                          ":/icon/play_press.png",
                          ":/icon/pause.png",
                          ":/icon/pause_hover.png",
                          ":/icon/pause_press.png");
}

SlideShowDlg::~SlideShowDlg()
{
    delete ui;
}
