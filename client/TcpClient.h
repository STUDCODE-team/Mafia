#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <Timer.h>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    void bind(const QString &ip, const QString &port);

public slots:
    void sendMessage(const QString &message);

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

    Timer *try_to_connect   = new Timer();
    Timer *check_connection = new Timer();
    void shiftTimers();

    void checkRequest();
    void sendRequest();

#define WaitForNetwork  "Wait for network connection..."
#define TryToConnect    "Trying to connect..."
#define Connected       "Connected"

#define connectionDelay 3000

};

#endif // TCPCLIENT_H
