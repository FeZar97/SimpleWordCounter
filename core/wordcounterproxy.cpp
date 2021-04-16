#include "wordcounterproxy.h"

WordCounterProxy::WordCounterProxy(QObject *parent,
                                   SimpleWordCounter *wordCounter)
    : QObject(parent), wordCounterPtr(wordCounter) {

    // start
    connect(this, &WordCounterProxy::startProcessFileBG,
            wordCounterPtr, &SimpleWordCounter::start);

    // work progress
    connect(wordCounterPtr, &SimpleWordCounter::updateProgress,
            this, &WordCounterProxy::setProgressUI, Qt::QueuedConnection);

    // info about most common words
    connect(wordCounterPtr, &SimpleWordCounter::updateWords,
            this, &WordCounterProxy::updateDataUI, Qt::QueuedConnection);

    // work statistics
    connect(wordCounterPtr, &SimpleWordCounter::updateStatistics, [=](quint64 totalWordsNb) {
            emit updateStatistics(totalWordsNb, startDateTime.secsTo(QDateTime::currentDateTime()));
    });

    connect(wordCounterPtr, &SimpleWordCounter::finished, this, &WordCounterProxy::finished);
}

// start worker
void WordCounterProxy::startProcessFileUI(QString fileName) {
    startDateTime = QDateTime::currentDateTime();
    emit startProcessFileBG(fileName.remove("file:///"));
}

// forced stopping worker
void WordCounterProxy::stopProcessFileUI() {
    if(wordCounterPtr) {
        wordCounterPtr->forcedStop = true;
    }
}
