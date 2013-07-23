#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tray = new QSystemTrayIcon;
    trayMenu = new QMenu;
    trayMenu->addAction("Hide");
    trayMenu->addAction("Show");
    trayMenu->addAction("Quit");
    tray->setContextMenu(trayMenu);
    tray->setIcon(QIcon(":/images/icon.png"));
    tray->show();
    connect(trayMenu, SIGNAL(triggered(QAction*)), this, SLOT(trayMenu_triggered(QAction*)));
    restoreGUI();
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "closeEvent(): triggered()";
    db.exec("VACUUM");
    db.close();
    settings->setValue("mainWindowGeometry", saveGeometry());
    settings->setValue("mainWindowState", saveState());
    settings->sync();
    tray->hide();
    event->accept();
}

void MainWindow::restoreGUI()
{
    if(QFile("portable.dat").exists())
    {
        qDebug() << "portable.dat detected!";
        DataLoc << "portable";
    }
    else
    {
        DataLoc = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
        qDebug() << "Application Data Folder: " << QStandardPaths::standardLocations(QStandardPaths::DataLocation);
    }
    bool ok = dir.mkpath(DataLoc.at(0));

    if(!ok)
    {
        QMessageBox::critical(this, "Warning", "Could Not Create Storage Path");
        this->close();
    }
    settings = new QSettings(DataLoc.at(0) + "/settings.ini", QSettings::IniFormat);
    restoreGeometry(settings->value("mainWindowGeometry").toByteArray());
    restoreState(settings->value("mainWindowGeometry").toByteArray());
}

// private slots
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About", "To Do List Manager \nCreated By Computerfr33k \nVersion: " + QApplication::applicationVersion());
}

void MainWindow::on_actionReport_A_Bug_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/computerfr33k/To-Do-List-Manager/issues"));
}

void MainWindow::on_actionQuit_triggered()
{
    this->close();
}

void MainWindow::on_actionAbout_Qt_2_triggered()
{
    QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_actionPreferences_triggered()
{
    Preferences *Dialog = new Preferences;
    connect(Dialog, SIGNAL(finished(int)), Dialog, SLOT(deleteLater()));
    search_model.setQuery("SELECT id,row,type FROM sort_preferences", db);
    /*
    qDebug() << search_model.record(0).value(1).toInt();
    qDebug() << search_model.record(0).value(2).toInt();
    */

    Dialog->set_sortRow(settings->value("Sorting/row", 0).toInt());
    Dialog->set_sortType(settings->value("Sorting/type", 0).toInt());
    Dialog->set_autoUpdate(settings->value("AutoUpdate/enabled", true).toBool());

    if(Dialog->exec())
    {
        qDebug() << "sortRow(): " << Dialog->get_sortRow();
        qDebug() << "sortType(): " << Dialog->get_sortType();
        qDebug() << "autoUpdate(): " << Dialog->get_autoUpdate();

        settings->setValue("Sorting/row", Dialog->get_sortRow());
        settings->setValue("Sorting/type", Dialog->get_sortType());
        settings->setValue("AutoUpdate/enabled", Dialog->get_autoUpdate());
        settings->sync();

        QSqlQuery query("insert or replace into sort_preferences (id, row, type) VALUES ('1', :row, :type)");
        query.bindValue(0, Dialog->get_sortRow());
        query.bindValue(1, Dialog->get_sortType());
        if(query.exec())
            updateTable();
    }
}

void MainWindow::on_actionWebsite_triggered()
{
    QDesktopServices::openUrl(QUrl("http://to-do-list-manager.computerfr33k.com"));
}

void MainWindow::on_actionCheck_For_Update_triggered()
{
    QMessageBox::information(this, "Checking for updates...", "Coming Soon");
}

void MainWindow::on_actionDonate_triggered()
{
    QDesktopServices::openUrl(QUrl("http://to-do-list-manager.computerfr33k.com/index.php?title=Donate"));
}

void MainWindow::on_actionLicense_triggered()
{
    LicenseDialog *licenseDialog = new LicenseDialog;
    licenseDialog->exec();
    licenseDialog->deleteLater();
}

void MainWindow::trayMenu_triggered(QAction *a)
{
    if(a->text().compare("Show") == 0)
        this->show();
    else if(a->text().compare("Hide") == 0)
        this->hide();
    else if(a->text().compare("Quit") == 0)
        this->close();
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

void MainWindow::on_tableView_doubleClicked(QModelIndex index)
{
    on_editTask_button_clicked();
}

void MainWindow::on_actionRestore_Tasks_Settings_triggered()
{
    QFileDialog fd;
    QString dir = fd.getExistingDirectory(this, tr("Choose Directory To Restore From"), "", QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty())
    {
        QFile::remove(DataLoc.at(0) + "/settings.ini");
        QFile::remove(DataLoc.at(0) + "/tasks.db");
        QFile::copy(dir + "/settings.ini", DataLoc.at(0) + "/settings.ini");
        QFile::copy(dir + "/tasks.db", DataLoc.at(0) + "/tasks.db");
        QMessageBox::information(this, tr("Status"), tr("Restore Complete!"));
        updateTable();
    }
}

void MainWindow::on_actionExport_Tasks_Settings_triggered()
{
    QFileDialog fileDialog;
    fileDialog.setFileMode(QFileDialog::DirectoryOnly);
    QString dir = fileDialog.getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty())
    {
        QFile::remove(dir + "/settings.ini");
        QFile::remove(dir + "/tasks.db");
        if(!QFile::copy(DataLoc.at(0) + "/settings.ini", dir + "/settings.ini"))
        {
            QMessageBox::warning(this, tr("Warning"), tr("settings.ini could not be copied"));
            QFile::remove(dir + "/settings.ini");
        }
        if(!QFile::copy(DataLoc.at(0) + "/tasks.db", dir + "/tasks.db"))
        {
            QMessageBox::warning(this, tr("Warning"), tr("tasks.db could not be copied"));
            QFile::remove(dir + "/tasks.db");
        }
        QMessageBox::information(this, tr("Export"), tr("Export Complete!"));
    }
}

void MainWindow::on_actionHide_triggered()
{
    this->hide();
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
    // create settings.ini file if it doesn't exist
    if(!QFile(DataLoc.at(0) + "/settings.ini").exists())
    {
        settings->setValue("mainWindowGeometry", saveGeometry());
        settings->setValue("mainWindowState", saveState());
        settings->setValue("Sorting/row", 0);
        settings->setValue("Sorting/type", 0);
        settings->setValue("AutoUpdate/enabled", true);
        settings->sync();
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DataLoc.at(0) + "/tasks.db");
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
    ui->tableView->resizeColumnsToContents();
    search_model.setQuery("SELECT id,row,type FROM sort_preferences", db);

    // ini settings test
    if(settings->value("sorting/type", 0).toInt() == 0)
        ui->tableView->sortByColumn(settings->value("sorting/row", 0).toInt(), Qt::AscendingOrder);
    else
        ui->tableView->sortByColumn(settings->value("sorting/row", 0).toInt(), Qt::DescendingOrder);
    //

    /*
     *if(search_model.record(0).value(2).toInt() == 0)
        ui->tableView->sortByColumn(search_model.record(0).value(1).toInt(), Qt::AscendingOrder);
    else if(search_model.record(0).value(2).toInt() == 1)
        ui->tableView->sortByColumn(search_model.record(0).value(1).toInt(), Qt::DescendingOrder);
        */
}

// end private slots

MainWindow::~MainWindow()
{
    delete all_model;
    delete ui;
    delete settings;
    delete tray;
}
