#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //create folders for storing the db
    qDebug() << QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    bool ok = dir.mkpath(QDesktopServices::storageLocation(QDesktopServices::DataLocation));

    if(!ok)
        QMessageBox::critical(this, "Warning", "Could Not Create Storage Path");

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/tasks.db");
    db.open();
    db.exec("create table if not exists tasks (Task, DueDate, Completed, Notes)");
    db.exec("create table if not exists preferences (row, sorting)");

    all_model = new QSqlTableModel(this, db);
    ui->tableView->setModel(all_model);
    updateTable();
    ui->tableView->sortByColumn(0, Qt::AscendingOrder);

    search_model = new QSqlTableModel(this, db);
    search_model->setTable("tasks");

    // ini stuff
    //tasks = new QSettings("tasks.ini", QSettings::IniFormat);
    //loadTasks();

    //ui->removeTask_button->setDisabled(true);
    //ui->editTask_button->setDisabled(true);
    // hook remove and edit buttons
    //connect(ui->tableWidget, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(enableRemoveTask_button()));
}

void MainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

// private slots
void MainWindow::on_actionAbout_activated()
{
    QMessageBox::about(this, "About", "To Do List Manager \nCreated By Computerfr33k \nVersion: " + QApplication::applicationVersion());
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

void MainWindow::on_actionPreferences_activated()
{
    Preferences *Dialog = new Preferences;
    connect(Dialog, SIGNAL(finished(int)), Dialog, SLOT(deleteLater()));
    Dialog->exec();
}

void MainWindow::on_addTask_button_clicked()
{
    insertItem *dialogBox = new insertItem;
    connect(dialogBox, SIGNAL(finished(int)), dialogBox, SLOT(deleteLater()));

    if( dialogBox->exec() )
    {
        /*
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
        */

        //db stuff
        QSqlQuery query ("insert into tasks (Task, DueDate, Completed, Notes) values (:name, :dueDate, :completed, :notes)");
        query.bindValue(0, dialogBox->get_task());
        query.bindValue(1, dialogBox->get_dueDate());
        query.bindValue(2, dialogBox->get_isCompleted());
        query.bindValue(3, dialogBox->get_notes());

        if(query.exec())
        {
            updateTable();
        }
    }
}

void MainWindow::on_removeTask_button_clicked()
{
    /*
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
    */

    QSqlQuery query;
    query.prepare("delete from tasks where Task = \'" + ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString() +"\'");

    if(!query.exec())
    {
        QMessageBox::warning(0,"Error", "Could Not Delete The Selected Task" + ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString() + ".");
        return;
    }
    updateTable();
}

void MainWindow::on_editTask_button_clicked()
{
    editTask *dialog = new editTask;
    connect(dialog, SIGNAL(finished(int)), dialog, SLOT(deleteLater()));
    //set Task Name For Edit Task Dialog
    dialog->set_taskName(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString());
    //set Due Date
    dialog->set_dueDate(QDateTime::fromString(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 1)).toString(), "MM/dd/yyyy hh:mm:ss AP"));

    //copy completed to variable for easier comparing
    QString completed = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 2)).toString();
    //set completed to yes or no
    if(completed.compare("No") == 0)
        dialog->set_completed(0);
    else
        dialog->set_completed(1);

    dialog->set_notes(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 3)).toString()); //set Notes
    if(dialog->exec())
    {
        QSqlQuery query("update tasks set Task=:name,DueDate=:dueDate,Completed=:completed,Notes=:notes WHERE Task=\'" + ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString() + "\'");
        query.bindValue(0, dialog->get_taskName());
        query.bindValue(1, dialog->get_dueDate());
        query.bindValue(2, dialog->get_completed());
        query.bindValue(3, dialog->get_notes());
        if(!query.exec())
        {
            qDebug() << "*** ERROR: line 171; mainwindow.cpp";
            qDebug() << "SQL Error! ***";
        }
    }
    updateTable();

    /*
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
    */
}

void MainWindow::loadTasks()
{
    /*
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
    */

}

void MainWindow::saveTasks()
{
}

//button hooks
void MainWindow::enableRemoveTask_button()
{
    /*
     *if(ui->tableWidget->currentRow() != -1)
    {
        ui->removeTask_button->setEnabled(true);
        ui->editTask_button->setEnabled(true);
    }
    */
}

//table
void MainWindow::updateTable()
{
    all_model->setTable("tasks");
    all_model->select();
    ui->tableView->setModel(all_model);
}

// end private slots

MainWindow::~MainWindow()
{
    db.exec("VACUUM");
    db.close();

    delete all_model;
    delete search_model;
    delete ui;
    //delete tasks;
}
