#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <vector>
#include "insertitem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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
    
private:
    Ui::MainWindow *ui;
    QSettings *tasks;

};

#endif // MAINWINDOW_H
