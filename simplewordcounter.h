#ifndef SIMPLEWORDCOUNTER_H
#define SIMPLEWORDCOUNTER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QFile>
#include <QThread>
#include <QRegExp>

#include <QDebug>

using Popularity = quint64;

enum FileOpenErrors {
    NOT_EXIST,
    NOT_AVAILABLE,
    READ_ERROR
};

class SimpleWordCounter: public QObject {
    Q_OBJECT

    bool isWorking{false};
    int currentProgress{0};
    QByteArray internalBuffer;

    const quint64 ChunkSize{1 << 20}; // 1 MB
    QMap<Popularity, QList<QStringView>> popularityToWords;
    QMap<QStringView, Popularity> wordToPopularity;
    QSet<QString> words;

    void reset();

    bool isWord(QString &str) const;
    void promoteWord(const QString &word);

public:
    SimpleWordCounter(QObject *parent = nullptr);

    int getCurrentProgress() const;
    bool isBusy() const;

    void startProcessFile(QString fileName);
    QMap<QString, int> mostPopularWords(int nbThreshold) const;

signals:
    void updateProgress(float curProgress = 0.);
    void fileError(QString description);
    void processFinished();
};

#endif // SIMPLEWORDCOUNTER_H
