#include "helpdialog.h"
#include "ui_helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent, QString version) :
    QDialog(parent),
    m_ui(new Ui::HelpDialog)
{
    Qt::WindowFlags flags = windowFlags();
    flags = flags & (~Qt::WindowContextHelpButtonHint);
    setWindowFlags(flags);

    m_ui->setupUi(this);

}

HelpDialog::~HelpDialog()
{
    delete m_ui;
}

void HelpDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
