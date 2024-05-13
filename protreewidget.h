#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include "opentreethread.h"
#include "protreethread.h"

#include <QTreeWidget>
#include<QProgressDialog>
#include<QAction>
#include<QtMultimedia/QMediaPlayer>
#include<QAudioOutput>
class SlideShowDlg;

class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProTreeWidget(QWidget* p = nullptr);
    void AddProToTree(const QString & name,const QString &path);
private:
    QSet<QString> _set_path; //判断重名
    //右键单击某个item后弹出菜单
    QTreeWidgetItem * _right_btn_item;
    QTreeWidgetItem * _active_item; //保存当前启动项目（缓存）
    QTreeWidgetItem * _selected_item; //当前选中的项
    QAction * _action_import; //导入动作
    QAction * _action_setstart;
    QAction * _action_closepro;
    QAction * _action_slideshow; //轮播图播放动作
    QProgressDialog * _dialog_progress;
    QProgressDialog * _open_progressdlg;

    std::shared_ptr<ProTreeThread> _thread_create_pro ;
    std::shared_ptr<OpenTreeThread> _thread_open_pro;

    std::shared_ptr<SlideShowDlg> _slide_show_dlg;

    QMediaPlayer * _player;
    QAudioOutput * _output;
public slots:
    void SlotOpenPro(const QString& path ); //在mainwindow中connect的，公有槽
    void SlotNextShow();
    void SlotPreShow();
    void SlotSetMusic();
    void SlotStartMusic();
    void SlotStopMusic();
private slots:
    void SlotItemPressed(QTreeWidgetItem *item, int column);
    void SlotImport();
    void SlotSetActive();
    void SlotClosePro();
    void SlotUpdateProgress(int count);
    void SlotFinishProgress(int count);
    void SlotCancalProgress();

    void SlotUpOpenProgress(int count);
    void SlotFinishOpenProgress(int count);
    void SlotCancalOpenProgress();

    void SlotDoubleClickItem(QTreeWidgetItem* doubleItem,int col); //column是列数
    void SlotSlideShow();
signals:
    void SigCancelProgress();
    void SigCancelOpenProgress();
    void SigUpdateSelected(const QString& );
    void SigUpdatePic(const QString&);
    void SigClearSelected();
};

#endif // PROTREEWIDGET_H
