#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    init_db();
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
    search_model.setQuery("SELECT id,row,type FROM sort_preferences", db);
    qDebug() << search_model.record(0).value(1).toInt();
    qDebug() << search_model.record(0).value(2).toInt();

    Dialog->set_sortRow(search_model.record(0).value(1).toInt());
    Dialog->set_sortType(search_model.record(0).value(2).toInt());

    if(Dialog->exec())
    {
        qDebug() << Dialog->get_sortRow();
        qDebug() << Dialog->get_sortType();

        QSqlQuery query("insert or replace into sort_preferences (id, row, type) VALUES ('1', :row, :type)");
        query.bindValue(0, Dialog->get_sortRow());
        query.bindValue(1, Dialog->get_sortType());
        if(query.exec())
            updateTable();
    }
}

void MainWindow::on_addTask_button_clicked()
{
    insertItem *dialogBox = new insertItem;
    connect(dialogBox, SIGNAL(finished(int)), dialogBox, SLOT(deleteLater()));

    if( dialogBox->exec() )
    {
        //db stuff
        QSqlQuery query ("insert into tasks (Task, DueDate, Completed, Priority, Notes) values (:name, :dueDate, :completed, :priority, :notes)");
        query.bindValue(0, dialogBox->get_task());
        query.bindValue(1, dialogBox->get_dueDate());
        query.bindValue(2, dialogBox->get_isCompleted());
        query.bindValue(3, dialogBox->get_priority());
        query.bindValue(4, dialogBox->get_notes());

        if(query.exec())
        {
            updateTable();
        }
    }
}

void MainWindow::on_removeTask_button_clicked()
{
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

    dialog->set_priority(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 3)).toInt()); //set Priority
    dialog->set_notes(ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 4)).toString()); //set Notes
    if(dialog->exec())
    {
        QSqlQuery query("update tasks set Task=:name,DueDate=:dueDate,Completed=:completed,Priority=:priority,Notes=:notes WHERE Task=\'" + ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0)).toString() + "\'");
        query.bindValue(0, dialog->get_taskName());
        query.bindValue(1, dialog->get_dueDate());
        query.bindValue(2, dialog->get_completed());
        query.bindValue(3, dialog->get_priority());
        query.bindValue(4, dialog->get_notes());
        if(!query.exec())
        {
            qDebug() << "*** ERROR: line 171; mainwindow.cpp";
            qDebug() << "SQL Error! ***";
        }
    }
    updateTable();
}

void MainWindow::loadTasks()
{
}

void MainWindow::saveTasks()
{
}

//button hooks
void MainWindow::enableRemoveTask_button()
{
}

//table
void MainWindow::init_db()
{
    //create folders for storing the db
    qDebug() << QDesktopServices::storageLocation(QDesktopServices::DataLocation);
    bool ok = dir.mkpath(QDesktopServices::storageLocation(QDesktopServices::DataLocation));

    if(!ok)
        QMessageBox::critical(this, "Warning", "Could Not Create Storage Path");

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/tasks.db");
    db.open();
    db.exec("create table if not exists tasks (Task, DueDate, Completed, Priority, Notes)");
    db.exec("create table if not exists sort_preferences (id PRIMARY KEY, row, type)");

    all_model = new QSqlTableModel(this, db);
    ui->tableView->setModel(all_model);
    updateTable();
}

void MainWindow::updateTable()
{
    all_model->setTable("tasks");
    all_model->select();
    ui->tableView->setModel(all_model);
    search_model.setQuery("SELECT id,row,type FROM sort_preferences", db);
    if(search_model.record(0).value(2).toInt() == 0)
        ui->tableView->sortByColumn(search_model.record(0).value(1).toInt(), Qt::AscendingOrder);
    else if(search_model.record(0).value(2).toInt() == 1)
        ui->tableView->sortByColumn(search_model.record(0).value(1).toInt(), Qt::DescendingOrder);
}

// end private slots

MainWindow::~MainWindow()
{
    db.exec("VACUUM");
    db.close();

    delete all_model;
    //delete search_model;
    delete ui;
    //delete tasks;
}
