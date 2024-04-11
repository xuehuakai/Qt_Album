#include "protreeitem.h"
ProTreeItem::ProTreeItem(QTreeWidget *view, const QString &name, const QString &path, int type)
    :QTreeWidgetItem(view,type),_path(path),_name(path),_root(this),_pre_item(nullptr),_next_item(nullptr)
{

}

ProTreeItem::ProTreeItem(QTreeWidgetItem *parent, const QString &name, const QString &path,
                         QTreeWidgetItem *root, int type)
    :QTreeWidgetItem(parent,type),_path(path),_name(name),_root(this),_pre_item(nullptr),_next_item(nullptr)
{

}

const QString &ProTreeItem::GetPath()
{
    return _path;
}

QTreeWidgetItem *ProTreeItem::GetRoot()
{
    return _root;
}

void ProTreeItem::setPreItem(QTreeWidgetItem *item)
{
    _pre_item = item;
}

void ProTreeItem::setNextItem(QTreeWidgetItem *item)
{
    _next_item = item;
}

ProTreeItem *ProTreeItem::GetPreItem()
{
    return dynamic_cast<ProTreeItem*>(_pre_item);
}

ProTreeItem *ProTreeItem::getNextItem()
{
    return dynamic_cast<ProTreeItem*>(_next_item);
}

ProTreeItem *ProTreeItem::GetLastPicChild()
{

}

ProTreeItem *ProTreeItem::GetFirstPicChild()
{

}
