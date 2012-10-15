#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "windows.h"
#include "u1mbrombsettings.h"

#include <QScrollBar>
#include <QTranslator>
#include <QMessageBox>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFile>
#include <QUrl>
#include <QString>
#include <QChar>
#include <QDialog>
#include <QDate>
#include <QTime>
#include <iostream>

U1MBRomBSettings *u1mbrombSettings;
MainWindow *mainWindow;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    mainWindow = this;

    /* Set application properties */
    QCoreApplication::setOrganizationName("atari8warez.com");
    QCoreApplication::setApplicationName("U1MBRomB");
    u1mbrombPath = QCoreApplication::applicationDirPath();

    u1mbrombSettings = new U1MBRomBSettings();

    /* Load translators */
    loadTranslators();

    /* Setup UI */
    ui->setupUi(this);

    /* Setup the lookup array */
    lookupI = " ! #$%&'()*+,-./          0123456789:;<=>?abcdefghijklmnopqrstuvwxyz                      ";
    lookupA = " ABCDEFGHIJKLMNO0123456789PQRSTUVWXYZ[\\]^_abcdefghijklmnopqrstuvwxyz!\"#$%&'()*+,-./:;<=>?@";

    lookupI[0] = '\0';
    lookupI[2] = '\"';
    lookupI[16] = '\x10';  // 0
    lookupI[17] = '\x11';  // 1
    lookupI[18] = '\x12';  // 2
    lookupI[19] = '\x13';  // 3
    lookupI[20] = '\x14';  // 4
    lookupI[21] = '\x15';  // 5
    lookupI[22] = '\x16';  // 6
    lookupI[23] = '\x17';  // 7
    lookupI[24] = '\x18';  // 8
    lookupI[25] = '\x19';  // 9
    lookupI[68] = '\x01';  // !
    lookupI[69] = '\x02';  // "
    lookupI[70] = '\x03';  // #
    lookupI[71] = '\x04';  // $
    lookupI[72] = '\x05';  // %
    lookupI[73] = '\x06';  // &
    lookupI[74] = '\x07';  // '
    lookupI[75] = '\x08';  // (
    lookupI[76] = '\x09';  // )
    lookupI[77] = '\x0A';  // *
    lookupI[78] = '\x0B';  // +
    lookupI[79] = '\x0C';  // ,
    lookupI[80] = '\x0D';  // -
    lookupI[81] = '\x0E';  // .
    lookupI[82] = '\x0F';  // /
    lookupI[83] = '\x1A';  // :
    lookupI[84] = '\x1B';  // ;
    lookupI[85] = '\x1C';  // <
    lookupI[86] = '\x1D';  // =
    lookupI[87] = '\x1E';  // >
    lookupI[88] = '\x1F';  // ?
    lookupI[89] = '\x20';  // @

    /* Set Rom Sizes */
    U1MBSize = 524288;
    SDXSize = 262144;
    RSVD1Size = 65536;
    RSVD2Size = 16384;
    RSVD3Size = 32768;
    BIOSSize = 16384;
    BASICSize= 8192;
    XEGSSize = 8192;
    OSSize = 16384;

    /* Setup treeWidget for ROM Selections */
    ui->treeWidget->expandAll();
    itemSDX = ui->treeWidget->topLevelItem(1);
    itemRSVD1 = ui->treeWidget->topLevelItem(3);
    itemBIOS = ui->treeWidget->topLevelItem(5);
    itemRSVD2 = ui->treeWidget->topLevelItem(7);
    itemRSVD3 = ui->treeWidget->topLevelItem(9);
    itemBAS = ui->treeWidget->topLevelItem(11);
    itemBAS1 = ui->treeWidget->topLevelItem(11)->child(0);
    itemBAS2 = ui->treeWidget->topLevelItem(11)->child(1);
    itemBAS3 = ui->treeWidget->topLevelItem(11)->child(2);
    itemBAS4 = ui->treeWidget->topLevelItem(11)->child(3);
    itemXEGS = ui->treeWidget->topLevelItem(12);
    itemXEGS1 = ui->treeWidget->topLevelItem(12)->child(0);
    itemXEGS2 = ui->treeWidget->topLevelItem(12)->child(1);
    itemXEGS3 = ui->treeWidget->topLevelItem(12)->child(2);
    itemXEGS4 = ui->treeWidget->topLevelItem(12)->child(3);
    itemOS = ui->treeWidget->topLevelItem(13);
    itemOS1 = ui->treeWidget->topLevelItem(13)->child(0);
    itemOS2 = ui->treeWidget->topLevelItem(13)->child(1);
    itemOS3 = ui->treeWidget->topLevelItem(13)->child(2);
    itemOS4 = ui->treeWidget->topLevelItem(13)->child(3);
    itemALL = ui->treeWidget->topLevelItem(14);
    ui->treeWidget->setCurrentItem(itemALL);
    ui->treeWidget->setHeaderLabel("");

    itemRSVD1->setText(0, u1mbrombSettings->RSVD1Name());
    ui->descRSVD1->setText(u1mbrombSettings->RSVD1Desc());
    ui->descRSVD1->setEnabled(false);
    itemRSVD2->setText(0, u1mbrombSettings->RSVD2Name());
    ui->descRSVD2->setText(u1mbrombSettings->RSVD2Desc());
    ui->descRSVD2->setEnabled(false);
    itemRSVD3->setText(0, u1mbrombSettings->RSVD3Name());
    ui->descRSVD3->setText(u1mbrombSettings->RSVD3Desc());
    ui->descRSVD3->setEnabled(false);

    // Set Main Window Positions and background style
    mainWindow->setGeometry(u1mbrombSettings->lastHorizontalPos(),u1mbrombSettings->lastVerticalPos(),u1mbrombSettings->lastWidth(),u1mbrombSettings->lastHeight());
    ui->mwFrame->setStyleSheet(u1mbrombSettings->backColor(false, 0));


    // Connect all UI Signals
    trayIcon.setIcon(windowIcon());
    connect(&trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    connect (ui->newU1MB, SIGNAL(clicked()), this, SLOT(on_actionNew_triggered()));
    connect (ui->loadU1MB, SIGNAL(clicked()), this, SLOT(on_actionLoad_triggered()));
    connect (ui->saveU1MB, SIGNAL(clicked()), this, SLOT(on_actionSave_triggered()));
    connect (ui->makeATR, SIGNAL(clicked()), this, SLOT(on_actionMakeATR_triggered()));
    connect (ui->quit, SIGNAL(clicked()), this, SLOT(on_actionExit_triggered()));
    connect (ui->loadROM, SIGNAL(clicked()), this, SLOT(on_actionLoadROM_triggered()));
    connect (ui->saveROM, SIGNAL(clicked()), this, SLOT(on_actionSaveROM_triggered()));
    connect (ui->descRSVD1, SIGNAL(editingFinished()), this, SLOT(rsvdDescChanged()));
    connect (ui->descRSVD2, SIGNAL(editingFinished()), this, SLOT(rsvdDescChanged()));
    connect (ui->descRSVD3, SIGNAL(editingFinished()), this, SLOT(rsvdDescChanged()));
    connect (ui->descBAS1, SIGNAL(editingFinished()), this, SLOT(descChanged()));
    connect (ui->descBAS2, SIGNAL(editingFinished()), this, SLOT(descChanged()));
    connect (ui->descBAS3, SIGNAL(editingFinished()), this, SLOT(descChanged()));
    connect (ui->descBAS4, SIGNAL(editingFinished()), this, SLOT(descChanged()));
    connect (ui->descXEGS1, SIGNAL(editingFinished()), this, SLOT(descChanged()));
    connect (ui->descXEGS2, SIGNAL(editingFinished()), this, SLOT(descChanged()));
    connect (ui->descXEGS3, SIGNAL(editingFinished()), this, SLOT(descChanged()));
    connect (ui->descXEGS4, SIGNAL(editingFinished()), this, SLOT(descChanged()));
    connect (ui->descOS1, SIGNAL(editingFinished()), this, SLOT(descChanged()));
    connect (ui->descOS2, SIGNAL(editingFinished()), this, SLOT(descChanged()));
    connect (ui->descOS3, SIGNAL(editingFinished()), this, SLOT(descChanged()));
    connect (ui->descOS4, SIGNAL(editingFinished()), this, SLOT(descChanged()));
    connect (ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(rsvdChanged()));


    terminalERR = false;
    changesMade  = false;

// Load default U1MB v2 Rom as a base to work with
    mainWindow->loadRom(u1mbrombSettings->defBaseRomDir(), u1mbrombSettings->defBaseRomName(), "default", U1MBSize);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::descChanged()
{
    changesMade = true;
}

// Called by Main to exit the application on terminal errors
bool MainWindow::terminate()
{
    return terminalERR;
}

// Rom size error message display
bool MainWindow::checkSize (int fileSize, int size)
{
    if (fileSize != size) {
       QString fileSizeStr = QString::number(fileSize);
       QString sizeStr = QString::number(size);
       QMessageBox::critical(this, tr("Rom file size error"),
                             tr("Rom size expected to be ") + sizeStr + tr(" bytes. The size of this rom file is ") + fileSizeStr,
                             QMessageBox::Ok);
       return false;
    } else {
       return true;
    }
}

// Message Window indicating changes made to U1MB Rom
int MainWindow::showChangesMade (QString saveMsg)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText (tr("Changes were made to the U1MB Rom"));
    msgBox.setInformativeText (saveMsg);
    msgBox.setStandardButtons (QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int answer = msgBox.exec();
    return answer;
}

// Language translators
void MainWindow::loadTranslators()
{
    qApp->removeTranslator(&u1mbromb_qt_translator);
    qApp->removeTranslator(&u1mbromb_translator);
    if (u1mbrombSettings->i18nLanguage().compare("auto") == 0) {
        QString locale = QLocale::system().name();
        u1mbromb_translator.load(":/translations/i18n/U1MBRomB_" + locale);
        u1mbromb_qt_translator.load(":/translations/i18n/qt_" + locale);
        qApp->installTranslator(&u1mbromb_qt_translator);
        qApp->installTranslator(&u1mbromb_translator);
    } else if (u1mbrombSettings->i18nLanguage().compare("en") != 0) {
        u1mbromb_translator.load(":/translations/i18n/U1MBRomB_" + u1mbrombSettings->i18nLanguage());
        u1mbromb_qt_translator.load(":/translations/i18n/qt_" + u1mbrombSettings->i18nLanguage());
        qApp->installTranslator(&u1mbromb_qt_translator);
        qApp->installTranslator(&u1mbromb_translator);
    }
}

// Message shown the very first time this program is run
void MainWindow::show()
{
    QMainWindow::show();
    /* Open options dialog if it's the first time */
    if (u1mbrombSettings->isFirstTime()) {
        if (QMessageBox::Yes == QMessageBox::question(this, tr("First run"),
                                   tr("You are running U1MBRomB for the first time.\n\nDo you want to open the options dialog?"),
                                   QMessageBox::Yes, QMessageBox::No)) {
            ui->actionOptions->trigger();
            }
    }
}

// System Tray event handler
void MainWindow::hideEvent(QHideEvent *event)
{
    if (u1mbrombSettings->minimizeToTray()) {
        trayIcon.show();
        if(QSystemTrayIcon::supportsMessages() && QSystemTrayIcon::isSystemTrayAvailable ()) trayIcon.showMessage("", "U1MB Rom builder");
        oldWindowFlags = windowFlags();
        oldWindowStates = windowState();
        setWindowFlags(Qt::Widget);
        hide();
        event->ignore();
        return;
    }
    QMainWindow::hideEvent(event);
}

// Exit program event handler
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (changesMade) {
        QString saveMsg = tr("<nobr>Would you like to save these changes before exiting?</nobr>\n<nobr>You can click Cancel to return to editing</nobr>");

        int answer = mainWindow->showChangesMade(saveMsg);
        if (answer == QMessageBox::Yes)  {

            ui->actionSave->trigger();
        }
        else {
            if(answer == QMessageBox::Cancel) event->ignore();
        }
    }
    if (u1mbrombSettings->saveWindowsPos()) {
        u1mbrombSettings->setLastHorizontalPos(mainWindow->geometry().x());
        u1mbrombSettings->setLastVerticalPos(mainWindow->geometry().y());
        u1mbrombSettings->setLastWidth(mainWindow->geometry().width());
        u1mbrombSettings->setLastHeight(mainWindow->geometry().height());
    }
}

// Function to get rom slot descriptions from the U1MB and/or BIOS Rom
QString MainWindow::getDescs(int offset)
{
    QString desc;
    int j=0;
    int k=0;

    for (int i=offset; i<=offset+13; i++ ){
        while ((BIOSdata[i] != lookupI[j]) && j<lookupI.size()) {
            j++;
        }
        desc[k] = lookupA[j];
        k++;
        j=0;
    }
    /* Remove trailing spaces */
    while (desc.endsWith(' ')) desc.chop(1);
    return desc;
}
// Function to put rom slot descriptions to the U1MB and/or BIOS Rom
void MainWindow::putDescs(QString desc, int offset)
{
    int j=0;
    int k=0;

    /* Append trailing spaces */
    for (int i=desc.size(); i<=13; i++) {
        desc.append(" ");
    }
    QByteArray descBA;
    descBA.append(desc);
    for (int i=offset; i<=offset+13; i++ ){
        while ((descBA[k] != lookupA[j]) && j<lookupA.size()) {
            j++;
        }
        BIOSdata[i] = lookupI[j];
        k++;
        j=0;
    }
}

// Populate UI with the slot descriptions from the rom file
void MainWindow::populateDescs(int offset)
{
    ui->descOS1->setText(mainWindow->getDescs(offset));
    offset += 32;
    ui->descOS2->setText(mainWindow->getDescs(offset));
    offset += 32;
    ui->descOS3->setText(mainWindow->getDescs(offset));
    offset += 32;
    ui->descOS4->setText(mainWindow->getDescs(offset));

    offset += 800;
    ui->descBAS1->setText(mainWindow->getDescs(offset));
    offset += 32;
    ui->descBAS2->setText(mainWindow->getDescs(offset));
    offset += 32;
    ui->descBAS3->setText(mainWindow->getDescs(offset));
    offset += 32;
    ui->descBAS4->setText(mainWindow->getDescs(offset));

    offset += 32;
    ui->descXEGS1->setText(mainWindow->getDescs(offset));
    offset += 32;
    ui->descXEGS2->setText(mainWindow->getDescs(offset));
    offset += 32;
    ui->descXEGS3->setText(mainWindow->getDescs(offset));
    offset += 32;
    ui->descXEGS4->setText(mainWindow->getDescs(offset));
}

// Update the slot descriptions from the UI to the rom file
void MainWindow::updateDescs(int offset)
{
    mainWindow->putDescs(ui->descOS1->text(),offset);
    offset += 32;
    mainWindow->putDescs(ui->descOS2->text(),offset);
    offset += 32;
    mainWindow->putDescs(ui->descOS3->text(),offset);
    offset += 32;
    mainWindow->putDescs(ui->descOS4->text(),offset);

    offset += 800;
    mainWindow->putDescs(ui->descBAS1->text(),offset);
    offset += 32;
    mainWindow->putDescs(ui->descBAS2->text(),offset);
    offset += 32;
    mainWindow->putDescs(ui->descBAS3->text(),offset);
    offset += 32;
    mainWindow->putDescs(ui->descBAS4->text(),offset);

    offset += 32;
    mainWindow->putDescs(ui->descXEGS1->text(),offset);
    offset += 32;
    mainWindow->putDescs(ui->descXEGS2->text(),offset);
    offset += 32;
    mainWindow->putDescs(ui->descXEGS3->text(),offset);
    offset += 32;
    mainWindow->putDescs(ui->descXEGS4->text(),offset);
}

// Load a Rom file
void MainWindow::loadRom(QString romDir, QString romName, QString type, int romSize)
{
    QString saveRomName = romName;
    QString saveRomDir = romDir;
    bool repeat;
    bool initError;
    do {
        repeat = false;
        initError = false;
        QFile file(romDir + "/" + romName);
         if (!file.open(QIODevice::ReadOnly)) {
            if (type == "default") {
                initError = true;
                QString msg = tr("\n\nIf you like to select another valid rom file as your default base rom ");
                        msg += tr("please click YES, otherwise click NO and U1MB Rom Builder will then exit.");

                int answer = QMessageBox::critical(this, tr("Unable to open the default base rom file"),
                               tr("Please make sure the default rom file --> ") + saveRomName + tr(" <-- exists in directory:\n")+
                                 QDir::toNativeSeparators(saveRomDir) + msg,
                                 QMessageBox::Yes, QMessageBox::No);

                if (answer == QMessageBox::Yes) {
                    QString fileName = QFileDialog::getOpenFileName(this, tr("Open U1MB default base rom File"), "",
                                         tr("Rom Files (*.rom *.bin);;" "All Files (*)"));
                    if (fileName.isEmpty()) {
                        terminalERR = true;
                        return;
                    }
                    romDir = QFileInfo (fileName).absolutePath() ;
                    QDir::setCurrent(romDir);
                    romName = QFileInfo (fileName).fileName();
                    file.setFileName(romName);

                    if(!file.open(QIODevice::ReadOnly)) {
                        QMessageBox::information(this,"Error Opening File", file.errorString() + " " +fileName,
                          QMessageBox::Ok);
                        repeat = true;
                        romName = "";
                        continue;
                    }
                } else {
                    terminalERR = true;
                    return;
                }
            } else {
                QMessageBox::critical(this, tr("Unable to open ") + type + tr(" Rom file"), file.errorString(), QMessageBox::Ok);
                return;
            }
         }

         romData = file.readAll();

         /* Check for Rom Size */
         if (file.size() != romSize) {
            QString size = QString::number(file.size());
            QString rightSize = QString::number(romSize);

            if (type == "default") {
                QMessageBox::critical(this, tr("Rom file size error while initializing default base rom image"),
                 tr("Expected rom file size for the default base rom is: ") + rightSize + tr(" bytes. The size of --> ") + romName + tr(" <-- is: ") + size +
                 tr(" bytes"),
                  QMessageBox::Ok);
                repeat = true;
                romName = "";
                continue;
            } else {
                QMessageBox::critical(this, type + tr(" Rom file size error"),
                 type + tr(" rom size is expected to be ") + rightSize + tr(" bytes. The size of -->  ") + romName + tr(" <-- is: ") + size + tr(" bytes."),
                  QMessageBox::Ok);
            }
            return;
         }

        /* Check for valid U1MB Rom */
         if ((type == "default" || type == "U1MB") && !(romData[0] == '\x053' && romData[1] == '\x044' && romData[2] == '\x058'))
         {
             if (type == "default") {
                 QMessageBox::critical(this, tr("Rom file content error while initializing default base rom image"),
                  tr("The default U1MB base rom file --> ") + romName + tr(" <-- does not have the expected data content."),
                   QMessageBox::Ok);
                 repeat = true;
                 romName="";
                 continue;
             } else {
                 QMessageBox::critical(this, tr("Rom file content error"),
                  tr("U1MB rom file you are trying to open does not have the expected data content."),
                   QMessageBox::Ok);
                 return;
             }
         }

         if(type == "default" || type == "U1MB") {
             if (type == "default") {
                 U1MBRomFile = tr("untitled.rom");
                 if (initError) {
                     int answer = QMessageBox::question(this, tr("Setting defaults"),
                      tr("Would you like to set the default base rom file name to:\n -->")+  romName +
                      tr(" <--") + tr("\n\nand the default rom directory name to:\n") + romDir,
                       QMessageBox::Yes, QMessageBox::No);

                     if (answer == QMessageBox::Yes) {
                         u1mbrombSettings->setdefBaseRomDir(romDir);
                         u1mbrombSettings->setdefBaseRomName(romName);
                     }
                 }
                 /* Make the base rom file READ-ONLY */
                 QString f = romDir + "/" + romName;
                 std::string fs = f.toStdString();
                 LPCSTR fn = fs.c_str();
                 if(SetFileAttributesA(fn, FILE_ATTRIBUTE_READONLY) == 0)
                     QMessageBox::information(this, romName, "Failed to set the default U1MB Base Rom file to READ-ONLY");

             } else {
                 U1MBRomFile = QFileInfo(file).fileName();
             }
             U1MBRomFilePath = QFileInfo(file).absolutePath();

             /* SPLIT U1MB ROM TO INDIVIDUAL ROMS */
             SDXdata = romData.mid(0,262144);
             RSVD1data = romData.mid(262144, 65536);
             BIOSdata = romData.mid(327680, 16384);
             RSVD2data = romData.mid(344064, 16384);
             RSVD3data = romData.mid(360448, 32768);
             BAS1data = romData.mid(393216, 8192);
             BAS2data = romData.mid(401408, 8192);
             BAS3data = romData.mid(409600, 8192);
             BAS4data = romData.mid(417792, 8192);
             XEGS1data = romData.mid(425984, 8192);
             XEGS2data = romData.mid(434176, 8192);
             XEGS3data = romData.mid(442368, 8192);
             XEGS4data = romData.mid(450560, 8192);
             OS1data = romData.mid(458752, 16384);
             OS2data = romData.mid(475136, 16384);
             OS3data = romData.mid(491520, 16384);
             OS4data = romData.mid(507904, 16384);

             mainWindow->populateDescs(8338);
             appWindowTitle = u1mbrombSettings->mainWindowTitle() + " -- " + U1MBRomFile;
             mainWindow->setRomDescs();
             mainWindow->resetRomPaths();
             mainWindow->resetRomFileName();
             changesMade = false;

         } else {
             if(type == "SDX") {
                 if (!(romData[0] == '\x053' && romData[1] == '\x044' && romData[2] == '\x058')) {
                     QMessageBox::critical(this, tr("Rom file content error"), tr("SDX rom file you are trying to open does not have the expected data content."),
                                           QMessageBox::Ok);
                     return;
                 }
                 SDXdata = romData;
             }
             if(type == "RSVD1") RSVD1data = romData;
             if(type == "RSVD2") RSVD2data = romData;
             if(type == "RSVD3") RSVD3data = romData;
             if(type == "BIOS") {
                 if (!(romData[8192] == '\x025' && romData[8224] == '\x025' && romData[8256] == '\x025' && romData[8448] == '\x033')) {
                     QMessageBox::critical(this, tr("Rom file content error"), tr("BIOS rom file you are trying to open does not have the expected data content."),
                                           QMessageBox::Ok);
                     return;
                 }
                 BIOSdata = romData;
                 QString msg = tr("You are loading a new BIOS ");
                 msg += tr("Rom into U1MB. Current descriptions for BASIC, GAME and OS ");
                 msg += tr("slots will be overridden by the descriptions from the BIOS ");
                 msg += tr("Rom you are loading.\n\n");
                 msg += tr("Click YES to override, NO to keep the current descriptions. ");

                 int answer = QMessageBox::question(this, tr("Confirmation needed..."), msg,
                      QMessageBox::Yes, QMessageBox::No);
                 if (answer == QMessageBox::Yes) {
                     mainWindow->populateDescs(8338);
                 }
             }
             if(type == "BAS1") BAS1data = romData;
             if(type == "BAS2") BAS2data = romData;
             if(type == "BAS3") BAS3data = romData;
             if(type == "BAS4") BAS4data = romData;
             if(type == "XEGS1") XEGS1data = romData;
             if(type == "XEGS2") XEGS2data = romData;
             if(type == "XEGS3") XEGS3data = romData;
             if(type == "XEGS4") XEGS4data = romData;
             if(type == "OS1") OS1data = romData;
             if(type == "OS2") OS2data = romData;
             if(type == "OS3") OS3data = romData;
             if(type == "OS4") OS4data = romData;

             mainWindow->setRomValues(type, QFileInfo(file).fileName(), QFileInfo(file).absolutePath());
             changesMade = true;
         }
         mainWindow->setWindowTitle(appWindowTitle);
         romData.clear();
         ui->treeWidget->setCurrentItem(itemALL);
    }
    while (repeat);
}

// Save a Rom file
void MainWindow::saveRom(QString romDir, QString romName, QString type)
{
    QFile file(romDir + "/" + romName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(this, tr("Unable to open file"), file.errorString());
        return;
    }

    /* Save U1MB Rom */
    if(type == "U1MB") {
        mainWindow->updateDescs(8338);

        /* Reassemble U1MB Rom */
        mainWindow->reassembleRom();
        U1MBRomFile = QFileInfo(file).fileName();
        U1MBRomFilePath = QFileInfo(file).absolutePath();
        appWindowTitle = u1mbrombSettings->mainWindowTitle() + (" -- ") + U1MBRomFile;
        mainWindow->setWindowTitle(appWindowTitle);
        changesMade = false;

    /* Save individual Rom */
    } else {
        if(type == "SDX") romData = SDXdata;
            mainWindow->setRomValues(type, romName, romDir);
        if(type ==  "RSVD1") romData = RSVD1data;
        if(type ==  "RSVD2") romData = RSVD2data;
        if(type ==  "RSVD3") romData = RSVD3data;
        if(type == "BIOS") {
            mainWindow->updateDescs(8338);
            romData = BIOSdata;
        }
        if(type == "BAS1") romData = BAS1data;
        if(type == "BAS2") romData = BAS2data;
        if(type == "BAS3") romData = BAS3data;
        if(type == "BAS4") romData = BAS4data;
        if(type == "XEGS1") romData = XEGS1data;
        if(type == "XEGS2") romData = XEGS2data;
        if(type == "XEGS3") romData = XEGS3data;
        if(type == "XEGS4") romData = XEGS4data;
        if(type == "OS1") romData = OS1data;
        if(type == "OS2") romData = OS2data;
        if(type == "OS3") romData = OS3data;
        if(type == "OS4") romData = OS4data;
        mainWindow->setRomValues(type, romName, romDir);
        }
    file.write(romData);
    romData.clear();
}
// Menu item File/New U1MB Rom
void MainWindow::on_actionNew_triggered()
{
    if (changesMade) {
        QString saveMsg = tr("<nobr>Would you like to save these changes before starting a new file?</nobr>\n<nobr>You can click Cancel to return to editing</nobr>");

        int answer = mainWindow->showChangesMade(saveMsg);
        if (answer == QMessageBox::Yes)  {

            if(!mainWindow-> on_actionSave_triggered()) return;
        } else {
            if (answer == QMessageBox::Cancel) return;
        }
    }
    appWindowTitle = u1mbrombSettings->mainWindowTitle() + tr(" -- untitled.rom ");
    mainWindow->setWindowTitle(appWindowTitle);
    U1MBRomFile = "untitled.rom";
    U1MBRomFilePath.clear();
    changesMade = false;
    mainWindow->loadRom(u1mbrombSettings->defBaseRomDir(), u1mbrombSettings->defBaseRomName(), "default", U1MBSize);
    if(terminalERR) QApplication::exit();
}
// Menu item File/Load U1MB Rom
void MainWindow::on_actionLoad_triggered()
{
    if (changesMade) {
        QString saveMsg = tr("<nobr>Would you like to save these changes before loading a new file?</nobr>\n<nobr>You can click Cancel to return to editing</nobr>");

        int answer = mainWindow->showChangesMade(saveMsg);
        if (answer == QMessageBox::Yes)  {

            if(!mainWindow->on_actionSave_triggered()) return;
        }
        else {
            if(answer == QMessageBox::Cancel) return;
        }
    }
    QString dir = u1mbrombSettings->defRomDir();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open U1MB Rom File"), dir ,tr("Rom Files (*.rom *.bin);;" "All Files (*)"));

    if (fileName.isEmpty()) {
        return;
    }
    if(u1mbrombSettings->useLast()) u1mbrombSettings->setdefRomDir(QFileInfo(fileName).absolutePath());
    mainWindow->loadRom(QFileInfo (fileName).absolutePath(), QFileInfo(fileName).fileName(), "U1MB", U1MBSize);
}

// Menu item File/Save U1MB Rom
// returns the boolean values true/false indicating that the save dialog was cancelled using the Cancel button

bool MainWindow::on_actionSave_triggered()
{
    QString dir = u1mbrombSettings->defRomDir();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Rom as"), dir + "/" + U1MBRomFile, tr("Rom Files  (*.rom *.bin);;" "All files (*)"));
    if (fileName.isEmpty()) {
        return false;
    }
    if(u1mbrombSettings->useLast()) u1mbrombSettings->setdefRomDir(QFileInfo(fileName).absolutePath());
    mainWindow->saveRom(QFileInfo(fileName).absolutePath(), QFileInfo(fileName).fileName(), "U1MB");
    return true;
}


void MainWindow::on_actionMakeATR_triggered()
{
    // Build the ATR image
    QByteArray atrData, flasherData;

    QFile file(u1mbrombPath + "/u1m.dat");
     if (!file.open(QIODevice::ReadOnly)) {
         QMessageBox::critical(this, tr("Missing application component!"),
            tr("The following file is missing from the application's installation directory: ") + file.fileName(),
           QMessageBox::Ok);
         return;
     }
     atrData = file.readAll();
     file.close();

     QString dir = u1mbrombSettings->defRomDir();
     QString fileName = QFileDialog::getOpenFileName(this, tr("Select the flasher executable"), dir,
                          tr("Atari Executable Files (*.com *.exe *.xex);;" "All Files (*)"));
     if (fileName.isEmpty()) {
         return;
     }
     if(u1mbrombSettings->useLast()) u1mbrombSettings->setdefRomDir(QFileInfo(fileName).absolutePath());
     file.setFileName(fileName);
     if(!file.open(QIODevice::ReadOnly)) {
         QMessageBox::critical(this, tr("Error Opening File"), file.errorString() + " " + fileName,
           QMessageBox::Ok);
         return;
     }
     if (file.size() > 8192) {
         QMessageBox::critical(this, tr("Flasher file size error"), tr("Flasher file size cannot be > 8K (8192 bytes)"),
           QMessageBox::Ok);
         return;
     }
     flasherData = file.readAll();
     file.close();

     // Check for valid executable
     if (flasherData.at(0) != '\xFF' || flasherData.at(1) != '\xFF') {
         QMessageBox::critical(this, tr("Invalid Executable"), fileName + tr("  is not a valid ATARI executable"),
           QMessageBox::Ok);
         return;
     }
     // Update the size of the file

     int loByte, hiByte, sectors, offset;
     hiByte = file.size() / 256;
     loByte = file.size() % 256;

     offset = 1194;
     atrData[offset] = loByte;
     atrData[offset+1] = hiByte;

     // Determine how many sectors (256 bytes each) does the flasher occupy (max flasher size is 8192 bytes)
     // We will only need one Sector Map Sector at sector 32
     sectors = hiByte;
     if (loByte != 0) sectors += 1;

     // Initialize and fill-in the Sector Map Sectors for the flasher
     offset = 7568;
     for (int i=0; i<256; i++) atrData[offset+i] = '\x00';
     offset = 7572;
     int smEntries = sectors;
     int j = 0;
     int nextDataSect = 33;
     for (int i=0; i<smEntries; i++ ){
         atrData[offset+j] = nextDataSect+i;
         atrData[offset+j+1] = '\x00';
         j += 2;
     }
     // Insert flasher file data
     offset = 7824;
     for (int i=0; i<8192; i++) atrData[offset+i] = '\x00';
     atrData.replace(offset, flasherData.size(), flasherData);

     // U1MB Rom will occupy 2048 sectors (256 bytes each), and we'll need 17 sector map sectors (65-81)
     // U1MB Rom data will start from sector 82
     int smSectors = 17;
     int dataSectors = 2048;
     int totalEntries = 0;
     int nextSmSect = 66;
     int prevSmSect = 0;
     nextDataSect = 82;
     offset = 16016;
     smEntries = 126;

     // Fill-in Sector Map Sectors starting from sector 65
     // For all SMS
     for (int i=0; i<4352; i++) atrData[offset+i] = '\x00';
     for (int i=0; i<smSectors; i++) {
         atrData[offset] = nextSmSect;
         atrData[offset+2] = prevSmSect;
         offset += 4;
         int j = 0;
         // For each SMS
         for (int k=0; k<smEntries; k++ ){
             hiByte = nextDataSect / 256;
             loByte = nextDataSect % 256;
             atrData[offset+j] = loByte;
             atrData[offset+j+1] = hiByte;
             j += 2;
             nextDataSect += 1;
             totalEntries += 1;
             if (totalEntries == dataSectors) break;
         }
         if (totalEntries == dataSectors) break;
         prevSmSect = nextSmSect - 1;
         nextSmSect +=1;
         offset += 252;
      }
     // Update next SMS # info on the last SMS sector to zero to indicate end-of-chain
     offset -= 4;
     atrData[offset] = '\x00';
     atrData[offset+1] = '\x00';

     // Insert U1MB Rom data
     offset += 256;
     mainWindow->reassembleRom();
     atrData.insert(offset, romData);

     QDate date = QDate::currentDate();
     QTime time = QTime::currentTime();
     int year = date.year();
     year = year - 2000;

     // Update the date and time of the FLASH.COM
     atrData[1208] = date.day();
     atrData[1209] = date.month();
     atrData[1210] = year;

     atrData[1211] = time.hour();
     atrData[1212] = time.minute();
     atrData[1213] = time.second();

     // Update the date and time of the U1M.ROM
     atrData[1231] = date.day();
     atrData[1232] = date.month();
     atrData[1233] = year;

     atrData[1234] = time.hour();
     atrData[1235] = time.minute();
     atrData[1236] = time.second();

     // Write the .atr file
     dir = u1mbrombSettings->defRomDir();
     fileName = QFileDialog::getSaveFileName(this, tr("Save ATR as"), dir + "/U1MFlash", tr("Atari Disk Image Files  (*.atr);;" "All files (*)"));
     if (fileName.isEmpty()) {
         return;
     }
     if(u1mbrombSettings->useLast()) u1mbrombSettings->setdefRomDir(QFileInfo(fileName).absolutePath());
     QFile atrfile(fileName);
     if (!atrfile.open(QIODevice::WriteOnly)) {
         QMessageBox::critical(this, tr("Unable to open file"), atrfile.errorString());
         return;
     }
     offset = atrData.size()+1;
     for (int i=offset; i<736912; i++) atrData.insert(i, '\x00');
     atrfile.write(atrData);
     atrData.clear();
     romData.clear();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionOptions_triggered()
{
    OptionsDialog optionsDialog(this);
    optionsDialog.exec() ;

    if (optionsDialog.result() == optionsDialog.Accepted){
        ui->mwFrame->setStyleSheet(u1mbrombSettings->backColor(false, 0));
    }

    loadTranslators();
    ui->retranslateUi(this);

}

void MainWindow::on_actionHelp_triggered()
{
    HelpDialog helpDialog(this, VERSION);
    helpDialog.exec() ;

}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog aboutDialog(this, VERSION);
    aboutDialog.exec();
}

void MainWindow::on_actionCredits_triggered()
{
    CreditsDialog creditsDialog(this, VERSION);
    creditsDialog.exec();
}

// Load ROM button to load individual ROMS into ROM slots
void MainWindow::on_actionLoadROM_triggered()
{
    QString rom;
    int size = 0;

    if (itemSDX->isSelected()) {
        rom = "SDX";
        size = SDXSize;
    }
    if (itemRSVD1->isSelected()) {
        rom = "RSVD1";
        size = RSVD1Size;
    }
    if (itemRSVD2->isSelected()) {
        rom = "RSVD2";
        size = RSVD2Size;
    }
    if (itemRSVD3->isSelected()) {
        rom = "RSVD3";
        size = RSVD3Size;
    }
    if (itemBIOS->isSelected()) {
        rom = "BIOS";
        size = BIOSSize;
    }
    if (itemBAS1->isSelected()) {
        rom = "BAS1";
        size = BASICSize;
    }
    if (itemBAS2->isSelected()) {
        rom = "BAS2";
        size = BASICSize;
    }
    if (itemBAS3->isSelected()) {
        rom = "BAS3";
        size = BASICSize;
    }
    if (itemBAS4->isSelected()) {
        rom = "BAS4";
        size = BASICSize;
    }
    if (itemXEGS1->isSelected()) {
        rom = "XEGS1";
        size = XEGSSize;
    }
    if (itemXEGS2->isSelected()) {
        rom = "XEGS2";
        size = XEGSSize;
    }
    if (itemXEGS3->isSelected()) {
        rom = "XEGS3";
        size = XEGSSize;
    }
    if (itemXEGS4->isSelected()) {
        rom = "XEGS4";
        size = XEGSSize;
    }
    if (itemOS1->isSelected()) {
        rom = "OS1";
        size = OSSize;
    }
    if (itemOS2->isSelected()) {
        rom = "OS2";
        size = OSSize;
    }
    if (itemOS3->isSelected()) {
        rom = "OS3";
        size = OSSize;
    }
    if (itemOS4->isSelected()) {
        rom = "OS4";
        size = OSSize;
    }

    QString fileName;
    QString dir = u1mbrombSettings->defRomDir();

    fileName = QFileDialog::getOpenFileName(this, tr("Open Rom File"), dir ,tr("Rom Files (*.rom *.bin);;" "All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }
    if(u1mbrombSettings->useLast()) u1mbrombSettings->setdefRomDir(QFileInfo(fileName).absolutePath());
    mainWindow->loadRom(QFileInfo (fileName).absolutePath(), QFileInfo(fileName).fileName(), rom, size);
}

// Extract ROM button to save individual ROMS from ROM slots
void MainWindow::on_actionSaveROM_triggered()
{
    QString file;
    QString rom;

    if(itemSDX->isSelected()){
        if(SDXRomFile.isEmpty()) SDXRomFile = "U1MBsdx.rom";
        file = SDXRomFile;
        rom = "SDX";
    }
    if(itemRSVD1->isSelected()){
        if(RSVD1RomFile.isEmpty()) RSVD1RomFile = "U1MBrsvd1.rom";
        file = RSVD1RomFile;
        rom = "RSVD1";
    }
    if(itemRSVD2->isSelected()){
        if(RSVD2RomFile.isEmpty()) RSVD2RomFile = "U1MBrsvd2.rom";
        file = RSVD2RomFile;
        rom = "RSVD2";
    }
    if(itemRSVD3->isSelected()){
        if(RSVD3RomFile.isEmpty()) RSVD3RomFile = "U1MBrsvd3.rom";
        file = RSVD3RomFile;
        rom = "RSVD3";
    }
    if(itemBIOS->isSelected()){
        if(BIOSRomFile.isEmpty()) BIOSRomFile = "U1MBbios.rom";
        file = BIOSRomFile;
        rom = "BIOS";
    }
    if(itemBAS1->isSelected()){
        if(BAS1RomFile.isEmpty()) BAS1RomFile = "U1MBbasic1.rom";
        file = BAS1RomFile;
        rom = "BAS1";
    }
    if(itemBAS2->isSelected()){
        if(BAS2RomFile.isEmpty()) BAS2RomFile = "U1MBbasic2.rom";
        file = BAS2RomFile;
        rom = "BAS2";
    }
    if(itemBAS3->isSelected()){
        if(BAS3RomFile.isEmpty()) BAS3RomFile = "U1MBbasic3.rom";
        file = BAS3RomFile;
        rom = "BAS3";
    }
    if(itemBAS4->isSelected()){
        if(BAS4RomFile.isEmpty()) BAS4RomFile = "U1MBbasic4.rom";
        file = BAS4RomFile;
        rom = "BAS4";
    }
    if(itemXEGS1->isSelected()){
        if(XEGS1RomFile.isEmpty()) XEGS1RomFile = "U1MBxegs1.rom";
        file = XEGS1RomFile;
        rom = "XEGS1";
    }
    if(itemXEGS2->isSelected()){
        if(XEGS2RomFile.isEmpty()) XEGS2RomFile = "U1MBxegs2.rom";
        file = XEGS2RomFile;
        rom = "XEGS2";
    }
    if(itemXEGS3->isSelected()){
        if(XEGS3RomFile.isEmpty()) XEGS3RomFile = "U1MBxegs3.rom";
        file = XEGS3RomFile;
        rom = "XEGS3";
    }
    if(itemXEGS4->isSelected()){
        if(XEGS4RomFile.isEmpty()) XEGS4RomFile = "U1MBxegs4.rom";
        file = XEGS4RomFile;
        rom = "XEGS4";
    }
    if(itemOS1->isSelected()){
        if(OS1RomFile.isEmpty()) OS1RomFile = "U1MBOs1.rom";
        file = OS1RomFile;
        rom = "OS1";
    }
    if(itemOS2->isSelected()){
        if(OS2RomFile.isEmpty()) OS2RomFile = "U1MBOs2.rom";
        file = OS2RomFile;
        rom = "OS2";
    }
    if(itemOS3->isSelected()){
        if(OS3RomFile.isEmpty()) OS3RomFile = "U1MBOs3.rom";
        file = OS3RomFile;
        rom = "OS3";
    }
    if(itemOS4->isSelected()){
        if(OS4RomFile.isEmpty()) OS4RomFile = "U1MBOs4.rom";
        file = OS4RomFile;
        rom = "OS4";
    }

    QString fileName;
    QString dir = u1mbrombSettings->defRomDir();

    fileName = QFileDialog::getSaveFileName(this, tr("Save Rom as"), dir + "/" + file, tr("Rom Files  (*.rom *.bin);;" "All files (*)"));
    if (fileName.isEmpty()) {
        return;
    }
    if(u1mbrombSettings->useLast()) u1mbrombSettings->setdefRomDir(QFileInfo(fileName).absolutePath());
    mainWindow->saveRom(QFileInfo(fileName).absolutePath(), QFileInfo(fileName).fileName(), rom);

}
void MainWindow::setRomValues(QString rom, QString fileName, QString path)
{
    if(rom == "SDX") {
        SDXRomFile = fileName;
        SDXRomFilePath = path;
        ui->pathSDX->setText(SDXRomFile);
        ui->pathSDX->setToolTip(QDir::toNativeSeparators(SDXRomFilePath));
        ui->pathSDXa->setText(ui->pathSDX->text());
        ui->pathSDXa->setToolTip(ui->pathSDX->toolTip());\
    }
    if(rom ==  "RSVD1") {
        RSVD1RomFile = fileName;
        RSVD1RomFilePath = path;
        ui->pathRSVD1->setText(RSVD1RomFile);
        ui->pathRSVD1->setToolTip(QDir::toNativeSeparators(RSVD1RomFilePath));
        ui->pathRSVD1a->setText(ui->pathRSVD1->text());
        ui->pathRSVD1a->setToolTip(ui->pathRSVD1->toolTip());
    }
    if(rom ==  "RSVD2") {
        RSVD2RomFile = fileName;
        RSVD2RomFilePath = path;
        ui->pathRSVD2->setText(RSVD2RomFile);
        ui->pathRSVD2->setToolTip(QDir::toNativeSeparators(RSVD2RomFilePath));
        ui->pathRSVD2a->setText(ui->pathRSVD2->text());
        ui->pathRSVD2a->setToolTip(ui->pathRSVD2->toolTip());
    }
    if(rom ==  "RSVD3") {
        RSVD3RomFile = fileName;
        RSVD3RomFilePath = path;
        ui->pathRSVD3->setText(RSVD3RomFile);
        ui->pathRSVD3->setToolTip(QDir::toNativeSeparators(RSVD3RomFilePath));
        ui->pathRSVD3a->setText(ui->pathRSVD3->text());
        ui->pathRSVD3a->setToolTip(ui->pathRSVD3->toolTip());
    }
    if(rom == "BIOS") {
        BIOSRomFile = fileName;
        BIOSRomFilePath = path;
        ui->pathBIOS->setText(BIOSRomFile);
        ui->pathBIOS->setToolTip(QDir::toNativeSeparators(BIOSRomFilePath));
        ui->pathBIOSa->setText(ui->pathBIOS->text());
        ui->pathBIOSa->setToolTip(ui->pathBIOS->toolTip());
    }
    if(rom == "BAS1") {
        BAS1RomFile = fileName;
        BAS1RomFilePath = path;
        ui->pathBAS1->setText(BAS1RomFile);
        ui->pathBAS1->setToolTip(QDir::toNativeSeparators(BAS1RomFilePath));
        ui->pathBAS1a->setText(ui->pathBAS1->text());
        ui->pathBAS1a->setToolTip(ui->pathBAS1->toolTip());
    }
    if(rom == "BAS2") {
        BAS2RomFile = fileName;
        BAS2RomFilePath = path;
        ui->pathBAS2->setText(BAS2RomFile);
        ui->pathBAS2->setToolTip(QDir::toNativeSeparators(BAS2RomFilePath));
        ui->pathBAS2a->setText(ui->pathBAS2->text());
        ui->pathBAS2a->setToolTip(ui->pathBAS2->toolTip());
    }
    if(rom == "BAS3") {
        BAS3RomFile = fileName;
        BAS3RomFilePath = path;
        ui->pathBAS3->setText(BAS3RomFile);
        ui->pathBAS3->setToolTip(QDir::toNativeSeparators(BAS3RomFilePath));
        ui->pathBAS3a->setText(ui->pathBAS3->text());
        ui->pathBAS2a->setToolTip(ui->pathBAS3->toolTip());
    }
    if(rom == "BAS4") {
        BAS4RomFile = fileName;
        BAS4RomFilePath = path;
        ui->pathBAS4->setText(BAS4RomFile);
        ui->pathBAS4->setToolTip(QDir::toNativeSeparators(BAS4RomFilePath));
        ui->pathBAS4a->setText(ui->pathBAS4->text());
        ui->pathBAS4a->setToolTip(ui->pathBAS4->toolTip());
    }
    if(rom == "XEGS1") {
        XEGS1RomFile = fileName;
        XEGS1RomFilePath = path;
        ui->pathXEGS1->setText(XEGS1RomFile);
        ui->pathXEGS1->setToolTip(QDir::toNativeSeparators(XEGS1RomFilePath));
        ui->pathXEGS1a->setText(ui->pathXEGS1->text());
        ui->pathXEGS1a->setToolTip(ui->pathXEGS1->toolTip());
    }
    if(rom == "XEGS2") {
        XEGS2RomFile = fileName;
        XEGS2RomFilePath = path;
        ui->pathXEGS2->setText(XEGS2RomFile);
        ui->pathXEGS2->setToolTip(QDir::toNativeSeparators(XEGS2RomFilePath));
        ui->pathXEGS2a->setText(ui->pathXEGS2->text());
        ui->pathXEGS2a->setToolTip(ui->pathXEGS2->toolTip());
    }
    if(rom == "XEGS3") {
        XEGS3RomFile = fileName;
        XEGS3RomFilePath = path;
        ui->pathXEGS3->setText(XEGS3RomFile);
        ui->pathXEGS3->setToolTip(QDir::toNativeSeparators(XEGS3RomFilePath));
        ui->pathXEGS3a->setText(ui->pathXEGS3->text());
        ui->pathBAS2a->setToolTip(ui->pathXEGS3->toolTip());
    }
    if(rom == "XEGS4") {
        XEGS4RomFile = fileName;
        XEGS4RomFilePath = path;
        ui->pathXEGS4->setText(XEGS4RomFile);
        ui->pathXEGS4->setToolTip(QDir::toNativeSeparators(XEGS4RomFilePath));
        ui->pathXEGS4a->setText(ui->pathXEGS4->text());
        ui->pathXEGS4a->setToolTip(ui->pathXEGS4->toolTip());
    }
    if(rom == "OS1") {
        OS1RomFile = fileName;
        OS1RomFilePath = path;
        ui->pathOS1->setText(OS1RomFile);
        ui->pathOS1->setToolTip(QDir::toNativeSeparators(OS1RomFilePath));
        ui->pathOS1a->setText(ui->pathOS1->text());
        ui->pathOS1a->setToolTip(ui->pathOS1->toolTip());
    }
    if(rom == "OS2") {
        OS2RomFile = fileName;
        OS2RomFilePath = path;
        ui->pathOS2->setText(OS2RomFile);
        ui->pathOS2->setToolTip(QDir::toNativeSeparators(OS2RomFilePath));
        ui->pathOS2a->setText(ui->pathOS2->text());
        ui->pathOS2a->setToolTip(ui->pathOS2->toolTip());
    }
    if(rom == "OS3") {
        OS3RomFile = fileName;
        OS3RomFilePath = path;
        ui->pathOS3->setText(OS3RomFile);
        ui->pathOS3->setToolTip(QDir::toNativeSeparators(OS3RomFilePath));
        ui->pathOS3a->setText(ui->pathOS3->text());
        ui->pathBAS2a->setToolTip(ui->pathOS3->toolTip());
    }
    if(rom == "OS4") {
        OS4RomFile = fileName;
        OS4RomFilePath = path;
        ui->pathOS4->setText(OS4RomFile);
        ui->pathOS4->setToolTip(QDir::toNativeSeparators(OS4RomFilePath));
        ui->pathOS4a->setText(ui->pathOS4->text());
        ui->pathOS4a->setToolTip(ui->pathOS4->toolTip());
    }

}

void MainWindow::setRomDescs() {

    ui->descRSVD1a->setText(ui->descRSVD1->text());
    ui->descRSVD2a->setText(ui->descRSVD2->text());
    ui->descRSVD3a->setText(ui->descRSVD3->text());
    ui->descBAS1a->setText(ui->descBAS1->text());
    ui->descBAS2a->setText(ui->descBAS2->text());
    ui->descBAS3a->setText(ui->descBAS3->text());
    ui->descBAS4a->setText(ui->descBAS4->text());
    ui->descXEGS1a->setText(ui->descXEGS1->text());
    ui->descXEGS2a->setText(ui->descXEGS2->text());
    ui->descXEGS3a->setText(ui->descXEGS3->text());
    ui->descXEGS4a->setText(ui->descXEGS4->text());
    ui->descOS1a->setText(ui->descOS1->text());
    ui->descOS2a->setText(ui->descOS2->text());
    ui->descOS3a->setText(ui->descOS3->text());
    ui->descOS4a->setText(ui->descOS4->text());
}

void MainWindow::resetRomPaths() {

    ui->pathSDX->setText(NULL);
    ui->pathRSVD1->setText(NULL);
    ui->pathRSVD2->setText(NULL);
    ui->pathRSVD3->setText(NULL);
    ui->pathBIOS->setText(NULL);
    ui->pathBAS1->setText(NULL);
    ui->pathBAS2->setText(NULL);
    ui->pathBAS3->setText(NULL);
    ui->pathBAS4->setText(NULL);
    ui->pathXEGS1->setText(NULL);
    ui->pathXEGS2->setText(NULL);
    ui->pathXEGS3->setText(NULL);
    ui->pathXEGS4->setText(NULL);
    ui->pathOS1->setText(NULL);
    ui->pathOS2->setText(NULL);
    ui->pathOS3->setText(NULL);
    ui->pathOS4->setText(NULL);

    ui->pathSDXa->setText(NULL);
    ui->pathRSVD1a->setText(NULL);
    ui->pathRSVD2a->setText(NULL);
    ui->pathRSVD3a->setText(NULL);
    ui->pathBIOSa->setText(NULL);
    ui->pathBAS1a->setText(NULL);
    ui->pathBAS2a->setText(NULL);
    ui->pathBAS3a->setText(NULL);
    ui->pathBAS4a->setText(NULL);
    ui->pathXEGS1a->setText(NULL);
    ui->pathXEGS2a->setText(NULL);
    ui->pathXEGS3a->setText(NULL);
    ui->pathXEGS4a->setText(NULL);
    ui->pathOS1a->setText(NULL);
    ui->pathOS2a->setText(NULL);
    ui->pathOS3a->setText(NULL);
    ui->pathOS4a->setText(NULL);

}

void MainWindow::resetRomFileName()
{
    SDXRomFile = "";
    RSVD1RomFile = "";
    RSVD2RomFile = "";
    RSVD3RomFile = "";
    BIOSRomFile = "";
    BAS1RomFile = "";
    BAS2RomFile = "";
    BAS3RomFile = "";
    BAS4RomFile = "";
    XEGS1RomFile = "";
    XEGS2RomFile = "";
    XEGS3RomFile = "";
    XEGS4RomFile = "";
    OS1RomFile = "";
    OS2RomFile = "";
    OS3RomFile = "";
    OS4RomFile = "";
}
void MainWindow::reassembleRom()
{
    romData = SDXdata;
    romData.append(RSVD1data);
    romData.append(BIOSdata);
    romData.append(RSVD2data);
    romData.append(RSVD3data);
    romData.append(BAS1data);
    romData.append(BAS2data);
    romData.append(BAS3data);
    romData.append(BAS4data);
    romData.append(XEGS1data);
    romData.append(XEGS2data);
    romData.append(XEGS3data);
    romData.append(XEGS4data);
    romData.append(OS1data);
    romData.append(OS2data);
    romData.append(OS3data);
    romData.append(OS4data);
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick) {
        setWindowFlags(oldWindowFlags);
        setWindowState(oldWindowStates);
        show();
        activateWindow();
        raise();
        trayIcon.hide();
    }
}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* /*previous*/)
{
    ui->loadROM->setEnabled(true);
    ui->saveROM->setEnabled(true);

// Single Slots
    if (current == itemSDX) ui->romDescriptions->setCurrentIndex(0);
    if (current == itemRSVD1) ui->romDescriptions->setCurrentIndex(1);
    if (current == itemBIOS) ui->romDescriptions->setCurrentIndex(2);
    if (current ==  itemRSVD2) ui->romDescriptions->setCurrentIndex(3);
    if (current == itemRSVD3)ui->romDescriptions->setCurrentIndex(4);

// Basic Group
    if (current == itemBAS1) {
        ui->romDescriptions->setCurrentIndex(5);
        ui->treeWidget->setCurrentItem(itemBAS1);
        ui->descBAS1->setEnabled(true);
        ui->descBAS2->setDisabled(true);
        ui->descBAS3->setDisabled(true);
        ui->descBAS4->setDisabled(true);
    }
    if (current == itemBAS2) {
        ui->romDescriptions->setCurrentIndex(5);
        ui->descBAS1->setDisabled(true);
        ui->descBAS2->setEnabled(true);
        ui->descBAS3->setDisabled(true);
        ui->descBAS4->setDisabled(true);
    }
    if (current == itemBAS3) {
        ui->romDescriptions->setCurrentIndex(5);
        ui->descBAS1->setDisabled(true);
        ui->descBAS2->setDisabled(true);
        ui->descBAS3->setEnabled(true);
        ui->descBAS4->setDisabled(true);
    }
    if (current == itemBAS4) {
        ui->romDescriptions->setCurrentIndex(5);
        ui->descBAS1->setDisabled(true);
        ui->descBAS2->setDisabled(true);
        ui->descBAS3->setDisabled(true);
        ui->descBAS4->setEnabled(true);
    }

// Game Group
    if (current == itemXEGS1) {
        ui->romDescriptions->setCurrentIndex(6);
        ui->treeWidget->setCurrentItem(itemXEGS1);
        ui->descXEGS1->setEnabled(true);
        ui->descXEGS2->setDisabled(true);
        ui->descXEGS3->setDisabled(true);
        ui->descXEGS4->setDisabled(true);
    }
    if (current == itemXEGS2) {
        ui->romDescriptions->setCurrentIndex(6);
        ui->descXEGS1->setDisabled(true);
        ui->descXEGS2->setEnabled(true);
        ui->descXEGS3->setDisabled(true);
        ui->descXEGS4->setDisabled(true);
    }
    if (current == itemXEGS3) {
        ui->romDescriptions->setCurrentIndex(6);
        ui->descXEGS1->setDisabled(true);
        ui->descXEGS2->setDisabled(true);
        ui->descXEGS3->setEnabled(true);
        ui->descXEGS4->setDisabled(true);
    }
    if (current == itemXEGS4) {
        ui->romDescriptions->setCurrentIndex(6);
        ui->descXEGS1->setDisabled(true);
        ui->descXEGS2->setDisabled(true);
        ui->descXEGS3->setDisabled(true);
        ui->descXEGS4->setEnabled(true);
    }

// OS Group
    if (current == itemOS1) {
        ui->romDescriptions->setCurrentIndex(7);
        ui->treeWidget->setCurrentItem(itemOS1);
        ui->descOS1->setEnabled(true);
        ui->descOS2->setDisabled(true);
        ui->descOS3->setDisabled(true);
        ui->descOS4->setDisabled(true);
    }
    if (current == itemOS2) {
        ui->romDescriptions->setCurrentIndex(7);
        ui->descOS1->setDisabled(true);
        ui->descOS2->setEnabled(true);
        ui->descOS3->setDisabled(true);
        ui->descOS4->setDisabled(true);
    }
    if (current == itemOS3) {
        ui->romDescriptions->setCurrentIndex(7);
        ui->descOS1->setDisabled(true);
        ui->descOS2->setDisabled(true);
        ui->descOS3->setEnabled(true);
        ui->descOS4->setDisabled(true);
    }
    if (current == itemOS4) {
        ui->romDescriptions->setCurrentIndex(7);
        ui->descOS1->setDisabled(true);
        ui->descOS2->setDisabled(true);
        ui->descOS3->setDisabled(true);
        ui->descOS4->setEnabled(true);
    }

    if (current == itemBAS || current == itemXEGS || current == itemOS) {
        ui->loadROM->setEnabled(false);
        ui->saveROM->setEnabled(false);
    }

// All items
    if (current == itemALL) {
        ui->romDescriptions->setCurrentIndex(8);
        ui->loadROM->setEnabled(false);
        ui->saveROM->setEnabled(false);
    }
        mainWindow->setRomDescs();
}

void MainWindow::on_treeWidget_itemChanged(QTreeWidgetItem* item, int /*column*/)
{
    if (item == itemRSVD1){
        u1mbrombSettings->setRSVD1Name(item->text(0));
        ui->descRSVD1->setEnabled(true);

    }
    if (item == itemRSVD2){
        u1mbrombSettings->setRSVD2Name(item->text(0));
        ui->descRSVD2->setEnabled(true);

    }
    if (item == itemRSVD3){
        u1mbrombSettings->setRSVD3Name(item->text(0));
        ui->descRSVD3->setEnabled(true);

    }
}

void MainWindow::rsvdDescChanged()
{
        u1mbrombSettings->setRSVD1Desc(ui->descRSVD1->text());
        ui->descRSVD1->setEnabled(false);

        u1mbrombSettings->setRSVD2Desc(ui->descRSVD2->text());
        ui->descRSVD2->setEnabled(false);

        u1mbrombSettings->setRSVD3Desc(ui->descRSVD3->text());
        ui->descRSVD3->setEnabled(false);
}

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem* item, int /*column*/)
{

}

