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

int Preferences::get_sortRow()
{
    return ui->sortRow->currentIndex();
}

int Preferences::get_sortType()
{
    return ui->sortDirection->currentIndex();
}
