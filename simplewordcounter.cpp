#include "simplewordcounter.h"

void SimpleWordCounter::reset() {
    isWorking = false;
    currentProgress = 0;
    popularityToWords.clear();
    wordToPopularity.clear();
    words.clear();
}

bool SimpleWordCounter::isWord(QString &str) const {
    str = str.toLower();
    return true;
}

void SimpleWordCounter::promoteWord(const QString &word) {
    quint64 prevPopularity;

    QStringView wordView = *(words.insert(word));

    // if already exist
    if(wordToPopularity.count(wordView)) {
        prevPopularity = wordToPopularity[word];
        wordToPopularity[wordView] = prevPopularity + 1;
        popularityToWords[prevPopularity].removeOne(wordView);
        popularityToWords[prevPopularity + 1].push_back(wordView);
    } else {
    // if not exist
        wordToPopularity[wordView] = 1;
        popularityToWords[1].push_back(wordView);
    }
}

QMap<QString, int> SimpleWordCounter::mostPopularWords(int nbThreshold) const {
    QMap<QString, int> result;

    const QList<Popularity> &popularities = popularityToWords.keys();

    for(auto curPopularityIter = popularities.rbegin(); curPopularityIter != popularities.rend(); curPopularityIter++) {

        const Popularity curPopularity = *curPopularityIter;

        for(QStringView wordView: popularityToWords[curPopularity]) {

            result.insert(QString(wordView.data()), curPopularity);

            if(result.size() == nbThreshold) {
                return result;
            }
        }
    }

    return result;
}

SimpleWordCounter::SimpleWordCounter(QObject *parent): QObject(parent) {
    internalBuffer.resize(ChunkSize);
}

int SimpleWordCounter::getCurrentProgress() const {
    return currentProgress;
}

bool SimpleWordCounter::isBusy() const {
    return isWorking;
}

void SimpleWordCounter::startProcessFile(QString fileName) {

    if(isWorking) {
        return;
    }
    reset();

    QFile inputFile(fileName);
    if(!inputFile.exists()) {
        emit fileError(NOT_EXIST, fileName);
        return;
    }
    if(!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit fileError(NOT_AVAILABLE, fileName);
        return;
    }

    quint64 fileSize = inputFile.size();
    int chunksNb = fileSize / ChunkSize + 1;

    emit updateProgress(0.);

    for(int chunkIdx = 0; chunkIdx < chunksNb; chunkIdx++) {
        if(!inputFile.isReadable()) {
            emit fileError(READ_ERROR, fileName);
            return;
        }

        quint64 readed = inputFile.read(internalBuffer.data(), ChunkSize);
        QStringList rawWords = QString(internalBuffer.left(readed)).simplified().split(" ", Qt::SkipEmptyParts);

        int chunkProgress = 0;
        for(QString &word: rawWords) {
            if(isWord(word)) {
                promoteWord(word);
            }
            chunkProgress++;
            emit updateProgress((chunkIdx + float(chunkProgress) / rawWords.size()) / chunksNb);
        }
    }
    inputFile.close();

    emit updateProgress(1.);
}
