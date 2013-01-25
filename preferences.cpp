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

void Preferences::set_sortDirection()
{
}
