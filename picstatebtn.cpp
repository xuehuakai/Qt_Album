#include "picstatebtn.h"
#include"const.h"
#include<QEvent>
PicStateBtn::PicStateBtn(QWidget *parent):QPushButton(parent)
{

}

void PicStateBtn::SetIcons(const QString &normal, const QString &hover, const QString &pressed,
                           const QString &normal_2, const QString &hover_2, const QString &pressed_2)
{
    _normal = normal;
    _hover = hover;
    _pressed = pressed;
    _normal_2 = normal_2;
    _hover_2 = hover_2;
    _pressed_2 = pressed_2;

    QPixmap tmpPixmap;
    tmpPixmap.load(normal);
    this->resize(tmpPixmap.size());
    this->setIcon(tmpPixmap);
    this->setIconSize(tmpPixmap.size());
    _cur_state = PicBtnStateNormal;

}

bool PicStateBtn::event(QEvent *e)
{
    switch(e->type()){
    case QEvent::Enter:
        if(_cur_state < PicBtnState2Normal){ //小于暂停图标，说明是播放图标
            setHoverIcon();
        }else{
            setHover2Icon();
        }
        break;
    case QEvent::Leave:
        if(_cur_state < PicBtnState2Normal){ //小于暂停图标，说明是播放图标
            setNormalIcon();
        }else{
            setNormal2Icon();
        }
        break;
    case QEvent::MouseButtonPress:
        if(_cur_state < PicBtnState2Normal){ //小于暂停图标，说明是播放图标
            setPressIcon();
        }else{
            setPress2Icon();
        }
        break;
    case QEvent::MouseButtonRelease:
        if(_cur_state < PicBtnState2Normal){ //小于暂停图标，说明是播放图标
            setHover2Icon();
        }else{
            setHoverIcon();
        }
        break;
    default:
        break;
    }

    return QPushButton::event(e);
}

void PicStateBtn::setNormalIcon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(_normal);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnStateNormal;
}

void PicStateBtn::setHoverIcon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(_hover);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnStateHover;
}

void PicStateBtn::setPressIcon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(_pressed);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnStatePress;
}

void PicStateBtn::setNormal2Icon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(_normal_2);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnState2Normal;
}

void PicStateBtn::setHover2Icon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(_hover_2);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnState2Hover;
}

void PicStateBtn::setPress2Icon()
{
    QPixmap tmpPixmap;
    tmpPixmap.load(_pressed_2);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnState2Press;
}

void PicStateBtn::SlotStart()
{
    setNormal2Icon();
}

void PicStateBtn::SlotStop()
{
    setNormalIcon();
}
