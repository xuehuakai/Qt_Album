#ifndef PROTREEITEM_H
#define PROTREEITEM_H

#include <QTreeWidgetItem>

class ProTreeItem : public QTreeWidgetItem
{
public:
    ProTreeItem(QTreeWidget * view,const QString& name,const QString &path,int type = Type);
    ProTreeItem(QTreeWidgetItem* parent,const QString &name,const QString &path,
                QTreeWidgetItem* root,int type = Type); //root是根节点 想不到的可以预留参数

    const QString & GetPath();
    QTreeWidgetItem * GetRoot();
    void setPreItem(QTreeWidgetItem * item);
    void setNextItem(QTreeWidgetItem * item);
    ProTreeItem * GetPreItem();
    ProTreeItem * getNextItem();
    ProTreeItem * GetLastPicChild();
    ProTreeItem * GetFirstPicChild();
private:
    QString _path;
    QString _name;
    QTreeWidgetItem * _root;
    QTreeWidgetItem * _pre_item;
    QTreeWidgetItem * _next_item;
};

#endif // PROTREEITEM_H
