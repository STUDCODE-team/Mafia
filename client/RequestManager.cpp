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

void RequestManager::connectToRoom(const QString &num)
{
    client.send("REQ:CONROOM:" + num);
}

void RequestManager::exitRoom()
{
    client.send("REQ:EXITROOM");
}

void RequestManager::reply(const QString &ans)
{
    if(ans.contains("REP:STATE"))
    {
        QString stateRep = ans.split(":STATE:{").last();
        reply_releaseStatus(stateRep.remove(stateRep.size()-1, 1));
    }
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

void RequestManager::reply_releaseStatus(const QString &rep)
{
    //ROOM:num:{player1, player2}
    QString roomNum = rep.split(":")[1];
    if(roomNum == "")
    {
        emit qml_exitRoom();
        return;
    }
    QStringList playerList = rep.split('{')[1].split('}')[0].split(',');
    emit qml_enterRoom(roomNum, playerList);
}
