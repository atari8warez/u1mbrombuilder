#include "u1mbrombsettings.h"
#include "mainwindow.h"

U1MBRomBSettings::U1MBRomBSettings()
{
    mSettings = new QSettings();

    mIsFirstTime = mSettings->value("FirstTime", true).toBool();
    mSettings->setValue("FirstTime", false);
    mMainWindowTitle = mSettings->value("MainWindowTitle", " ").toString();
    if(mMainWindowTitle == " ") {
        mMainWindowTitle = mSettings->value("MainWindowTitle", "U1MB Rom Builder v").toString();
        mMainWindowTitle += VERSION;
        mSettings->setValue("MainWindowTitle", mMainWindowTitle);
    }

    mMainX = mSettings->value("MainX", 20).toInt();
    mMainY = mSettings->value("MainY", 40).toInt();
    mMainW = mSettings->value("MainW", 600).toInt();
    mMainH = mSettings->value("MainH", 486).toInt();

    mRSVD1Name = mSettings->value("RSVD1Name", "RSVD1 (64K)").toString();
    mRSVD2Name = mSettings->value("RSVD2Name", "RSVD2 (16K)").toString();
    mRSVD3Name = mSettings->value("RSVD3Name", "RSVD3 (32K)").toString();

    mRSVD1Desc = mSettings->value("RSVD1Desc", "Reserved").toString();
    mRSVD2Desc = mSettings->value("RSVD2Desc", "Reserved").toString();
    mRSVD3Desc = mSettings->value("RSVD3Desc", "Reserved").toString();

    mUseDef = mSettings->value("UseDefDir", true).toBool();
    mUseLast = mSettings->value("UseLastDir", false).toBool();
    mUseNone = mSettings->value("UseNone", false).toBool();

    mDefBaseRomDir = mSettings->value("defBaseRomDir", QDir::currentPath() + "/roms").toString();
    mSettings->setValue("defBaseRomDir", mDefBaseRomDir);

    mDefRomDir = mSettings->value("defRomDir","").toString();
    if (mUseDef) {
        mSettings->setValue("defRomDir", mDefBaseRomDir);
    }

    mDefBaseRomName = mSettings->value("defBaseRomName", "U1MBv2.rom").toString();
    mSettings->setValue("defBaseRomName", mDefBaseRomName);

    mI18nLanguage = mSettings->value("I18nLanguage", "auto").toString();
    mMinimizeToTray = mSettings->value("MinimizeToTray", false).toBool();
    mSaveWindowsPos = mSettings->value("SaveWindowsPosSize", true).toBool();
}

U1MBRomBSettings::~U1MBRomBSettings()
{
    delete mSettings;
}


QString U1MBRomBSettings::mainWindowTitle()
{
    return mMainWindowTitle;
}

bool U1MBRomBSettings::isFirstTime()
{
    return mIsFirstTime;
}

bool U1MBRomBSettings::useDef()
{
    return mUseDef;
}

void U1MBRomBSettings::setUseDef(bool useDef)
{
    mUseDef = useDef;
    mSettings->setValue("UseDefDir", mUseDef);
}

bool U1MBRomBSettings::useLast()
{
    return mUseLast;
}

void U1MBRomBSettings::setUseLast(bool useLast)
{
    mUseLast = useLast;
    mSettings->setValue("UseLastDir", mUseLast);
}

bool U1MBRomBSettings::useNone()
{
    return mUseNone;
}

void U1MBRomBSettings::setUseNone(bool useNone)
{
    mUseNone = useNone;
    mSettings->setValue("UseNone", mUseNone);
}

QString U1MBRomBSettings::defBaseRomName()
{
    return mDefBaseRomName;
}

void U1MBRomBSettings::setdefBaseRomName(const QString &defBaseRomName)
{
    mDefBaseRomName = defBaseRomName;
    mSettings->setValue("defBaseRomName", mDefBaseRomName);
}

QString U1MBRomBSettings::defBaseRomDir()
{
    return mDefBaseRomDir;
}

void U1MBRomBSettings::setdefBaseRomDir(const QString &dir)
{
    mDefBaseRomDir = dir;
    mSettings->setValue("defRomDir", mDefBaseRomDir);
}

QString U1MBRomBSettings::defRomDir()
{
    return mDefRomDir;
}

void U1MBRomBSettings::setdefRomDir(const QString &dir)
{
    mDefRomDir = dir;
    mSettings->setValue("defRomDir", mDefRomDir);
}

QString U1MBRomBSettings::RSVD1Name()
{
    return mRSVD1Name;
}

void U1MBRomBSettings::setRSVD1Name(const QString &rsvd1Name)
{
    mRSVD1Name = rsvd1Name;
    mSettings->setValue("RSVD1Name", mRSVD1Name);
}

QString U1MBRomBSettings::RSVD2Name()
{
    return mRSVD2Name;
}

void U1MBRomBSettings::setRSVD2Name(const QString &rsvd2Name)
{
    mRSVD2Name = rsvd2Name;
    mSettings->setValue("RSVD2Name", mRSVD2Name);
}

QString U1MBRomBSettings::RSVD3Name()
{
    return mRSVD3Name;
}

void U1MBRomBSettings::setRSVD3Name(const QString &rsvd3Name)
{
    mRSVD3Name = rsvd3Name;
    mSettings->setValue("RSVD3Name", mRSVD3Name);
}
QString U1MBRomBSettings::RSVD1Desc()
{
    return mRSVD1Desc;
}

void U1MBRomBSettings::setRSVD1Desc(const QString &rsvd1Desc)
{
    mRSVD1Desc = rsvd1Desc;
    mSettings->setValue("RSVD1Desc", mRSVD1Desc);
}

QString U1MBRomBSettings::RSVD2Desc()
{
    return mRSVD2Desc;
}

void U1MBRomBSettings::setRSVD2Desc(const QString &rsvd2Desc)
{
    mRSVD2Desc = rsvd2Desc;
    mSettings->setValue("RSVD2Desc", mRSVD2Desc);
}

QString U1MBRomBSettings::RSVD3Desc()
{
    return mRSVD3Desc;
}

void U1MBRomBSettings::setRSVD3Desc(const QString &rsvd3Desc)
{
    mRSVD3Desc = rsvd3Desc;
    mSettings->setValue("RSVD3Desc", mRSVD3Desc);
}

bool U1MBRomBSettings::saveWindowsPos()
{
    return mSaveWindowsPos;
}

void U1MBRomBSettings::setSaveWindowsPos(bool saveMwp)
{
    mSaveWindowsPos = saveMwp;
    mSettings->setValue("SaveWindowsPosSize", mSaveWindowsPos);
}
// Last main window position/size

int U1MBRomBSettings::lastHorizontalPos()
{
    return mMainX;
}

void U1MBRomBSettings::setLastHorizontalPos(int lastHpos)
{
    mMainX = lastHpos;
    mSettings->setValue("MainX", mMainX);
}
int U1MBRomBSettings::lastVerticalPos()
{
    return mMainY;
}

void U1MBRomBSettings::setLastVerticalPos(int lastVpos)
{
    mMainY = lastVpos;
    mSettings->setValue("MainY", mMainY);
}
int U1MBRomBSettings::lastWidth()
{
    return mMainW;
}

void U1MBRomBSettings::setLastWidth(int lastW)
{
    mMainW = lastW;
    mSettings->setValue("MainW", mMainW);
}
int U1MBRomBSettings::lastHeight()
{
    return mMainH;
}

void U1MBRomBSettings::setLastHeight(int lastH)
{
    mMainH = lastH;
    mSettings->setValue("MainH", mMainH);
}

QString U1MBRomBSettings::i18nLanguage()
{
    return mI18nLanguage;
}

void U1MBRomBSettings::setI18nLanguage(const QString &lang)
{
    mI18nLanguage = lang;
    mSettings->setValue("I18nLanguage", mI18nLanguage);
}

bool U1MBRomBSettings::minimizeToTray()
{
    return mMinimizeToTray;
}

void U1MBRomBSettings::setMinimizeToTray(bool tray)
{
    mMinimizeToTray = tray;
    mSettings->setValue("MinimizeToTray", mMinimizeToTray);
}
