#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preferences)
{
    ui->setupUi(this);
}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::set_sortRow(int index)
{
    ui->sortRow->setCurrentIndex(index);
}

void Preferences::set_sortType(int index)
{
    ui->sortDirection->setCurrentIndex(index);
}

void Preferences::set_autoUpdate(bool temp)
{
    int index;
    if(temp)
        index = 0;
    else
        index = 1;

    ui->autoUpdate->setCurrentIndex(index);
}

void Preferences::set_portableMode(bool mode)
{
    ui->portableMode->setChecked(mode);
}

void Preferences::set_dataLoc(QString location)
{
    ui->dataLoc->setText(location);
}

int Preferences::get_sortRow()
{
    return ui->sortRow->currentIndex();
}

int Preferences::get_sortType()
{
    return ui->sortDirection->currentIndex();
}

bool Preferences::get_autoUpdate()
{
    if(ui->autoUpdate->currentIndex() == 0)
        return true;
    else
        return false;
}

bool Preferences::get_portableMode()
{
    //returns true if is portable mode
    return ui->portableMode->isChecked();
}
