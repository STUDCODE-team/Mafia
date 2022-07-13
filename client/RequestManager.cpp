#include "RequestManager.h"

RequestManager::RequestManager()
{
    connect(&client, &TcpClient::newMessage, this, &RequestManager::answer);
    connect(&client, &TcpClient::setConnectionStatus, this, &RequestManager::setConnectionStatus);
}

void RequestManager::bind_server(const QString &port, const QString &ip)
{
    this->client.bind(port, ip);
}

void RequestManager::createNewRoom()
{
    client.send("request:createroom");
}

void RequestManager::answer(const QString &ans)
{
    QList<QString> ansParse = ans.split("=");
    if (ansParse.first() == "answer:roomcreated")
    {
        emit roomCreated(ansParse.last());
    }
}
