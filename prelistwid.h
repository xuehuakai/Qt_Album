#ifndef PRELISTWID_H
#define PRELISTWID_H

/*预览窗口*/

#include <QListWidget>
#include<QTreeWidgetItem>
class PreListWid : public QListWidget
{
    Q_OBJECT
private:
    QMap<QString,QListWidgetItem*> _set_items; //path - item
    void AddListItem(const QString& path);
    int _global;
    QPoint _pos_origin; //缓存图片位置，为了切换
    int last_index;
public:
    PreListWid(QWidget *parent = nullptr);
    virtual ~PreListWid();

public slots:
    void SlotUpPreList(QTreeWidgetItem* tree_item);
    void SlotUpSelect(QTreeWidgetItem* tree_item);
    void SlotItemPressed(QListWidgetItem * item);
signals:
    void SigUpSelect(const  QString &  path);
};

#endif // PRELISTWID_H
