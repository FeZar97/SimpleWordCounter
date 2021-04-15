#ifndef WORDCOUNTERPROXY_H
#define WORDCOUNTERPROXY_H

#include <QObject>
#include <QDateTime>

#include "simplewordcounter.h"

class WordCounterProxy : public QObject {
    Q_OBJECT
    SimpleWordCounter *wordCounterPtr{nullptr};
    QDateTime startDateTime;

public:
    explicit WordCounterProxy(QObject *parent = nullptr,
                              SimpleWordCounter *wordCounter = nullptr);

public slots:
    void startProcessFileUI(QString fileName);
    void stopProcessFileUI();

signals:
    void startProcessFileBG(QString fileName);
    void setProgressUI(float curProgress);
    void updateDataUI(QVariantList data);
    void updateStatistics(quint64 totalWordsNb, qint64 processingTimeInSec);
    void finished();
};

#endif // WORDCOUNTERPROXY_H
