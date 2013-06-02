#ifndef LICENSEDIALOG_H
#define LICENSEDIALOG_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui {
class LicenseDialog;
}

class LicenseDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LicenseDialog(QWidget *parent = 0);
    ~LicenseDialog();
    
private:
    Ui::LicenseDialog *ui;
};

#endif // LICENSEDIALOG_H
