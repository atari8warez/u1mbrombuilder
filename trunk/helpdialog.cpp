#include "helpdialog.h"
#include "ui_helpdialog.h"
#include "u1mbrombsettings.h"

HelpDialog *helpDialog;

HelpDialog::HelpDialog(QWidget *parent, QString version) :
    QDialog(parent),
    m_ui(new Ui::HelpDialog)
{
    helpDialog = this;

    Qt::WindowFlags flags = windowFlags();
    flags = flags & (~Qt::WindowContextHelpButtonHint);
    setWindowFlags(flags);

    m_ui->setupUi(this);
    HelpDialog::setStyleSheet(u1mbrombSettings->backColor(false, 0));
    m_ui->helpBrowser->setStyleSheet(u1mbrombSettings->backColor(false, 0));

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
