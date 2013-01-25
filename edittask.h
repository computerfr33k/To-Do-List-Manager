#ifndef EDITTASK_H
#define EDITTASK_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
class editTask;
}

class editTask : public QDialog
{
    Q_OBJECT
    
public:
    explicit editTask(QWidget *parent = 0);
    ~editTask();

    void set_taskName(QString);
    void set_dueDate(QDateTime);
    void set_completed(int);
    void set_priority(int index);
    void set_notes(QString);

    //return statements
    QString get_taskName();
    QString get_dueDate();
    QString get_completed();
    QString get_priority();
    QString get_notes();
    
private:
    Ui::editTask *ui;
};

#endif // EDITTASK_H
