#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include "insertitem.h"
#include "edittask.h"

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

private slots:
    // action Buttons
    void on_actionAbout_activated();
    void on_actionReport_A_Bug_activated();
    void on_actionQuit_activated();
    void on_actionAbout_Qt_2_activated();

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
    void updateTable();
    
private:
    Ui::MainWindow *ui;
    QSettings *tasks;
    QSqlDatabase db;
    QSqlTableModel *all_model;
    QSqlTableModel *search_model;

};

#endif // MAINWINDOW_H
