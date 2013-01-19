#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tasks = new QSettings("tasks.ini", QSettings::IniFormat);
    loadTasks();

    ui->removeTask_button->setDisabled(true);
    ui->editTask_button->setDisabled(true);
    // hook remove and edit buttons
    connect(ui->tableWidget, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(enableRemoveTask_button()));
}

// private slots
void MainWindow::on_actionAbout_activated()
{
    QMessageBox::about(this, "About", "Version 0.1 Alpha");
}

void MainWindow::on_actionReport_A_Bug_activated()
{
    QDesktopServices::openUrl(QUrl("https://github.com/computerfr33k/To-Do-List-Manager/issues"));
}

void MainWindow::on_actionQuit_activated()
{
    qApp->quit();
}

void MainWindow::on_actionAbout_Qt_2_activated()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_addTask_button_clicked()
{
    insertItem *dialogBox = new insertItem;
    connect(dialogBox, SIGNAL(finished(int)), dialogBox, SLOT(deleteLater()));

    if( dialogBox->exec() )
    {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(dialogBox->get_task()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(dialogBox->get_dueDate()));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(dialogBox->get_isCompleted()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(dialogBox->get_notes()));

        tasks->beginGroup(QString::number(row));
        tasks->setValue("name", dialogBox->get_task());
        tasks->setValue("due_date", dialogBox->get_dueDate());
        tasks->setValue("is_completed", dialogBox->get_isCompleted());
        tasks->setValue("notes", dialogBox->get_notes());
        tasks->endGroup();
        tasks->sync();
    }
}

void MainWindow::on_removeTask_button_clicked()
{
    qDebug() << ui->tableWidget->currentRow();
    if(ui->tableWidget->rowCount() == 1)
    {
        //tasks->remove(QString::number(ui->tableWidget->currentRow()));
        ui->tableWidget->clearContents();
        ui->tableWidget->removeRow(0);
        ui->removeTask_button->setDisabled(true);
    }
    else
    {
        //tasks->remove(QString::number(ui->tableWidget->currentRow()));
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
    }

    tasks->clear();
    for(int i=0; i < ui->tableWidget->rowCount(); i++)
    {
        tasks->beginGroup(QString::number(i));
        tasks->setValue("name", ui->tableWidget->item(i, 0)->text());
        tasks->setValue("due_date", ui->tableWidget->item(i, 1)->text());
        tasks->setValue("is_completed", ui->tableWidget->item(i, 2)->text());
        tasks->setValue("notes", ui->tableWidget->item(i, 3)->text());
        tasks->endGroup();
        tasks->sync();
    }
}

void MainWindow::on_editTask_button_clicked()
{
    int row = ui->tableWidget->currentRow();
    insertItem *DialogBox = new insertItem;
    connect(DialogBox, SIGNAL(finished(int)), DialogBox, SLOT(deleteLater()));

    if( DialogBox->exec() )
    {
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem( DialogBox->get_task() ));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem( DialogBox->get_dueDate() ));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem( DialogBox->get_isCompleted() ));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem( DialogBox->get_notes() ));
    }
}

void MainWindow::loadTasks()
{
    QVector<QString> row = tasks->childGroups().toVector();
    for(int i=0; i < tasks->childGroups().toVector().size(); i++)
    {
        tasks->beginGroup(QString::number(row.at(i).toInt()));
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(tasks->value("name").toString()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(tasks->value("due_date").toString()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(tasks->value("is_completed").toString()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(tasks->value("notes").toString()));
        tasks->endGroup();
    }

    tasks->clear();
    for(int i=0; i < ui->tableWidget->rowCount(); i++)
    {
        tasks->beginGroup(QString::number(i));
        tasks->setValue("name", ui->tableWidget->item(i, 0)->text());
        tasks->setValue("due_date", ui->tableWidget->item(i, 1)->text());
        tasks->setValue("is_completed", ui->tableWidget->item(i, 2)->text());
        tasks->setValue("notes", ui->tableWidget->item(i, 3)->text());
        tasks->endGroup();
        tasks->sync();
    }

}

void MainWindow::saveTasks()
{
}

//button hooks
void MainWindow::enableRemoveTask_button()
{
    if(ui->tableWidget->currentRow() != -1)
    {
        ui->removeTask_button->setEnabled(true);
        ui->editTask_button->setEnabled(true);
    }
}

// end private slots

MainWindow::~MainWindow()
{
    delete ui;
    delete tasks;
}
