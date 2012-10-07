#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QtDebug>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QTranslator>
#include <QSystemTrayIcon>
#include <QTreeWidget>

#include "optionsdialog.h"
#include "aboutdialog.h"
#include "creditsdialog.h"
#include "helpdialog.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
     void closeEvent(QCloseEvent *event);
     void hideEvent(QHideEvent *event);

public slots:
    void show();
    bool terminate();

private:
    Ui::MainWindow *ui;
    QTranslator u1mbromb_translator, u1mbromb_qt_translator;
    QSystemTrayIcon trayIcon;
    Qt::WindowFlags oldWindowFlags;
    Qt::WindowStates oldWindowStates;
    QTreeWidgetItem *itemSDX, *itemRSVD1, *itemBIOS, *itemRSVD2, *itemRSVD3;
    QTreeWidgetItem *itemBAS, *itemBAS1, *itemBAS2, *itemBAS3, *itemBAS4;
    QTreeWidgetItem *itemXEGS, *itemXEGS1, *itemXEGS2, *itemXEGS3, *itemXEGS4;
    QTreeWidgetItem *itemOS, *itemOS1, *itemOS2, *itemOS3, *itemOS4;
    QTreeWidgetItem *itemALL;

    QByteArray lookupI;
    QByteArray lookupA;

    QByteArray romData;
    QByteArray SDXdata;
    QByteArray BIOSdata;
    QByteArray RSVD1data;
    QByteArray RSVD2data;
    QByteArray RSVD3data;
    QByteArray BAS1data;
    QByteArray BAS2data;
    QByteArray BAS3data;
    QByteArray BAS4data;
    QByteArray XEGS1data;
    QByteArray XEGS2data;
    QByteArray XEGS3data;
    QByteArray XEGS4data;
    QByteArray OS1data;
    QByteArray OS2data;
    QByteArray OS3data;
    QByteArray OS4data;

    QString appWindowTitle;

    QString U1MBRomFile;
    QString U1MBRomFilePath;
    QString SDXRomFile;
    QString SDXRomFilePath;
    QString RSVD1RomFile;
    QString RSVD1RomFilePath;
    QString RSVD2RomFile;
    QString RSVD2RomFilePath;
    QString RSVD3RomFile;
    QString RSVD3RomFilePath;
    QString BIOSRomFile;
    QString BIOSRomFilePath;
    QString BAS1RomFile;
    QString BAS1RomFilePath;
    QString BAS2RomFile;
    QString BAS2RomFilePath;
    QString BAS3RomFile;
    QString BAS3RomFilePath;
    QString BAS4RomFile;
    QString BAS4RomFilePath;
    QString XEGS1RomFile;
    QString XEGS1RomFilePath;
    QString XEGS2RomFile;
    QString XEGS2RomFilePath;
    QString XEGS3RomFile;
    QString XEGS3RomFilePath;
    QString XEGS4RomFile;
    QString XEGS4RomFilePath;
    QString OS1RomFile;
    QString OS1RomFilePath;
    QString OS2RomFile;
    QString OS2RomFilePath;
    QString OS3RomFile;
    QString OS3RomFilePath;
    QString OS4RomFile;
    QString OS4RomFilePath;
    QString u1mbrombPath;

    int U1MBSize;
    int SDXSize;
    int RSVD1Size;
    int RSVD2Size;
    int RSVD3Size;
    int BIOSSize;
    int BASICSize;
    int XEGSSize;
    int OSSize;

    bool terminalERR;
    bool shownFirstTime;
    bool changesMade;


    void loadTranslators();


private slots:
     void loadRom(QString defBaseRomDir, QString defBaseRomName, QString type, int romSize);
     void saveRom(QString defBaseRomDir, QString defBaseRomName, QString type);
     QString getDescs(int offset);
     void putDescs(QString desc, int offset);
     bool checkSize (int fileSize, int size);
     int showChangesMade(QString saveMsg);
     void populateDescs (int offset);
     void updateDescs (int offset);
     void on_actionNew_triggered();
     void on_actionLoad_triggered();
     bool on_actionSave_triggered();
     void on_actionMakeATR_triggered();
     void on_actionExit_triggered();
     void on_actionOptions_triggered();
     void on_actionHelp_triggered();
     void on_actionAbout_triggered();
     void on_actionCredits_triggered();
     void on_actionLoadROM_triggered();
     void on_actionSaveROM_triggered();
     void rsvdDescChanged();
     void descChanged();
     void setRomValues(QString rom, QString fileName, QString path);
     void setRomDescs();
     void resetRomPaths();
     void resetRomFileName();
     void reassembleRom();
     void on_treeWidget_itemChanged(QTreeWidgetItem* item,int column);
     void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);
     void on_treeWidget_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
     void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

};

#endif // MAINWINDOW_H
