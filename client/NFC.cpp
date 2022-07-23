#include "NFC.h"

NFC::NFC(QObject *parent)
    : QObject{parent}
{
    nfc = new QNearFieldManager(this);
    connect(nfc, &QNearFieldManager::targetDetected, this, &NFC::targetDetected);
    connect(nfc, &QNearFieldManager::targetLost,     this, &NFC::targetLost);
}

NFC::~NFC()
{
    delete nfc;
}

void NFC::tryWrite(const QByteArray &message)
{
    nfcAction = WriteNdef;
    messageToWrite = message;
}

void NFC::tryRead()
{
    nfcAction = ReadNdef;
}

void NFC::off()
{
    nfcAction = NoAction;
}

void NFC::ndefMessageRead(const QNdefMessage &message)
{
    if(message.isEmpty()) return;
    QString room = message.toByteArray().toStdString().c_str();
    // and??
#ifdef QT_DEBUG
        qInfo() << QTime::currentTime().toString() << room;
#endif
}

void NFC::targetDetected(QNearFieldTarget *target)
{
#ifdef QT_DEBUG
        qInfo() << QTime::currentTime().toString() << "NFC DETECTED";
#endif
    switch (nfcAction) {
        case NoAction:
            break;
        case ReadNdef:
                connect(target, &QNearFieldTarget::ndefMessageRead, this, &NFC::ndefMessageRead);
                connect(target, &QNearFieldTarget::error, this, &NFC::targetError);
                target->readNdefMessages();
//                if (!m_request.isValid()) // cannot read messages
//                    targetError(QNearFieldTarget::NdefReadError, m_request);
                break;
        case WriteNdef:
                connect(target, &QNearFieldTarget::error, this, &NFC::targetError);

                QNdefNfcTextRecord textRecord;
                textRecord.setText(messageToWrite);
                QNdefMessage message;
                message.append(textRecord);

                target->writeNdefMessages(QList<QNdefMessage>() << message);
    //            if (!m_request.isValid()) // cannot write messages
    //                targetError(QNearFieldTarget::NdefWriteError, m_request);
                break;
    }
}

void NFC::targetLost(QNearFieldTarget *target)
{
    target->deleteLater();
}

void NFC::targetError(QNearFieldTarget::Error error, const QNearFieldTarget::RequestId &id)
{
#ifdef QT_DEBUG
        qInfo() << QTime::currentTime().toString() << error << " " << id.d;
#endif
}
