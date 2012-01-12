#ifndef CORPUS_HPP
#define CORPUS_HPP

#include <QUrl>
#include <QUuid>
#include <QObject>
#include <QStringList>

#include "sentence.hpp"

class QDomDocument;

namespace SpeechControl {
    class Corpus;
    class Sentence;
    class Dictionary;

    typedef QList<Corpus*> CorpusList;
    typedef QMap<QUuid, Corpus*> CorpusMap;

    class Corpus : public QObject {
        Q_OBJECT
        Q_DISABLE_COPY(Corpus)
        Q_PROPERTY(SentenceList Sentences READ sentences)
        Q_PROPERTY(Dictionary* Dictionary READ dictionary)
        Q_PROPERTY(QUuid Uuid READ uuid)
        friend class Sentence;
        friend class Dictionary;

    public:
        Corpus(const QUuid& );
        virtual ~Corpus();
        static Corpus* obtain(const QUuid&);
        static Corpus* create(const QStringList&);
        static CorpusList allCorpuses();
        static const bool exists(const QUuid&);

        Sentence* addSentence(Sentence*);
        Sentence* addSentence(const QString&, const QFile*);
        Corpus& operator<<(Sentence*);
        Corpus& operator<<(SentenceList&);

        const QDateTime timeStarted() const;
        const QDateTime timeLastModified() const;
        const QDateTime timeCompleted() const;
        Dictionary* dictionary() const;
        SentenceList sentences() const;
        Sentence* sentenceAt(const int&) const;
        const QUuid uuid() const;

    public slots:
        void load(const QUuid&);
        void save();

    private:
        static QUrl getPath(const QUuid&);
        static CorpusMap s_lst;

        QUrl audioPath() const;
        QDomDocument* m_dom;
        SentenceList m_sntncLst;
        Dictionary* m_dict;
    };

}
#endif // CORPUS_HPP
