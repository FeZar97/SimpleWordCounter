#ifndef SIMPLEWORDCOUNTER_H
#define SIMPLEWORDCOUNTER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QFile>
#include <QSet>
#include <QVariantList>

using Popularity = quint64;

enum FileOpenErrors {
    NOT_EXIST,
    NOT_AVAILABLE,
    READ_ERROR
};

const int PopularWordsNb{10};

const double ProgressUpdateThreshold{0.0001};
const double WordsUpdateThreshold{0.01};
const quint64 ChunkSize{1 << 20}; // 1 MB

class SimpleWordCounter: public QObject {
    Q_OBJECT

    bool isWorking{false};
    float currentProgress{0}, prevProgressUpdateVal{0}, prevWordsUpdateVal{0};
    QByteArray internalBuffer;
    quint64 totalWordsNb{0};

    QMap<Popularity, QList<QStringView>> popularityToWords;
    QMap<QStringView, Popularity> wordToPopularity;
    QSet<QString> words;

    void reset();

    bool isWord(QString &str) const;
    void promoteWord(const QString &word);

    QVariantList mostPopularWords(int nbThreshold) const;

public:
    SimpleWordCounter(QObject *parent = nullptr);
    bool forcedStop{false};
    void start(QString fileName);

signals:
    void updateProgress(float curProgress = 0.);
    void updateWords(QVariantList data);
    void updateStatistics(quint64 totalWordsNb);
    void fileError(QString description);
    void finished();
};

#endif // SIMPLEWORDCOUNTER_H
