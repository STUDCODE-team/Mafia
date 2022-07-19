#include "RequestManager.h"

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
    int deviceID = FileSystem::bindDeviceID();
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
        qInfo() << QTime::currentTime().toString() << "Unknown reply message";
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
