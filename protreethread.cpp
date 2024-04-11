#include "protreethread.h"
#include "protreeitem.h"
#include "qdir.h"
#include"const.h"

using namespace Qt;

ProTreeThread::ProTreeThread(const QString &src_path,
                             const QString &dist_path,
                             QTreeWidgetItem *parent_item,
                             int file_count, QTreeWidget *self,
                             QTreeWidgetItem *root, QObject *parent)
    :QThread(parent),_src_path(src_path),_dist_path(dist_path),
    _file_count(file_count),_parent_item(parent_item),_root(root),
    _self(self),_bstop(false)
{

}

ProTreeThread::~ProTreeThread()
{

}

void ProTreeThread::run()
{
    //运行完就结束
    CreateProTree(_src_path,_dist_path,_parent_item,
                  _file_count,_self,_root); //创建项目树
    if(_bstop){
        QString path = dynamic_cast<ProTreeItem*>(_self)->GetPath();
        int index = _self->indexOfTopLevelItem(_root);
        delete _self->takeTopLevelItem(index);
        QDir dir(path);
        dir.removeRecursively();
        return;
    }

    emit SigFinishProgress(_file_count);
}

//创建treewidget
void ProTreeThread::CreateProTree(const QString &src_path, const QString &dist_path,
                                  QTreeWidgetItem *parent_item, int &file_count, QTreeWidget *self,
                                  QTreeWidgetItem *root, QTreeWidgetItem *preItem)
//为什么pre_item不能直接设置为protreeitem 因为为了避免互引用
{
    //不用exit terminate（生硬）退出，自己手动控制退出
    if(_bstop){
        return;
    }
    bool needcopy = true; //同一个路径不用copy  不同的需要
    if(src_path == dist_path){
        needcopy = false;
    }
    QDir import_dir(src_path); //导入路径
    //设置文件过滤器
    QStringList nameFilters;
    import_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    import_dir.setSorting(QDir::Name);
    QFileInfoList list =  import_dir.entryInfoList(); //过滤后的所有有效的文件和文件夹
    for(int i = 0 ; i<list.size();i++){
        if(_bstop){
            return;
        }

        QFileInfo fileInfo = list.at(i);
        bool bIsDir  = fileInfo.isDir();
        if(bIsDir){
            if(_bstop)
                return;
            file_count++;
            emit SigUpdateProgress(file_count);


            QDir  dist_dir(dist_path);
            //构造子目的路径
            QString sub_dist_path = dist_dir.absoluteFilePath(fileInfo.fileName());
            qDebug()<<"the sub_dist_path is "<<sub_dist_path;
            QDir sub_dist_dir(sub_dist_path);
            if(!sub_dist_dir.exists()){
                //不能存在则创建
                bool ok  = sub_dist_dir.mkpath(sub_dist_path);
                if(!ok){
                    qDebug()<<"create the sub_dist_dir is failed!";
                    continue;
                }else{
                    qDebug()<< "sub_dist_dir mkpath success.";
                }
            }

            ProTreeItem * item = new ProTreeItem(parent_item,fileInfo.fileName()
                                                ,sub_dist_path,root,TreeItemDir);

            item->setData(0,Qt::DisplayRole, fileInfo.fileName());
            item->setData(0,Qt::DecorationRole, QIcon(":/icon/dir.png"));
            item->setData(0,Qt::ToolTipRole, sub_dist_path);

            CreateProTree(fileInfo.absoluteFilePath(), sub_dist_path, item, file_count, self,root,preItem);


        }else{
            if(_bstop)
                return;

            const QString & suffix = fileInfo.completeSuffix() ; //完整后缀
            if(suffix !="png" && suffix !="ipeg" && suffix!="jpg"){
                qDebug()<<"suffix is not pic";
                continue;
            }

            file_count++;
            emit SigUpdateProgress(file_count);
            if(!needcopy){
                continue;
            }

            QDir dist_dir(dist_path);
            QString dist_file_path = dist_dir.absoluteFilePath(fileInfo.fileName());
            //将源文件赋值到目标路径的文件
            if(!QFile::copy(fileInfo.absoluteFilePath(),dist_file_path)){
                continue;
            }

            auto * item = new ProTreeItem(parent_item,fileInfo.fileName(),
                                         dist_file_path,root,TreeItemPic); //构建结点

            item->setData(0,Qt::DisplayRole,fileInfo.fileName()); //名字
            item->setData(0,Qt::DecorationRole,QIcon(":/icon/pic.png"));
            item->setData(0,Qt::ToolTipRole,dist_file_path); //提示的是路径

            if(preItem){ //前一个结点不是为空
                //链接
                auto * pre_proitem = dynamic_cast<ProTreeItem*>(preItem); //用来设置后一个节点
                pre_proitem->setNextItem(item);
            }

            item->setPreItem(preItem);
            preItem = item;
        }
    }

    parent_item->setExpanded(true); //展开项目
}

void ProTreeThread::SlotCancelProgress()
{
    _bstop = true;
}
