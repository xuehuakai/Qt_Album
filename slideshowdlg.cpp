#include "slideshowdlg.h"
#include "protreewidget.h"
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

     auto * prelistwid = dynamic_cast<PreListWid*>(ui->preListWidget);
    connect(ui->picAnimation,&PicAnimationWid::SigUpPreList,
            prelistwid,&PreListWid::SlotUpPreList);
    connect(ui->picAnimation,&PicAnimationWid::SigSelectItem,
             prelistwid,&PreListWid::SlotUpSelect);


    connect(ui->slidenextBtn,&QPushButton::clicked,
            this,&SlideShowDlg::SlotSlideNext);
    connect(ui->slidepreBtn,&QPushButton::clicked,
            this,&SlideShowDlg::SlotSlidePre);
    connect(prelistwid,&PreListWid::SigUpSelect,
            ui->picAnimation,&PicAnimationWid::SlotUpSelect);

    connect(ui->closeBtn,&QPushButton::clicked,
            this,&SlideShowDlg::close);
    connect(ui->playBtn,&PicStateBtn::clicked,ui->picAnimation,
            &PicAnimationWid::SlotStartOrStop);

    connect(ui->picAnimation,&PicAnimationWid::SigStart,
           ui->playBtn,&PicStateBtn::SlotStart);
    connect(ui->picAnimation,&PicAnimationWid::SigStop,
           ui->playBtn,&PicStateBtn::SlotStop);

    auto * _protree_widget = dynamic_cast<ProTreeWidget*>(parent);



    connect(ui->picAnimation,&PicAnimationWid::SigStartMusic,
            _protree_widget,&ProTreeWidget::SlotStartMusic);

    connect(ui->picAnimation,&PicAnimationWid::SigStopMusic,
            _protree_widget,&ProTreeWidget::SlotStopMusic);
    ui->picAnimation->SetPixmap(_first_item);

    ui->picAnimation->Start();
}

SlideShowDlg::~SlideShowDlg()
{
    delete ui;
}

void SlideShowDlg::SlotSlideNext()
{
    ui->picAnimation->SlideNext();
}

void SlideShowDlg::SlotSlidePre()
{
    ui->picAnimation->SlidePre();
}
