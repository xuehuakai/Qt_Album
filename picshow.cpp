#include "picshow.h"
#include "ui_picshow.h"

PicShow::PicShow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PicShow)
{
    ui->setupUi(this);
    ui->nextBtn->SetIcons(":/icon/next.png",
                              ":/icon/next_hover.png",
                              ":/icon/next_press.png");
    ui->previousBtn->SetIcons(":/icon/previous.png",
                          ":/icon/previous_hover.png",
                          ":/icon/previous_press.png");

    QGraphicsOpacityEffect * opacity_pre = new QGraphicsOpacityEffect(this);
    opacity_pre->setOpacity(0);
    ui->previousBtn->setGraphicsEffect(opacity_pre);


    QGraphicsOpacityEffect * opacity_next = new QGraphicsOpacityEffect(this);
    opacity_next->setOpacity(0);
    ui->nextBtn->setGraphicsEffect(opacity_next);


    _animation_show_pre = new QPropertyAnimation(opacity_pre,"opacity",this);
    _animation_show_pre->setEasingCurve(QEasingCurve::Linear); //线性
    _animation_show_pre->setDuration(500); //500ms 动画时长

    _animation_show_next = new QPropertyAnimation(opacity_next,"opacity",this);
    _animation_show_next->setEasingCurve(QEasingCurve::Linear); //线性
    _animation_show_next->setDuration(500); //500ms 动画时长

}

PicShow::~PicShow()
{
    delete ui;
}

bool PicShow::event(QEvent *e)
{
    switch(e->type()){
    case QEvent::Enter:
        ShowPreNextBtns(true);
        break;
    case QEvent::Leave:
        ShowPreNextBtns(false);
        break;
    default:
        break;
    }
    return QDialog::event(e);
}

void PicShow::ShowPreNextBtns(bool b_show)
{
    if(!b_show && _b_btnvisible){ //隐藏
        _animation_show_pre->stop();
        _animation_show_pre->setStartValue(1);
        _animation_show_pre->setEndValue(0); //动画从显示到隐藏
        _animation_show_pre->start();

        _animation_show_next->stop();
        _animation_show_next->setStartValue(1);
        _animation_show_next->setEndValue(0); //动画从显示到隐藏
        _animation_show_next->start();

        _b_btnvisible=false;
        return;
    }

    //显示
    if(b_show && !_b_btnvisible){ //隐藏
        _animation_show_pre->stop();
        _animation_show_pre->setStartValue(0);
        _animation_show_pre->setEndValue(1); //动画从隐藏到显示
        _animation_show_pre->start();

        _animation_show_next->stop();
        _animation_show_next->setStartValue(0);
        _animation_show_next->setEndValue(1); //动画从隐藏到显示
        _animation_show_next->start();

        _b_btnvisible=true;
        return;
    }

}
