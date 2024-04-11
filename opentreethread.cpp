#include "opentreethread.h"
#include "qdir.h"
#include"protreeitem.h"
#include"const.h"
OpenTreeThread::OpenTreeThread(const QString &src_path, int file_count,
                               QTreeWidget *self, QObject *parent):
    QThread(parent),_src_path(src_path),_file_count(file_count),_self(self),
    _bstop(false),_root(nullptr)

{

}

void OpenTreeThread::OpenProTree(const QString &src_path, int &file_count, QTreeWidget *self)
{
    qDebug()<<__FUNCTION__;
    QDir src_dir(src_path);
    QString name  = src_dir.dirName();
    //第一次导入肯定是要创建一个根节点
    auto * item = new ProTreeItem(self,name,src_path,TreeItemPro);
    item->setData(0,Qt::DisplayRole,name);
    item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
    item->setData(0,Qt::ToolTipRole,src_path);

    _root = item; //初始创建的item就是root

    RecursiveProTree(src_path,file_count,self,_root,item,nullptr);

}

void OpenTreeThread::run()
{
    qDebug()<<__FUNCTION__;
    OpenProTree(_src_path,_file_count,_self);
    //在run中写结束逻辑
    if(_bstop){
        auto path = dynamic_cast<ProTreeItem*>(_root)->GetPath();
        auto index = _self->indexOfTopLevelItem(_root);
        delete _self->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }

}

void OpenTreeThread::RecursiveProTree(const QString &src_path, int &file_count,
                                      QTreeWidget *self, QTreeWidgetItem *root,
                                      QTreeWidgetItem *parent, QTreeWidgetItem *preitem)
{
    qDebug()<<__FUNCTION__;
    QDir src_dir(src_path);
    //设置过滤器
    src_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    src_dir.setSorting(QDir::Name);
    QFileInfoList list = src_dir.entryInfoList();

    for(int i =0 ;i<list.size();i++){
        if(_bstop){
            return;
        }
        QFileInfo fileInfo = list.at(i);
        bool bisDir = fileInfo.isDir();
        if(bisDir){
            qDebug()<<"is dir";
            if(_bstop){
                return;
            }
            file_count++;
            emit SigUpdateProgress(file_count); //扔
            auto * item  = new ProTreeItem(root,fileInfo.fileName(),
                                         fileInfo.absoluteFilePath(),root,TreeItemDir);

            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
            item->setData(0,Qt::ToolTipRole,fileInfo.absoluteFilePath());

            RecursiveProTree(fileInfo.absoluteFilePath(),file_count,
                             self,root,item,preitem);
        }else{ //不是目录
            if(_bstop){
                return;
            }

            const QString & suffix = fileInfo.completeSuffix();
            if(suffix !="png" && suffix != "jpeg" && suffix!="jpg"){
                continue;
            }

            file_count++;
            emit SigUpdateProgress(file_count);

            auto * item  = new ProTreeItem(root,fileInfo.fileName(),
                                         fileInfo.absoluteFilePath(),root,TreeItemPic);

            item->setData(0,Qt::DisplayRole,fileInfo.fileName());
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/pic.png"));
            item->setData(0,Qt::ToolTipRole,fileInfo.absoluteFilePath());

            //构造双向链表
            if(preitem){
                auto * pre_proitem = dynamic_cast<ProTreeItem*>(preitem);
                pre_proitem->setNextItem(item);
            }
            item->setPreItem(preitem);
            preitem = item;

        }
    }

    emit SigFinishProgress(file_count);

}

void OpenTreeThread::SlotCancelProgress()
{
    //取消操作
    _bstop = true;
}
