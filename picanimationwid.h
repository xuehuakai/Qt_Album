#ifndef PICANIMATIONWID_H
#define PICANIMATIONWID_H

#include "qtreewidget.h"
#include<QWidget>
class PicAnimationWid : public QWidget
{
    Q_OBJECT
public:
    explicit PicAnimationWid(QWidget *parent = nullptr);
    ~PicAnimationWid();
    void SetPixmap(QTreeWidgetItem * item); //从哪个文件开始播
    void Start();
    void Stop();
    void SlideNext();
    void SlidePre();
protected:
       virtual void paintEvent(QPaintEvent *event);
private:
    float _factor; //因子
    QTimer* _timer;
    QPixmap _pixmap1;
    QPixmap _pixmap2; //双缓冲绘图
    QTreeWidgetItem * _cur_item;
    QMap<QString,QTreeWidgetItem*> _map_item; //只处理新的
    bool _b_start;

    void UpSelectPixmap(QTreeWidgetItem * item);
public slots:
    void SlotUpSelect(const QString & path);
     void SlotStartOrStop();
private slots:
    void TimeOut();
signals:
    void SigUpPreList(QTreeWidgetItem * ); //通知下方缩略图
    void SigSelectItem(QTreeWidgetItem * ); //更新预览窗口item选中状态
    void SigStart();
    void SigStop();
    void SigStartMusic();
    void SigStopMusic();
};

#endif // PICANIMATIONWID_H
