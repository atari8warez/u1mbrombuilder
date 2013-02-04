#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "u1mbrombsettings.h"

AboutDialog *aboutDialog;

AboutDialog::AboutDialog(QWidget *parent, QString version) :
    QDialog(parent),
    m_ui(new Ui::AboutDialog)
{
    aboutDialog = this;
    Qt::WindowFlags flags = windowFlags();
    flags = flags & (~Qt::WindowContextHelpButtonHint);
    setWindowFlags(flags);

    m_ui->setupUi(this);

    m_ui->versionLabel->setText(tr("Version %1").arg(version));

    AboutDialog::setStyleSheet(u1mbrombSettings->backColor(false, 0));

}

AboutDialog::~AboutDialog()
{
    delete m_ui;
}

void AboutDialog::changeEvent(QEvent *e)
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
