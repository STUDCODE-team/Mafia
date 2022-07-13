#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QObject>
#include <TcpClient.h>

class RequestManager : public QObject
{
    Q_OBJECT

public:
    explicit RequestManager();

    void bind_server(const QString &port, const QString &ip);

public slots:
    void createNewRoom();

private:
    void answer(const QString &ans);
    TcpClient client;

signals:
     void setConnectionStatus(const QString &status);
     void roomCreated(const QString &roomKey);
};

#endif // REQUESTMANAGER_H
