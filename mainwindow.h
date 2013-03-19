#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QtWidgets>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSettings>
#include "insertitem.h"
#include "edittask.h"
#include "preferences.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected slots:
        void changeEvent(QEvent *e);
        void closeEvent(QCloseEvent *event);
        void restoreGUI();

protected slots:
    // action Buttons
    void on_actionAbout_triggered();
    void on_actionReport_A_Bug_triggered();
    void on_actionQuit_triggered();
    void on_actionAbout_Qt_2_triggered();
    void on_actionPreferences_triggered();
    void on_actionWebsite_triggered();
    void on_actionCheck_For_Update_triggered();
    void on_actionDonate_triggered();

    //push Buttons
    void on_addTask_button_clicked();
    void on_removeTask_button_clicked();
    void on_editTask_button_clicked();

    //load & save tasks
    void loadTasks();
    void saveTasks();

    //button hooks
    void enableRemoveTask_button();

    //table
    void init_db();
    void updateTable();

    //settings file (settings.ini)
    void readSettings();
    void writeSettings();
    
protected:
    QStringList DataLoc;
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel *all_model;
    QSqlQueryModel search_model;
    QDir dir;
    QSettings *settings;

};

#endif // MAINWINDOW_H
