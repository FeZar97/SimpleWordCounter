#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "simplewordcounter.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    SimpleWordCounter *wordCounter = new SimpleWordCounter();
    QThread *workerThread = new QThread();
    wordCounter->moveToThread(workerThread);
    workerThread->start();

    QObject::connect(wordCounter, &SimpleWordCounter::updateProgress, [=](float curProgress) {
        qDebug() << "Progress: " << curProgress;
    });

    wordCounter->startProcessFile("C:/Users/fnazarov/Desktop/test text.txt");

    qDebug() << wordCounter->mostPopularWords(15);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
