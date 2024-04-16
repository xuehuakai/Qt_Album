#ifndef SLIDESHOWDLG_H
#define SLIDESHOWDLG_H

#include <QDialog>
#include<QTreeWidgetItem>
namespace Ui {
class SlideShowDlg;
}

class SlideShowDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SlideShowDlg(QWidget *parent,QTreeWidgetItem* first_item,
                          QTreeWidgetItem* last_item);
    ~SlideShowDlg();

private:
    Ui::SlideShowDlg *ui;
};

#endif // SLIDESHOWDLG_H
