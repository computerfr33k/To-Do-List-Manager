#ifndef EDITTASK_H
#define EDITTASK_H

#include <QDialog>

namespace Ui {
class editTask;
}

class editTask : public QDialog
{
    Q_OBJECT
    
public:
    explicit editTask(QWidget *parent = 0);
    ~editTask();
    
private:
    Ui::editTask *ui;
};

#endif // EDITTASK_H
