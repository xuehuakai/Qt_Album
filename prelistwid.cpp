#include "prelistwid.h"
#include "const.h"
#include "prelistitem.h"
#include "protreeitem.h"
#include "qapplication.h"
#include "qpainter.h"
#include "qtreewidget.h"


PreListWid::PreListWid(QWidget *parent):QListWidget(parent),_global(0),last_index(17){
    //为了list展示 在构造函数中设置展示模式
    this->setViewMode(QListWidget::IconMode); //只展示icon
    this->setIconSize(QSize(PREICON_SIZE,PREICON_SIZE));
    this->setSpacing(5); //item间隔
    connect(this,&PreListWid::itemPressed,this,&PreListWid::SlotItemPressed);
}

PreListWid::~PreListWid()
{

}

void PreListWid::SlotUpPreList(QTreeWidgetItem *tree_item)
{
    if(!tree_item)
    {
        qDebug()<<"tree_item is empty in "<<__FUNCTION__; //防御式编程  进攻式：assert(!=nullptr)
        return;
    }

    auto * pro_item = dynamic_cast<ProTreeItem*>(tree_item);
    auto path = pro_item->GetPath();
    auto iter = _set_items.find(path);
    //判断路径是否在集合中
    if(iter != _set_items.end()){
        qDebug()<<"路径已经存在了";
        return;
    }

    AddListItem(path);



}

void PreListWid::SlotUpSelect(QTreeWidgetItem *tree_item)
{
    if(!tree_item) return;

    auto * pro_item = dynamic_cast<ProTreeItem*>(tree_item);
    auto path = pro_item->GetPath();
    auto iter = _set_items.find(path);
    if(iter == _set_items.end()){
        return;
    }

    auto * list_item = dynamic_cast<PreListItem*>(iter.value());
    auto index  = list_item->GetIndex();
    if(index>17){ //翻页
        auto pos_cur = this->pos();//预览窗口的位置
        this->move(pos_cur.x() - (index - last_index)*100,pos_cur.y());
        qDebug()<<"此时 预览图位置："<<this->pos();

    }else{
        this->move(_pos_origin);//移动到原始位置，就是不动
        last_index = 17;
    }

    this->setCurrentItem(iter.value());
}

void PreListWid::SlotItemPressed(QListWidgetItem *item)
{
    if(QGuiApplication::mouseButtons() != Qt::LeftButton){
        return;
    }

    auto * list_item = dynamic_cast<PreListItem*>(item);
    auto cur_index = list_item->GetIndex();
    auto path = list_item->GetPath();

    this->setCurrentItem(item);
    emit SigUpSelect(path);
}



void PreListWid::AddListItem(const QString &path)
{
    QPixmap src_pixmap(path);// 一边画pixmap 一边加入map中
    src_pixmap = src_pixmap.scaled(PREICON_SIZE,PREICON_SIZE,Qt::KeepAspectRatio); //拉伸

    //目的图片
    QPixmap dst_pixmap(QSize(PREICON_SIZE,PREICON_SIZE));

    dst_pixmap.fill(QColor(220,220,220,50)); //参数4--透明度
    QPainter painter(&dst_pixmap);
    auto src_width = src_pixmap.width();
    auto src_height = src_pixmap.height();

    auto dst_width = dst_pixmap.width();
    auto dst_height = dst_pixmap.height();

    auto x = (dst_width-src_width)/2;
    auto y = (dst_height-src_height)/2; //居中点

    painter.drawPixmap(x,y,src_pixmap); //在目的pix上画原pix
    _global++;

    PreListItem * pItem = new PreListItem(QIcon(dst_pixmap),path,_global,this);
    pItem->setSizeHint(QSize(PREICON_SIZE,PREICON_SIZE));
    this->addItem(pItem);
    _set_items[path] = pItem;
    if(_global == 1){ //第一个pix,存一下
        _pos_origin = this->pos();
    }
}
