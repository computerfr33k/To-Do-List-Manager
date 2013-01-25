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
    ~Preferences();

public slots:
    void set_sortRow(int index);
    void set_sortDirection();
    
private:
    Ui::Preferences *ui;
};

#endif // PREFERENCES_H
