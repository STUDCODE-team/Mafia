#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QObject>
#include <TcpClient.h>
#include <NFC.h>

#ifdef QT_DEBUG
    #include <QTime>
#endif

class RequestManager : public QObject
{
    Q_OBJECT

public:
    explicit RequestManager();
    ~RequestManager();

    void bind_server(const QString &port, const QString &ip);

public slots:
    void broadcastNFC(const QByteArray &room);
    void catchNFC();
    void offNFC();

    void createNewRoom();
    void connectToRoom(const QString &num);
    void exitRoom();

private:
    void reply(const QString &rep);

    void reply_releaseStatus(const QString &rep);

    TcpClient client;

    NFC m_nfc;
    void ndefMessageRead(QString &room);

private:

signals:
     void setConnectionStatus(const QString &status);
     void qml_enterRoom(const QString &roomKey, const QStringList &playersList);
     void qml_exitRoom();

};

#endif // REQUESTMANAGER_H
