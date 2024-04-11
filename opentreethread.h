#ifndef OPENTREETHREAD_H
#define OPENTREETHREAD_H

#include "qtreewidget.h"
#include <QThread>

class OpenTreeThread : public QThread
{
    Q_OBJECT
public:
    explicit OpenTreeThread(const QString & src_path,int file_count,
                            QTreeWidget* self,QObject *parent = nullptr);
    void OpenProTree(const QString & src_path,int &file_count,QTreeWidget* self);
protected:
    virtual void run() override;
private:
    void RecursiveProTree(const QString & src_path,int & file_count,QTreeWidget * self,
                          QTreeWidgetItem * root,QTreeWidgetItem * parent,QTreeWidgetItem* preitem);
    QString _src_path; //源路径
    int _file_count; //导入的文件数量
    QTreeWidget* _self; //给哪个QWidget去添加
    bool _bstop; //控制是否停止
    QTreeWidgetItem* _root; //根节点
signals:
    void SigFinishProgress(int);
    void SigUpdateProgress(int);
public slots:
    void SlotCancelProgress();
};

#endif // OPENTREETHREAD_H
