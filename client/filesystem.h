#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <QStandardPaths>
#include <QRandomGenerator>
#include <QFile>
#include <QDir>

#ifdef QT_DEBUG
    #include <QDebug>
    #include <QTime>
#endif

class FileSystem
{
public:
    FileSystem();

    static int bindDeviceID();

private:
    static QString getStandartPath();
};

#endif // FILESYSTEM_H
