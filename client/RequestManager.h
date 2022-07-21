#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QObject>
#include <TcpClient.h>

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
    void exitRoom();

private:
    void reply(const QString &rep);

    void reply_releaseStatus(const QString &rep);

    TcpClient client;

signals:
     void setConnectionStatus(const QString &status);
     void qml_enterRoom(const QString &roomKey, const QStringList &playersList);
     void qml_exitRoom();

};

#endif // REQUESTMANAGER_H
