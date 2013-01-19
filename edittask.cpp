#include "edittask.h"
#include "ui_edittask.h"

editTask::editTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editTask)
{
    ui->setupUi(this);
}

editTask::~editTask()
{
    delete ui;
}
