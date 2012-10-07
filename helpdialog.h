#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class HelpDialog;
}

class HelpDialog : public QDialog {
    Q_OBJECT

public:
    HelpDialog(QWidget *parent, QString version);
    ~HelpDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::HelpDialog *m_ui;

};

#endif // HELPDIALOG_H
