#ifndef INSERTITEM_H
#define INSERTITEM_H

#include <QDialog>
#include <QtGui>

namespace Ui {
class insertItem;
}

class insertItem : public QDialog
{
    Q_OBJECT
    
public:
    explicit insertItem(QWidget *parent = 0);
    ~insertItem();

    QString get_task();
    QString get_dueDate();
    QString get_isCompleted();
    QString get_priority();
    QString get_notes();

private slots:
    void checkTaskText();
    
private:
    Ui::insertItem *ui;
};

#endif // INSERTITEM_H
