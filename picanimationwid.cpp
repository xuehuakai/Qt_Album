#include "picanimationwid.h"
#include "protreeitem.h"
#include<QTimer>
#include<QPainter>
PicAnimationWid::PicAnimationWid(QWidget *parent)
    : QWidget{parent},_factor(0.0),_cur_item(nullptr),_b_start(false)
{

    _timer = new QTimer(this);
    connect(_timer,&QTimer::timeout,this,&PicAnimationWid::TimeOut);
}

PicAnimationWid::~PicAnimationWid()
{

}

void PicAnimationWid::SetPixmap(QTreeWidgetItem *item)
{
    if(!item){
        return;
    }

    auto * tree_item = dynamic_cast<ProTreeItem*>(item);
    auto path = tree_item->GetPath();
    _pixmap1.load(path);
    _cur_item = tree_item;

    if(_map_item.find(path) == _map_item.end()){
        _map_item[path] = tree_item;
        //发送更新列表逻辑
        emit SigUpPreList(item); //先更新当前的Item 再更新后面的item 因为是双缓冲绘图
    }

    emit SigSelectItem(item);

    auto * next_item = tree_item->getNextItem();
    if(!next_item){
        return;
    }

    auto next_path = next_item->GetPath();
    _pixmap2.load(next_path);

    if(_map_item.find(next_path) == _map_item.end()){
        _map_item[next_path]=next_item;
        //发送更新列表逻辑
        emit SigUpPreList(next_item);
    }
}

void PicAnimationWid::Start()
{
    emit SigStart(); //通知slideshow界面
    emit SigStartMusic();
    _factor = 0;
    _timer->start(25);
    _b_start = true;
}

void PicAnimationWid::Stop()
{
    emit SigStop();
    emit SigStopMusic();
    _timer->stop();
    _factor=0;
    _b_start = false;
}

void PicAnimationWid::SlideNext()
{
    Stop();
    if(!_cur_item) return;
    auto * cur_pro_item = dynamic_cast<ProTreeItem*>(_cur_item);
    auto * next_item = cur_pro_item->getNextItem();
    if(!next_item) return;
    SetPixmap(next_item);
    update(); //刷新一下  图片才会显示出来
    Start();
}

void PicAnimationWid::SlidePre()
{
    Stop();
    if(!_cur_item) return;
    auto * cur_pro_item = dynamic_cast<ProTreeItem*>(_cur_item);
    auto * pre_item = cur_pro_item->GetPreItem();
    if(!pre_item) return;
    SetPixmap(pre_item);
    update(); //刷新一下  图片才会显示出来
    Start();
}

void PicAnimationWid::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if(_pixmap1.isNull()){
        return;
    }
    QPainter painter(this); //Painter绑定到Widget上面
    painter.setRenderHint(QPainter::Antialiasing,true); //防锯齿、

    //矩形区域
    QRect rect = geometry(); //绘制区域
    int w = rect.width();
    int h = rect.height();

    _pixmap1 = _pixmap1.scaled(w,h,Qt::KeepAspectRatio); //图片拉伸到矩形区域

    //渐渐隐藏的效果
    int alpha = 255*(1.0f - _factor); //剩余通道值  --黑色遮罩  离255越近越显示出来
    QPixmap alphaPixmap(_pixmap1.size());
    alphaPixmap.fill(Qt::transparent);//透明的图

    QPainter p1(&alphaPixmap);
    p1.setCompositionMode(QPainter::CompositionMode_Source); //组合模式 原图
    p1.drawPixmap(0,0,_pixmap1); //在p1上画另一张图
    p1.setCompositionMode(QPainter::CompositionMode_Destination);
    p1.fillRect(alphaPixmap.rect(),QColor(0,0,0,alpha)); //填充一个黑色遮罩
    //遮罩随着alpha的增加，透明度减少，最后遮罩显示出来   遮罩挡住图片，图片消失 --电影的一个效果
    p1.end();

    //做居中处理
    int x = (w-_pixmap1.width())/2;
    int y = (h-_pixmap1.height())/2;
    painter.drawPixmap(x,y,alphaPixmap);


    //图片2
    if(_pixmap2.isNull()) return;
    _pixmap2 = _pixmap2.scaled(w,h,Qt::KeepAspectRatio);
    //渐渐显示的效果
    alpha = 255*( _factor);
    QPixmap alphaPixmap2(_pixmap2.size());
    alphaPixmap2.fill(Qt::transparent); //设置透明

    //画真实的图片
    QPainter p2(&alphaPixmap2);

    //通过以下代码 补偿了遮罩 我们也许就看不到了  具体就需要了解图形学
    p2.setCompositionMode(QPainter::CompositionMode_Source); //涉及图形学
    p2.drawPixmap(0,0,_pixmap2); //将map2画到这个透明的图片上
    p2.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    //已经将真实的图片画出来了
    //透明度渐渐显示 --利用alpha因子
    p2.fillRect(alphaPixmap2.rect(),QColor(0,0,0,alpha));
    p2.end();
    //pixmap2是一个渐渐显示的效果，因为alpha是越来越大  pixmap1的alpha是越来越小 渐渐消失
    x = (w-_pixmap2.width())/2;
    y = (h-_pixmap2.height()) /2;
    painter.drawPixmap(x,y,alphaPixmap2);
}

void PicAnimationWid::UpSelectPixmap(QTreeWidgetItem *item) //更新缩略图选择的图片
{
    // if(!item) return;
    // auto * tree_item = dynamic_cast<ProTreeItem*>(item);
    // auto path = tree_item->GetPath();
    // _pixmap1.load(path);
    // _cur_item = tree_item;

    // if(_map_item.find(path) == _map_item.end()){
    //     _map_item[path] = tree_item;
    // }
    // auto * next_item = tree_item->getNextItem();

    // if(!next_item) return;

    // auto next_path = tree_item->GetPath();
    // _pixmap2.load(next_path);

    // if(_map_item.find(next_path) == _map_item.end()){
    //     _map_item[next_path] = next_item;
    // }
    SetPixmap(item);
}

void PicAnimationWid::SlotUpSelect(const QString &path)
{
    auto iter = _map_item.find(path);
    if(iter == _map_item.end()){
        return;
    }
    UpSelectPixmap(iter.value());
    update();
}

void PicAnimationWid::SlotStartOrStop()
{
    if(!_b_start){ //非开始状态
        _factor = 0;
        _timer->start(25);
        _b_start = true;
        emit SigStartMusic();
    }else{
        _timer->stop();
        _factor = 0;
        update();
        _b_start = false;
        emit SigStopMusic();
    }
}


void PicAnimationWid::TimeOut()
{
    if(!_cur_item) {
        Stop();
        update(); //调用paintevent
        return;
    }

    _factor = _factor+0.01;
    if(_factor>=1){
        _factor=0;
        auto * cur_pro_item = dynamic_cast<ProTreeItem*>(_cur_item);
        auto * next_pro_itrm = cur_pro_item->getNextItem();
        if(!next_pro_itrm){
            Stop();
            update();
            return;
        }

        SetPixmap(next_pro_itrm);
        update();
        return;
    }

    update();
}


