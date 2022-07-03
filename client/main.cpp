#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "QtCore/qthread.h"
#include "TcpClient.h"
#include "QtConcurrent"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);


    TcpClient client;
    client.bind("91.239.26.149", "5893");


    engine.rootContext()->setContextProperty("client", &client);
    engine.load(url);

    return app.exec();
}
