#include "TcpClient.h"

TcpClient::TcpClient(QObject *parent) : QObject(parent)
{
    connect(&_socket,   &QTcpSocket::connected,       this,     &TcpClient::onConnected);
    connect(&_socket,   &QTcpSocket::disconnected,    this,     &TcpClient::onDisconnected);
    connect(&_socket,   &QTcpSocket::errorOccurred,   this,     &TcpClient::onErrorOccurred);
    connect(&_socket,   &QTcpSocket::readyRead,       this,     &TcpClient::onReadyRead);
}

void TcpClient::bind(const QString &ip, const QString &port)
{
    // thying to connect every N seconds via timer
    connect(try_to_connect, &Timer::timeout, this, [=]{connectToServer(ip, port);});
    try_to_connect->start(connectionDelay);

    // checking if connection available every N seconds via timer
    connect(check_connection, &Timer::timeout, this, &TcpClient::checkRequest);
}

void TcpClient::connectToServer(const QString &ip, const QString &port)
{
    _socket.connectToHost(ip, port.toUInt());
}

void TcpClient::onConnected()
{
#ifdef QT_DEBUG
    qInfo() << "Connected to host.";
#endif
    isConnected = true;
    lastRequestAnswered = true;
    emit setConnectionStatus(Connected);
    shiftTimers();
}

void TcpClient::onDisconnected()
{
#ifdef QT_DEBUG
    qInfo() << "Disconnected.";
#endif
    isConnected = false;
    lastRequestAnswered = false;
    emit setConnectionStatus(TryToConnect);
    shiftTimers();
}

void TcpClient::shiftTimers()
{
    if (isConnected)
    {// start checking if connection available every N seconds
        try_to_connect->stop();
        check_connection->start(connectionDelay);
    }
    else
    {// start trying to connect
        check_connection->stop();
        try_to_connect->start(connectionDelay);
    }
}

void TcpClient::sendMessage(const QString &message)
{
    _socket.write(message.toUtf8());
    _socket.flush();
}

void TcpClient::onReadyRead()
{
    const auto message = _socket.readAll();
    if(message == "connection:confirmed")
    {
        lastRequestAnswered = true;
        return;
    }
    emit newMessage(message);
}

void TcpClient::onErrorOccurred(QAbstractSocket::SocketError error)
{
    if (error == QAbstractSocket::NetworkError)
    {
        emit setConnectionStatus(WaitForNetwork);
    }
#ifdef QT_DEBUG
    qWarning() << "Error:" << error;
#endif
}

void TcpClient::sendRequest()
{
    sendMessage("connection:check");
}

void TcpClient::checkRequest()
{
    if (lastRequestAnswered)
    {// connection is confirmed
#ifdef QT_DEBUG
        qInfo() << "connection is confirmed";
#endif
        lastRequestAnswered = false;
        sendRequest();
    }
    else
    {// check request was not confirmed
#ifdef QT_DEBUG
        qInfo() << "connection is failed";
#endif
        _socket.close();
    }
}

