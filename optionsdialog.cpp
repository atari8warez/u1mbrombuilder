#include "optionsdialog.h"
#include "ui_optionsdialog.h"

#include "u1mbrombsettings.h"
#include <QTranslator>
#include <QDir>
#include <QFile>

OptionsDialog *optionsDialog;

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::OptionsDialog)
{
    optionsDialog = this;

    Qt::WindowFlags flags = windowFlags();
    flags = flags & (~Qt::WindowContextHelpButtonHint);
    setWindowFlags(flags);

    m_ui->setupUi(this);

    m_ui->treeWidget->expandAll();
    itemI18n = m_ui->treeWidget->topLevelItem(0);
    m_ui->treeWidget->setCurrentItem(itemI18n);

    connect(this, SIGNAL(accepted()), this, SLOT(OptionsDialog_accepted()));
    connect(m_ui->browseBaseFolder, SIGNAL(clicked()), this, SLOT(browseBasePath()));
    connect(m_ui->browseFolder, SIGNAL(clicked()), this, SLOT(browsePath()));
    connect(m_ui->useDefault, SIGNAL(stateChanged(int)), this, SLOT(check_UseDef()));
    connect(m_ui->useLast, SIGNAL(stateChanged(int)), this, SLOT(check_UseLast()));
    connect(m_ui->useNone, SIGNAL(stateChanged(int)), this, SLOT(check_UseNone()));

    /* Retrieve application settings */
    m_ui->minimizeToTrayBox->setChecked(u1mbrombSettings->minimizeToTray());
    m_ui->saveWinPosBox->setChecked(u1mbrombSettings->saveWindowsPos());
    m_ui->defBaseRomName->setText(u1mbrombSettings->defBaseRomName());
    m_ui->defBaseRomDirName->setText(u1mbrombSettings->defBaseRomDir());
    m_ui->defRomDirName->setText(u1mbrombSettings->defRomDir());
    m_ui->useDefault->setChecked(u1mbrombSettings->useDef());
    m_ui->useLast->setChecked(u1mbrombSettings->useLast());
    m_ui->useNone->setChecked(u1mbrombSettings->useNone());


        optionsDialog->check_UseDef();
        optionsDialog->check_UseLast();
        optionsDialog->check_UseNone();


    /* list available translations */
    QTranslator local_translator;
    m_ui->i18nLaguageCombo->clear();
    m_ui->i18nLaguageCombo->addItem(tr("Automatic"), "auto");
    if (u1mbrombSettings->i18nLanguage().compare("auto") == 0)
      m_ui->i18nLaguageCombo->setCurrentIndex(0);
    m_ui->i18nLaguageCombo->addItem(QT_TR_NOOP("English"), "en");
    if (u1mbrombSettings->i18nLanguage().compare("en") == 0)
      m_ui->i18nLaguageCombo->setCurrentIndex(1);
    QDir dir(":/translations/i18n/");
    QStringList filters;
    filters << "U1MBRomB_*.qm";
    dir.setNameFilters(filters);
    for (int i = 0; i < dir.entryList().size(); ++i) {
        local_translator.load(":/translations/i18n/" + dir.entryList()[i]);
        m_ui->i18nLaguageCombo->addItem(local_translator.translate("OptionsDialog", "English"), dir.entryList()[i].mid(9).replace(".qm", ""));
        if (dir.entryList()[i].mid(9).replace(".qm", "").compare(u1mbrombSettings->i18nLanguage()) == 0) {
            m_ui->i18nLaguageCombo->setCurrentIndex(i+2);
	}
    }
}

OptionsDialog::~OptionsDialog()
{
    delete m_ui;
}

void OptionsDialog::changeEvent(QEvent *e)
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

void OptionsDialog::OptionsDialog_accepted()
{
    u1mbrombSettings->setMinimizeToTray(m_ui->minimizeToTrayBox->isChecked());
    u1mbrombSettings->setSaveWindowsPos(m_ui->saveWinPosBox->isChecked());
    u1mbrombSettings->setI18nLanguage(m_ui->i18nLaguageCombo->itemData(m_ui->i18nLaguageCombo->currentIndex()).toString());
    u1mbrombSettings->setdefBaseRomName(m_ui->defBaseRomName->text());
    u1mbrombSettings->setdefBaseRomDir(m_ui->defBaseRomDirName->text());
    u1mbrombSettings->setdefRomDir(m_ui->defRomDirName->text());
    u1mbrombSettings->setUseDef(m_ui->useDefault->isChecked());
    u1mbrombSettings->setUseLast(m_ui->useLast->isChecked());
    u1mbrombSettings->setUseNone(m_ui->useNone->isChecked());

}

void OptionsDialog::check_UseDef()
{
    if(m_ui->useDefault->isChecked()) {
        m_ui->defRomDirName->setText(m_ui->defBaseRomDirName->text());
        m_ui->defRomDirName->setEnabled(false);
        m_ui->browseFolder->setEnabled(false);
    }
}
void OptionsDialog::check_UseLast()
{
    if(m_ui->useLast->isChecked()) {
        m_ui->defRomDirName->setEnabled(false);
        m_ui->browseFolder->setEnabled(false);
    }
}

void OptionsDialog::check_UseNone()
{
    if(m_ui->useNone->isChecked()) {
        m_ui->defRomDirName->setEnabled(true);
        m_ui->browseFolder->setEnabled(true);
    }
}

void OptionsDialog::on_treeWidget_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* /*previous*/)
{
    if (current == itemI18n) {
        m_ui->stackedWidget->setCurrentIndex(0);
    } else {
        m_ui->stackedWidget->setCurrentIndex(1);
    }
}

void OptionsDialog::on_treeWidget_itemClicked(QTreeWidgetItem* item, int /*column*/)
{

}
void OptionsDialog::browseBasePath()
{
    QString dir;
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Select Directory"), dir);
    if (fileName.isEmpty()) {
        return;
    } else {
        m_ui->defBaseRomDirName->setText(fileName);
    }
}
void OptionsDialog::browsePath()
{
    QString dir;
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Select Directory"), dir);
    if (fileName.isEmpty()) {
        return;
    } else {
        m_ui->defRomDirName->setText(fileName);
    }
}
