#include "wordcounterproxy.h"

WordCounterProxy::WordCounterProxy(QObject *parent,
                                   SimpleWordCounter *wordCounter)
    : QObject(parent), wordCounterPtr(wordCounter) {

    // start
    connect(this, &WordCounterProxy::startProcessFileBG,
            wordCounterPtr, &SimpleWordCounter::start);

    // progress
    connect(wordCounterPtr, &SimpleWordCounter::updateProgress,
            this, &WordCounterProxy::setProgressUI);

    // words
    connect(wordCounterPtr, &SimpleWordCounter::updateWords,
            this, &WordCounterProxy::updateDataUI);

    // statistics
    connect(wordCounterPtr, &SimpleWordCounter::updateStatistics, [=](quint64 totalWordsNb) {
            emit updateStatistics(totalWordsNb, startDateTime.secsTo(QDateTime::currentDateTime()));
    });

    // finished
    connect(wordCounterPtr, &SimpleWordCounter::finished,
            this, &WordCounterProxy::finished);
}

void WordCounterProxy::startProcessFileUI(QString fileName) {
    startDateTime = QDateTime::currentDateTime();
    emit startProcessFileBG(fileName.remove("file:///"));
}

void WordCounterProxy::stopProcessFileUI() {
    if(wordCounterPtr) {
        wordCounterPtr->forcedStop = true;
    }
}
