#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>

#include "simplewordcounter.h"
#include "wordcounterproxy.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    app.setOrganizationName("FeZar97");

    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    QQmlContext *context = engine.rootContext();

    SimpleWordCounter *wordCounter = new SimpleWordCounter();
    WordCounterProxy *wordCounterProxy = new WordCounterProxy(nullptr, wordCounter);

    context->setContextProperty("wordCounterProxy", wordCounterProxy);

    QThread *workerThread = new QThread();
    wordCounter->moveToThread(workerThread);
    workerThread->start();

    wordCounterProxy->moveToThread(engine.thread());

    engine.load(url);
    return app.exec();
}
