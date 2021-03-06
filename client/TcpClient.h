#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <Timer.h>
#include "FileSystem.h"

#ifdef QT_DEBUG
    #include <QTime>
#endif

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient();
    void bind(const QString &ip, const QString &port);

public slots:
    void send(const QString &message);

private:
    void onConnected();
    void onDisconnected();

    bool isConnected = false;
    bool lastRequestAnswered = false;

signals:
    void newMessage(const QByteArray &ba);
    void setConnectionStatus(const QString &status);

private slots:
    void connectToServer(const QString &ip, const QString &port);
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError error);

private:
    QTcpSocket _socket;

    Timer try_to_connect;
    Timer check_connection;
    void shiftTimers();

    int deviceID = FileSystem::bindDeviceID();
    void sendDeviceID();

    void checkConnRequest();
    void sendConnRequest();

    void reply(const QByteArray &rep);

#define WaitForNetwork  "Wait for network connection..."
#define TryToConnect    "Trying to connect..."
#define Connected       "Connected"

};

#endif // TCPCLIENT_H
