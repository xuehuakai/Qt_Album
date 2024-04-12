#ifndef PICSHOW_H
#define PICSHOW_H

#include <QDialog>
#include<QGraphicsOpacityEffect> //透明度
#include<QPropertyAnimation> //属性动画

namespace Ui {
class PicShow;
}

class PicShow : public QDialog
{
    Q_OBJECT

public:
    explicit PicShow(QWidget *parent = nullptr);
    ~PicShow();
protected:
    bool event(QEvent* e) override;
private:
    void ShowPreNextBtns(bool b_show);
    Ui::PicShow *ui;
    QPropertyAnimation * _animation_show_pre;
    QPropertyAnimation * _animation_show_next;
    bool _b_btnvisible;
};

#endif // PICSHOW_H
