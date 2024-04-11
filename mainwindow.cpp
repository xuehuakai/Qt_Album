#include "mainwindow.h"
#include "picshow.h"
#include "protree.h"
#include "qfiledialog.h"
#include "ui_mainwindow.h"
#include<QMenu>
#include<QAction>
#include<QDebug>
#include"wizard.h"
#include"protreewidget.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //创建菜单
    QMenu* menu_file = menuBar()->addMenu("文件(&F)");
    //创建项目动作
    QAction* act_create_pro = new QAction(QIcon(":/icon/createpro.png"),"创建项目",this); //交给mainwindow管理
    act_create_pro->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_N));
    menu_file->addAction(act_create_pro);

    //打开项目
    QAction * act_open_pro = new QAction(QIcon(":/icon/openpro.png"),"打开项目",this);
    act_open_pro->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_O));
    menu_file->addAction(act_open_pro);

    //创建设置菜单
    QMenu * menu_set = menuBar()->addMenu("设置(&S)");
    //设置背景音乐
    QAction * act_music = new QAction(QIcon(":/icon/music.png"),"背景音乐",this);
    act_music->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_M));
    menu_set->addAction(act_music);
    //menu_set->menuAction();

    //连接信号和槽
    connect(act_create_pro,&QAction::triggered,this,&MainWindow::SlotCreatePro);
    connect(act_open_pro,&QAction::triggered,this,&MainWindow::SlotOpenPro);


    _protree = new ProTree();
    ui->proLayout->addWidget(_protree);
    QTreeWidget * tree_widget = dynamic_cast<ProTree*>(_protree)->GetTreeWidget();
    auto * pro_tree_widget = dynamic_cast<ProTreeWidget*>(tree_widget);
    connect(this,&MainWindow::SigOpenPro,pro_tree_widget,&ProTreeWidget::SlotOpenPro);


    _picshow = new PicShow();
    ui->picLayout->addWidget(_picshow);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotCreatePro(bool)
{
    qDebug()<<"slot create  pro triggere 【创建】";
    Wizard wizard(this);
    wizard.setWindowTitle("创建项目");
    QWizardPage * page = wizard.page(0); //第一页
    page->setTitle("设置项目配置");
    //链接信号和槽，把项目配置传回来
    connect(&wizard,&Wizard::SinProSettings,
            dynamic_cast<ProTree*>(_protree),&ProTree::AddProToTree);
    //wizard.show();
    wizard.exec();

    //断开所有信号 todo...
    disconnect();
}

void MainWindow::SlotOpenPro(bool)
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory); //设置为目录模式
    file_dialog.setWindowTitle("选择导入的文件夹");
    file_dialog.setDirectory(QDir::currentPath());
    file_dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if(file_dialog.exec()){
        fileNames = file_dialog.selectedFiles();
    }

    if(fileNames.length()<=0) return;

    QString import_path  = fileNames.at(0);
    emit SigOpenPro(import_path);
}
