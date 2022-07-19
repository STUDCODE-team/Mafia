#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QObject>
#include <TcpClient.h>
#include "FileSystem.h"

#ifdef QT_DEBUG
    #include <QTime>
#endif

class RequestManager : public QObject
{
    Q_OBJECT

public:
    explicit RequestManager();

    void bind_server(const QString &port, const QString &ip);

public slots:
    void createNewRoom();

private:
    void reply(const QString &rep);

    void reply_newroom(const QString &rep);

    void setClientDeviceID();

    TcpClient client;

signals:
     void setConnectionStatus(const QString &status);
     void qml_new_room(const QString &roomKey);
};

#endif // REQUESTMANAGER_H
