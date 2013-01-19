#include "insertitem.h"
#include "ui_insertitem.h"

insertItem::insertItem(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::insertItem)
{
    ui->setupUi(this);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(checkTaskText()));
}

insertItem::~insertItem()
{
    delete ui;
}

QString insertItem::get_task()
{
    return ui->lineEdit->text();
}

QString insertItem::get_dueDate()
{
    return ui->dateTimeEdit->dateTime().toString("ddd MM/dd/yyyy hh:mm:ss AP");
}

QString insertItem::get_isCompleted()
{
    return ui->comboBox->currentText();
}

QString insertItem::get_notes()
{
    return ui->plainTextEdit->toPlainText();
}

void insertItem::checkTaskText()
{
    if(ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, "Error", "Task Name Is Empty!");
    }
    else
    {
        insertItem::accept();
    }
}
