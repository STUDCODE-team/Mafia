#ifndef NFC_H
#define NFC_H

#include <QObject>
#include <QNearFieldManager>
#include <QNdefMessage>
#include <QNdefNfcTextRecord>

#ifdef QT_DEBUG
    #include <QDebug>
    #include <QTime>
#endif

class NFC : public QObject
{
    Q_OBJECT

public:
    explicit NFC(QObject *parent = nullptr);
    ~NFC();
    enum Action {
        NoAction,
        ReadNdef,
        WriteNdef
    };

    void tryWrite(const QByteArray &message);
    void tryRead();
    void off();
private:
    QNearFieldManager *nfc;
    Action nfcAction = NoAction;
    QByteArray messageToWrite;

private slots:
    void ndefMessageRead(const QNdefMessage &message);
    void targetDetected(QNearFieldTarget *target);
    void targetLost(QNearFieldTarget *target);
    void targetError(QNearFieldTarget::Error error, const QNearFieldTarget::RequestId &id);
};

#endif // NFC_H
