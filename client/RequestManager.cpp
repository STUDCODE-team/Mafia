#include "RequestManager.h"

RequestManager::RequestManager()
{
    connect(&client, &TcpClient::newMessage, this, &RequestManager::reply);
    connect(&client, &TcpClient::setConnectionStatus, this, &RequestManager::setConnectionStatus);
}

void RequestManager::bind_server(const QString &port, const QString &ip)
{
    this->client.bind(port, ip);
}

void RequestManager::createNewRoom()
{
    client.send("REQ:NEWROOM");
}

void RequestManager::reply(const QString &ans)
{
    if(ans.contains("REP:ROOM:"))
    {
        reply_enterRoom(ans.split(":").last());
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

void RequestManager::reply_enterRoom(const QString &rep)
{
    if (rep == "NO")
    {
#ifdef QT_DEBUG
        qInfo() << QTime::currentTime().toString() << "room creating error";
#endif
        return;
    }

    emit qml_enterRoom(rep);
}
