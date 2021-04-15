#include "simplewordcounter.h"

void SimpleWordCounter::reset() {
    forcedStop = false;
    isWorking = false;
    currentProgress = 0.;
    totalWordsNb = 0;
    popularityToWords.clear();
    wordToPopularity.clear();
    words.clear();

    emit updateProgress(currentProgress);
}

bool SimpleWordCounter::isWord(QString &str) const {
    str = str.toLower();
    return true;
}

void SimpleWordCounter::promoteWord(const QString &word) {
    quint64 prevPopularity;

    QStringView wordView = *(words.insert(word));

    if(wordToPopularity.count(wordView)) {
        prevPopularity = wordToPopularity[word];
        wordToPopularity[wordView] = prevPopularity + 1;
        popularityToWords[prevPopularity].removeOne(wordView);
        popularityToWords[prevPopularity + 1].push_back(wordView);
    } else {
        wordToPopularity[wordView] = 1;
        popularityToWords[1].push_back(wordView);
    }
}

QVariantList SimpleWordCounter::mostPopularWords(int nbThreshold) const {

    QMap<QString, Popularity> words;

    const QList<Popularity> &popularities = popularityToWords.keys();

    for(auto curPopularityIter = popularities.rbegin(); curPopularityIter != popularities.rend(); curPopularityIter++) {

        const Popularity curPopularity = *curPopularityIter;

        for(QStringView wordView: popularityToWords[curPopularity]) {
            words.insert(QString(wordView.data()), curPopularity);
            if(words.size() == nbThreshold) {
                break;
            }
        }

        if(words.size() == nbThreshold) {
            break;
        }
    }


    QVariantList wordList;
    QList<QVariant> tempList;

    for(const QString &key: words.keys()) {
        tempList.append(key);
        tempList.append(words[key]);
        wordList.append(QVariant(tempList));
        tempList.clear();
    }

    return wordList;
}

SimpleWordCounter::SimpleWordCounter(QObject *parent): QObject(parent) {
    internalBuffer.resize(ChunkSize);
}

void SimpleWordCounter::start(QString fileName) {

    if(isWorking) {
        return;
    }
    reset();

    QFile inputFile(fileName);
    if(!inputFile.exists()) {
        emit fileError(QString("File [%1] not exist.").arg(fileName));
        return;
    }
    if(!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit fileError(QString("File [%1] can't opened.").arg(fileName));
        return;
    }

    isWorking = true;
    quint64 fileSize = inputFile.size();
    int chunksNb = fileSize / ChunkSize + 1;

    for(int chunkIdx = 0; chunkIdx < chunksNb; chunkIdx++) {
        if(!inputFile.isReadable()) {
            emit fileError(QString("File [%1] is not readable.").arg(fileName));
            isWorking = false;
            return;
        }

        quint64 readed = inputFile.read(internalBuffer.data(), ChunkSize);
        QStringList rawWords = QString(internalBuffer.left(readed)).simplified().split(" ", Qt::SkipEmptyParts);

        int chunkProgress = 0;
        for(QString &word: rawWords) {

            if(forcedStop) {
                reset();
                emit finished();
                return;
            }

            if(isWord(word)) {
                promoteWord(word);
            }
            chunkProgress++;
            totalWordsNb++;

            currentProgress = (chunkIdx + float(chunkProgress) / rawWords.size()) / float(chunksNb);

            if(fabs(currentProgress - prevProgressUpdateVal) >= ProgressUpdateThreshold) {
                prevProgressUpdateVal = currentProgress;
                emit updateProgress(currentProgress);
                emit updateStatistics(totalWordsNb);
            }

            if(fabs(currentProgress - prevWordsUpdateVal) >= WordsUpdateThreshold) {
                prevWordsUpdateVal = currentProgress;
                emit updateWords(mostPopularWords(PopularWordsNb));
            }
        }
    }

    inputFile.close();
    isWorking = false;

    emit finished();
}
