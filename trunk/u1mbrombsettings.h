#ifndef U1MBROMBSETTINGS_H
#define U1MBROMBSETTINGS_H
#include <QSettings>
#include <QtDebug>

class U1MBRomBSettings
{
public:
    class RomSettings {
    public:
        QString fileName;
        bool isWriteProtected;
    };

    U1MBRomBSettings();
    ~U1MBRomBSettings();

    bool isFirstTime();
    QString i18nLanguage();
    QString mainWindowTitle();

    QString defBaseRomName();
    void setdefBaseRomName (const QString &defBaseRomName);

    QString defBaseRomDir();
    void setdefBaseRomDir(const QString &dir);

    QString defRomDir();
    void setdefRomDir(const QString &dir);

    bool useDef();
    void setUseDef (bool useDef);

    bool useLast();
    void setUseLast (bool useLast);

    bool useNone();
    void setUseNone (bool useNone);

    QString RSVD1Name ();
    void setRSVD1Name ( const QString &rsvd1Name);

    QString RSVD2Name ();
    void setRSVD2Name ( const QString &rsvd2Name);

    QString RSVD3Name ();
    void setRSVD3Name ( const QString &rsvd3Name);

    QString RSVD1Desc ();
    void setRSVD1Desc ( const QString &rsvd1Desc);

    QString RSVD2Desc ();
    void setRSVD2Desc ( const QString &rsvd2Desc);

    QString RSVD3Desc ();
    void setRSVD3Desc ( const QString &rsvd2Desc);

    int lastVerticalPos();
    void setLastVerticalPos(int lastVpos);

    int lastHorizontalPos();
    void setLastHorizontalPos(int lastHpos);

    int lastWidth();
    void setLastWidth(int lastW);

    int lastHeight();
    void setLastHeight(int lastH);

    void setI18nLanguage(const QString &$lang);

    QString backColor (bool preview, int index);
    int backColorIndex();
    void setBackColor(int colorIndex);

    bool minimizeToTray();
    void setMinimizeToTray(bool tray);

    bool saveWindowsPos();
    void setSaveWindowsPos(bool saveMwp);

private:
    QSettings *mSettings;

    bool mIsFirstTime;
    bool mMinimizeToTray;
    bool mSaveWindowsPos;
    bool mUseDef;
    bool mUseLast;
    bool mUseNone;

    int mMainX;
    int mMainY;
    int mMainW;
    int mMainH;
    int mBackColor;

    QString mMainWindowTitle;
    QString mDefRomDir;
    QString mDefBaseRomDir;
    QString mDefBaseRomName;
    QString mI18nLanguage;
    QString mRSVD1Name;
    QString mRSVD2Name;
    QString mRSVD3Name;
    QString mRSVD1Desc;
    QString mRSVD2Desc;
    QString mRSVD3Desc;
    QString mStyle;

};

extern U1MBRomBSettings *u1mbrombSettings;

#endif // U1MBROMBSETTINGS_H
