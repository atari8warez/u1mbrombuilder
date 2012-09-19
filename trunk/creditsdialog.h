#ifndef CREDITSDIALOG_H
#define CREDITSDIALOG_H
#include <QtGui/QDialog>

namespace Ui {
    class CreditsDialog;
}

class CreditsDialog : public QDialog {
    Q_OBJECT

public:
    CreditsDialog(QWidget *parent, QString version);
    ~CreditsDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::CreditsDialog *m_ui;

//private slots:
};
#endif // CREDITSDIALOG_H
