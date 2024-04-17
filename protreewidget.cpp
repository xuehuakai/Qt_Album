#include "protreewidget.h"
#include "const.h"
#include "protreeitem.h"
#include "qfiledialog.h"
#include "qmenu.h"
#include"removeprodialog.h"
#include"slideshowdlg.h"
#include<QHeaderView>
#include<QDir>
#include<QFont>
#include<QGuiApplication>
#include<QDebug>
ProTreeWidget::ProTreeWidget(QWidget* p) : QTreeWidget(p),_right_btn_item(nullptr),_active_item(nullptr)
    ,_selected_item(nullptr),_dialog_progress(nullptr),_open_progressdlg(nullptr),_thread_open_pro(nullptr){
    this->header()->hide();

    connect(this,&ProTreeWidget::itemPressed,
            this,&ProTreeWidget::SlotItemPressed);

    _action_import = new QAction(QIcon(":/icon/import.png"),"导入文件",this);
    _action_setstart = new QAction(QIcon("/icon/core.png"),"设置活动项目",this);
    _action_closepro = new QAction(QIcon(":/icon/close.png"),"关闭项目",this);
    _action_slideshow = new QAction(QIcon(":/icon/slideshow.png"),"轮播图播放",this);

    connect(_action_import,&QAction::triggered,
            this,&ProTreeWidget::SlotImport);
    connect(_action_setstart,&QAction::triggered,
            this,&ProTreeWidget::SlotSetActive);
    connect(_action_closepro,&QAction::triggered,
            this,&ProTreeWidget::SlotClosePro);
    connect(this,&ProTreeWidget::itemDoubleClicked,
            this,&ProTreeWidget::SlotDoubleClickItem);
    connect(_action_slideshow,&QAction::triggered,
            this,&ProTreeWidget::SlotSlideShow);
}

void ProTreeWidget::AddProToTree(const QString &name, const QString &path)
{
    QDir dir(path);
    QString file_path = dir.absoluteFilePath(name); //拼的新路径
    if(_set_path.find(file_path) != _set_path.end()){
        return;
    }
    QDir pro_dir(file_path);
    if(!pro_dir.exists()){
        bool enable = pro_dir.mkpath(file_path); //创建目录,上级存不存在没关系
        if(!enable){
            return;
        }
    }
    _set_path.insert(file_path);
    auto * item= new ProTreeItem(this,name,file_path,TreeItemPro);
    item->setData(0,Qt::DisplayRole,name);
    item->setData(0,Qt::DecorationRole,QIcon(":/icon/dir.png"));
    item->setData(0,Qt::ToolTipRole,file_path);
    addTopLevelItem(item);
}

void ProTreeWidget::SlotOpenPro(const QString &path)
{
    qDebug()<<__FUNCTION__;
    if(_set_path.contains(path)){
        return;
    }

    _set_path.insert(path);
    int file_count = 0;
    QDir pro_dir(path);
    const QString  proname = pro_dir.dirName();

    _thread_open_pro = std::make_shared<OpenTreeThread>(path,file_count,this,nullptr);
    _thread_open_pro->start();

    _open_progressdlg = new QProgressDialog(this);

    connect(_thread_open_pro.get(),&OpenTreeThread::SigUpdateProgress,
            this,&ProTreeWidget::SlotUpOpenProgress);
    connect(_thread_open_pro.get(),&OpenTreeThread::SigFinishProgress,
            this,&ProTreeWidget::SlotFinishOpenProgress);
    connect(_open_progressdlg,&QProgressDialog::canceled,
            this,&ProTreeWidget::SlotCancalProgress);
    connect(this,&ProTreeWidget::SigCancelOpenProgress,
            _thread_open_pro.get(),&OpenTreeThread::SlotCancelProgress);

    _open_progressdlg->setWindowTitle("Please wait...");
    _open_progressdlg->setFixedWidth(PROGRESS_WIDTH);
    _open_progressdlg->setRange(0,PROGRESS_WIDTH);
    _open_progressdlg->exec();


}

//前后按钮类似于双向链表，之前的函数：getnextitem()
void ProTreeWidget::SlotNextShow()
{
    if(!_selected_item){ //选中的条目为空
        return;
    }

    auto * curItem = dynamic_cast<ProTreeItem*>(_selected_item)->getNextItem();
    if(!curItem) return;

    emit SigUpdatePic(curItem->GetPath()); //告诉picshow我们的图片更新了
    _selected_item = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::SlotPreShow()
{
    if(!_selected_item){ //选中的条目为空
        return;
    }

    auto * curItem = dynamic_cast<ProTreeItem*>(_selected_item)->GetPreItem();
    if(!curItem) return;

    emit SigUpdatePic(curItem->GetPath()); //告诉picshow我们的图片更新了
    _selected_item = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::SlotItemPressed(QTreeWidgetItem* pressedItem,int column){
    if(QGuiApplication::mouseButtons() == Qt::RightButton){
        //弹出菜单
        QMenu menu(this);
        int itemtype = pressedItem->type();
        if(itemtype == TreeItemPro){
            _right_btn_item = pressedItem; //尽量缓存一些条目
            menu.addAction(_action_import);
            menu.addAction(_action_setstart);
            menu.addAction(_action_closepro);
            menu.addAction(_action_slideshow);
            menu.exec(QCursor::pos());
        }
    }
}

void ProTreeWidget::SlotImport()
{
   // QFileDialog
    QFileDialog file_dialog;
   file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle("选择导入的文件夹");
    QString path  = ""; //局部变量存储路径
    if(!_right_btn_item){
        qDebug()<<"_right_btn_item is empty";
        return;
    }

    path = dynamic_cast<ProTreeItem*>(_right_btn_item)->GetPath(); //目的路径
    file_dialog.setDirectory(path);
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(file_dialog.exec()){
        fileNames = file_dialog.selectedFiles();
    }

    if(fileNames.length() <= 0) return;

     QString import_path = fileNames.at(0); //默认只打开第一个文件夹
    int file_count = 0; //文件个数
    _dialog_progress = new QProgressDialog(this);

    _thread_create_pro = std::make_shared<ProTreeThread>(std::ref(import_path),std::ref(path),_right_btn_item,
                                                         file_count,this,_right_btn_item);
    //this就是传的这个widget    _right_btn_item就是这个根的参数

    connect(_thread_create_pro.get(),&ProTreeThread::SigUpdateProgress,
            this,&ProTreeWidget::SlotUpdateProgress);
    connect(_thread_create_pro.get(),&ProTreeThread::SigFinishProgress,
            this,&ProTreeWidget::SlotFinishProgress);
    connect(_dialog_progress,&QProgressDialog::canceled,
            this,&ProTreeWidget::SlotCancalProgress);
    connect(this,&ProTreeWidget::SigCancelProgress,
            _thread_create_pro.get(),&ProTreeThread::SlotCancelProgress);

    _thread_create_pro->start();
    _dialog_progress->setWindowTitle("Please wait...");
    _dialog_progress->setFixedWidth(PROGRESS_WIDTH);
    _dialog_progress->setRange(0,PROGRESS_WIDTH);
    _dialog_progress->exec();
}

void ProTreeWidget::SlotSetActive()
{
    if(!_right_btn_item){
        return;
    }

    QFont nullFont;
    nullFont.setBold(false);

    if(_active_item){
        _active_item->setFont(0,nullFont);
    }

    _active_item = _right_btn_item;
    nullFont.setBold(true);
    _active_item->setFont(0,nullFont);

}

void ProTreeWidget::SlotClosePro()
{
    RemoveProDialog remove_pro_dialog;
    auto res = remove_pro_dialog.exec();
    if(res != QDialog::Accepted){
        return;
    }
    bool b_remove = remove_pro_dialog.IsRemoved();
    auto index_right_btn = this->indexOfTopLevelItem(_right_btn_item);
    auto * protreeitem = dynamic_cast<ProTreeItem*>(_right_btn_item);
    auto * selecteditem = dynamic_cast<ProTreeItem*>(_selected_item); //选择的状态和右键状态是不一样的
    auto delete_path = protreeitem->GetPath();
    _set_path.remove(delete_path);
    if(b_remove){
        QDir delete_dir(delete_path); //通过删除路径构造一个目录
        delete_dir.removeRecursively(); //递归的删掉
    }

    if(protreeitem ==  _active_item){
        _active_item = nullptr;
    }

    if(selecteditem && protreeitem ==selecteditem->GetRoot() ){ //右侧显示图片的根节点恰好是我们关闭的项目
        selecteditem = nullptr;
        _selected_item = nullptr;
        emit SigClearSelected();
    }

    delete this->takeTopLevelItem(index_right_btn);
    _right_btn_item = nullptr;
}

void ProTreeWidget::SlotUpdateProgress(int count)
{
    if(!_dialog_progress){ //担心对话框闪退
        return;
    }

    if(count >= PROGRESS_MAX){
        _dialog_progress->setValue(count%PROGRESS_MAX);
    }else{
        _dialog_progress->setValue(count);
    }
}

void ProTreeWidget::SlotFinishProgress(int count){
    Q_UNUSED(count);
    _dialog_progress->setValue(PROGRESS_MAX);
    _dialog_progress->deleteLater();
}

void ProTreeWidget::SlotCancalProgress()
{
    emit SigCancelProgress();
    delete _dialog_progress;
    _dialog_progress = nullptr;
}

void ProTreeWidget::SlotUpOpenProgress(int count)
{
    if(!_open_progressdlg){ //担心对话框闪退
        return;
    }

    if(count >= PROGRESS_MAX){
        _open_progressdlg->setValue(count%PROGRESS_MAX);
    }else{
        _open_progressdlg->setValue(count);
    }
}

void ProTreeWidget::SlotFinishOpenProgress(int count)
{
    if(!_open_progressdlg){ //担心对话框闪退
        return;
    }
    _open_progressdlg->setValue(PROGRESS_MAX);
    _open_progressdlg->deleteLater();
    _open_progressdlg = nullptr;
}

void ProTreeWidget::SlotCancalOpenProgress()
{
    emit SigCancelOpenProgress();
    delete _open_progressdlg;
    _open_progressdlg = nullptr;
}

void ProTreeWidget::SlotDoubleClickItem(QTreeWidgetItem *doubleItem, int col)
{
    if(QGuiApplication::mouseButtons() == Qt::LeftButton){
        auto * tree_doubleItem = dynamic_cast<ProTreeItem*>(doubleItem);
        if(!tree_doubleItem){
            return;
        }
        int itemtype = tree_doubleItem->type();
        if(itemtype == TreeItemPic){
            emit SigUpdateSelected(tree_doubleItem->GetPath()); //发送路径
            _selected_item =doubleItem; //缓存当前选中的条目
        }

    }
}

void ProTreeWidget::SlotSlideShow()
{
    if(!_right_btn_item){
        return;
    }

    auto * right_pro_item = dynamic_cast<ProTreeItem*>(_right_btn_item) ;//变成真实类型
    auto * last_child_item = right_pro_item->GetLastPicChild();
    if(!last_child_item){
        return;
    }
    auto * first_child_item = right_pro_item->GetFirstPicChild();
    if(!first_child_item){
        return;
    }

    _slide_show_dlg = std::make_shared<SlideShowDlg>(this,
                                                     first_child_item,last_child_item);
    _slide_show_dlg->setModal(true); //模态
    _slide_show_dlg->showMaximized();

}
