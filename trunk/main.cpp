#include <QtGui/QApplication>
#include <QTextCodec>
#include <QLibraryInfo>
#include "mainwindow.h"

#ifdef Q_OS_WIN
#include <windows.h>
#include <Mmsystem.h>
#endif

int main(int argc, char *argv[])
{
    int ret;

    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    MainWindow w;
    if (!w.terminate()) {
        w.show();
        ret = a.exec();
    }
    return ret;
}
