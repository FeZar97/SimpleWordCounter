#ifndef SIMPLEWORDCOUNTER_H
#define SIMPLEWORDCOUNTER_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QFile>
#include <QSet>
#include <QVariantList>
#include <QDateTime>

using Popularity = quint64;

const int PopularWordsNb{15};
const qint64 ChunkSize{1 << 20}; // 1 MB
const qint64 UiUpdatePeridMsecs{350};

class SimpleWordCounter: public QObject {
    Q_OBJECT

    bool isWorking{false};
    float currentProgress{0};
    QByteArray internalBuffer;
    quint64 totalWordsNb{0};
    QDateTime prevUiUpdateTime;

    QMap<Popularity, QList<QStringView>> popularityToWords;
    QMap<QStringView, Popularity> wordToPopularity;
    QSet<QString> words;

    void reset();
    bool tryOpenFile(QFile &file);
    bool checkFileAvailable(QFile &file);
    qint64 readCorrectnessChunk(QFile &curFile);
    bool isWord(QString &str) const;
    void promoteWord(const QString &word);
    QVariantList mostPopularWords(int nbThreshold) const;
    void updateUi();

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
