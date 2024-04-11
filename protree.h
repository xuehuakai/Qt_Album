#ifndef PROTREE_H
#define PROTREE_H

#include "qtreewidget.h"
#include <QDialog>

namespace Ui {
class ProTree;
}

class ProTree : public QDialog
{
    Q_OBJECT

public:
    explicit ProTree(QWidget *parent = nullptr);
    ~ProTree();
    QTreeWidget * GetTreeWidget();
private:
    Ui::ProTree *ui;

public slots:
    void AddProToTree(const QString name,const QString path);
};

#endif // PROTREE_H
