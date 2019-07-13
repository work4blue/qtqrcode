#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "qrddencode.h"
#include "AndroidActivity.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<QREnCode>("com.duoduo.components", 1, 0, "QtQuick2QREncode");



    QQmlApplicationEngine engine;

    //注册qml可使用的全局变量
    AndroidActivity mActivity;
        engine.rootContext()->setContextProperty("AndroidActivity", &mActivity);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);



    return app.exec();
}
