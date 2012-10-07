#include "creditsdialog.h"
#include "ui_creditsdialog.h"
#include "u1mbrombsettings.h"

CreditsDialog *creditsDialog;

CreditsDialog::CreditsDialog(QWidget *parent, QString version) :
    QDialog(parent),
    m_ui(new Ui::CreditsDialog)
{
    creditsDialog = this;
    Qt::WindowFlags flags = windowFlags();
    flags = flags & (~Qt::WindowContextHelpButtonHint);
    setWindowFlags(flags);

    m_ui->setupUi(this);
    CreditsDialog::setStyleSheet(u1mbrombSettings->backColor(false, 0));
    m_ui->textBrowser->setStyleSheet(u1mbrombSettings->backColor(false,0));


}

CreditsDialog::~CreditsDialog()
{
    delete m_ui;
}

void CreditsDialog::changeEvent(QEvent *e)
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
