#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>

namespace Ui {
class Preferences;
}

class Preferences : public QDialog
{
    Q_OBJECT
    
public:
    explicit Preferences(QWidget *parent = 0);
    void set_sortRow(int index);
    void set_sortType(int index);
    void set_autoUpdate(bool temp);

    int get_sortRow();
    int get_sortType();
    bool get_autoUpdate();
    ~Preferences();
    
private:
    Ui::Preferences *ui;
};

#endif // PREFERENCES_H
