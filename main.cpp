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
        qDebug() << QString("Progress: %1").arg(curProgress);
        qDebug() << "Current statistic: " << wordCounter->mostPopularWords(15);
    });

    QObject::connect(wordCounter, &SimpleWordCounter::fileError, [=](QString description) {
        qDebug() << QString("Error occured: (%1)").arg(description);
    });

    QObject::connect(wordCounter, &SimpleWordCounter::processFinished, [=]() {
        qDebug() << QString("Finished!");
    });

    wordCounter->startProcessFile("../SimpleWordCounter/testText.txt");

    qDebug() << "Result: " << wordCounter->mostPopularWords(15);

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
