#include "RequestManager.h"
#include <QRandomGenerator>

#include <QStandardPaths>
#include <QFile>
#include <QDir>


#ifdef QT_DEBUG
    #include <QTime>
#endif

RequestManager::RequestManager()
{
    connect(&client, &TcpClient::newMessage, this, &RequestManager::reply);
    connect(&client, &TcpClient::setConnectionStatus, this, &RequestManager::setConnectionStatus);
    setClientDeviceID();
}

void RequestManager::bind_server(const QString &port, const QString &ip)
{
    this->client.bind(port, ip);
}

void RequestManager::setClientDeviceID()
{
    // get folder path for currect working on IOS devices;
    QString m_dataLocation = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation).value(0);

#ifndef QT_DEBUG
    if (!QDir(m_dataLocation).exists())
    {
        QDir("").mkpath(m_dataLocation);
    }
#else
    if (QDir(m_dataLocation).exists()){
        qDebug() << "App data directory exists. " << m_dataLocation;
    } else {
        if (QDir("").mkpath(m_dataLocation)) {
            qDebug() << "Created app data directory. " << m_dataLocation;
        } else {
            qDebug() << "Failed to create app data directory. " << m_dataLocation;
        }
    }
#endif

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

    client.setDeviceID(deviceID);

#ifdef QT_DEBUG
        qInfo() << QTime::currentTime().toString() << "DEVICE_ID: \t" << deviceID;
#endif
}

void RequestManager::createNewRoom()
{
    client.send("REQ:NEWROOM");
}

void RequestManager::reply(const QString &ans)
{
    if(ans.contains("REP:NEWROOM:"))
    {
        reply_newroom(ans.split(":").last());
    }
//    else if()
//    {

//    }
//    else if()
//    {

//    }
    else
    {
#ifdef QT_DEBUG
        qInfo() << QTime::currentTime().toString() << "Unknown reply";
#endif
    }
}

void RequestManager::reply_newroom(const QString &rep)
{
    if (rep == "NO")
    {
#ifdef QT_DEBUG
        qInfo() << QTime::currentTime().toString() << "room creating error";
#endif
        return;
    }

    emit qml_new_room(rep);
}
