#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

//处理选择文件目录显示文件到treewidget时的线程

#include <QThread>
class QTreeWidgetItem;
class QTreeWidget;
class ProTreeThread : public QThread
{
    Q_OBJECT
public:
    //原路径 目标路径 父结点 文件数 treewidget本身 根节点

    /*
    _thread_create_pro = std::make_shared<ProTreeThread>(std::ref(import_path),std::ref(path),
                                                         file_count,this,_right_btn_item);*/

    ProTreeThread(const QString & src_path,const QString & dist_path,QTreeWidgetItem * parent_item,
                  int file_count,QTreeWidget * self,QTreeWidgetItem * root,QObject* parent = nullptr);
    ~ProTreeThread();
protected:
    virtual void run();
private:
    //创建tree的结点
    void CreateProTree(const QString & src_path,const QString & dist_path,QTreeWidgetItem * parent_item,
                       int & file_count,QTreeWidget * self,QTreeWidgetItem * root,
                       QTreeWidgetItem * preItem = nullptr);
    QString _src_path;
    QString _dist_path;
    int _file_count;
    QTreeWidgetItem * _parent_item;
    QTreeWidgetItem * _root;
    QTreeWidget * _self;

    bool _bstop;

public slots:
    void SlotCancelProgress();

signals:
    void SigUpdateProgress(int); //更新
    void SigFinishProgress(int); //完成
};

#endif // PROTREETHREAD_H
