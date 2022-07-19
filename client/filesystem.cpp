#include "FileSystem.h"


FileSystem::FileSystem()
{

}

int FileSystem::bindDeviceID()
{
    QString m_dataLocation = getStandartPath();

    QFile file(m_dataLocation + "/deviceID.txt");
    if(!file.open(QIODevice::ReadWrite))
    {
        qFatal("Device ID unreachable");
    }

    QString id = file.readAll();
    int deviceID;
    if(id.isEmpty())
    {// first application usement
        deviceID = QRandomGenerator::global()->bounded(0, INT_MAX);
        file.write(QString::number(deviceID).toLatin1());
    }
    else
    {// non-first usement
        deviceID = id.toInt();
    }
    file.close();

#ifdef QT_DEBUG
        qInfo() << QTime::currentTime().toString() << "DEVICE_ID: \t" << deviceID;
#endif

    return deviceID;
}

QString FileSystem::getStandartPath()
{
    // get folder path for currect working on IOS devices;
    QString m_dataLocation = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).value(0);

#ifndef QT_DEBUG
    if (!QDir(m_dataLocation).exists() and !QDir("").mkpath(m_dataLocation))
    {
        qFatal("Failed to reach app data directory.");
    }
#else
    if (QDir(m_dataLocation).exists()){
        qDebug() << "App data directory exists. " << m_dataLocation;
    } else {
        if (QDir("").mkpath(m_dataLocation)) {
            qDebug() << "Created app data directory. " << m_dataLocation;
        } else {
            qFatal("Failed to create app data directory.");
        }
    }
#endif
    return m_dataLocation;
}
