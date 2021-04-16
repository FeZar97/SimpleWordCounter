#include "simplewordcounter.h"

// reset fields of worker to initial state
void SimpleWordCounter::reset() {
    forcedStop = false;
    isWorking = false;
    currentProgress = 0.;
    totalWordsNb = 0;
    popularityToWords.clear();
    wordToPopularity.clear();
    words.clear();
    prevUiUpdateTime = QDateTime::currentDateTime();

    emit updateProgress(currentProgress);
}

// check file existing and try open it
bool SimpleWordCounter::tryOpenFile(QFile &file){
    if(!file.exists()) {
        isWorking = false;
        emit fileError(QString("File [%1] not exist.").arg(file.fileName()));
        return true;
    }
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        isWorking = false;
        emit fileError(QString("File [%1] can't opened.").arg(file.fileName()));
        return true;
    }
    return false;
}

// check file available, while the worker is running
bool SimpleWordCounter::checkFileAvailable(QFile &file) {
    if(!file.isReadable()) {
        isWorking = false;
        emit fileError(QString("File [%1] is not readable.").arg(file.fileName()));
        return true;
    }
    return false;
}

// checking the condition that string 'str' is a word
bool SimpleWordCounter::isWord(QString &str) const {
    str = str.toLower();
    return str.length() > 1;
}

// updating information about given word
void SimpleWordCounter::promoteWord(const QString &word) {
    quint64 prevPopularity;
    QStringView wordView = *(words.insert(word));

    if(wordToPopularity.count(wordView)) {
        prevPopularity = wordToPopularity[word];
        wordToPopularity[wordView] = prevPopularity + 1;
        popularityToWords[prevPopularity].removeOne(wordView);
        popularityToWords[prevPopularity + 1].push_back(wordView);
        if(popularityToWords[prevPopularity].empty()) {
            popularityToWords.remove(prevPopularity);
        }
    } else {
        wordToPopularity[wordView] = 1;
        popularityToWords[1].push_back(wordView);
    }
}

// create and return object with information about [nbThreshold] most popular words
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

    QVariantList mostPopularWords;
    QList<QVariant> tempList;
    for(const QString &key: words.keys()) {
        tempList.append(key);
        tempList.append(words[key]);
        mostPopularWords.append(QVariant(tempList));
        tempList.clear();
    }

    return mostPopularWords;
}

// emitting signals with current information
void SimpleWordCounter::updateUi() {
    emit updateProgress(currentProgress);
    emit updateStatistics(totalWordsNb);
    emit updateWords(mostPopularWords(PopularWordsNb));
}

// reads block of bytes, taking into account the correct occurrence of last word in the block
qint64 SimpleWordCounter::readCorrectnessChunk(QFile &inputFile) {
    qint64 prevPos = inputFile.pos();
    qint64 maxToRead = qMin(inputFile.size() - prevPos, ChunkSize);
    qint64 lastReadedSize = inputFile.read(internalBuffer.data(), maxToRead);

    if(lastReadedSize) {
        qint64 lastSpacePos = 0;
        for(qint64 pos = lastReadedSize - 1; pos >= 0; pos--) {
            if(internalBuffer.data()[pos] == ' ') {
                lastSpacePos = pos;
                break;
            }
        }

        lastReadedSize = lastSpacePos;
    }
    inputFile.seek(prevPos + lastReadedSize);

    return lastReadedSize;
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
    if(tryOpenFile(inputFile)) {
        return;
    }

    isWorking = true;
    qint64 chunksNb = inputFile.size() / ChunkSize + 1,
           lastReadedSize = 0;

    for(int chunkIdx = 0; chunkIdx < chunksNb; chunkIdx++) {

        if(checkFileAvailable(inputFile)) {
            return;
        }

        // find correctness end of last word in chunk
        lastReadedSize = readCorrectnessChunk(inputFile);
        QStringList rawWords = QString(internalBuffer.left(lastReadedSize)).simplified().split(" ", Qt::SkipEmptyParts);

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

            currentProgress = (chunkIdx + (float(chunkProgress) / rawWords.size())) / chunksNb;

            if(prevUiUpdateTime.msecsTo(QDateTime::currentDateTime()) > UiUpdatePeridMsecs) {
                prevUiUpdateTime = QDateTime::currentDateTime();
                updateUi();
            }
        }
    }

    updateUi();

    inputFile.close();
    isWorking = false;

    emit finished();
}
