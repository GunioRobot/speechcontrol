#ifndef SENTENCE_HPP
#define SENTENCE_HPP

#include <QDir>
#include <QUuid>
#include <QObject>

class QDomElement;

namespace SpeechControl {
    class Corpus;
    class Phrase;
    class Sentence;

    typedef QList<Sentence*> SentenceList;
    typedef QList<Phrase*> PhraseList;

    class Phrase : public QObject {
        Q_OBJECT
        Q_DISABLE_COPY(Phrase)
        friend class Sentence;

    public:
        virtual ~Phrase();
        QFile* audio() const;
        const QString text() const;
        const int index() const;
        const bool isCompleted() const;

    private:
        explicit Phrase(const Sentence*, const int& );
        const Sentence* m_sntnc;
        const int m_indx;
    };

    class Sentence : public QObject {
        Q_OBJECT
        Q_DISABLE_COPY(Sentence)
        friend class Corpus;
        friend class Phrase;

    public:
        virtual ~Sentence();
        static Sentence* create(Corpus* , const QString& );
        Corpus* parentSession() const;
        const QUuid uuid() const;
        const QString text() const;
        const QDir audioPath() const;
        const PhraseList phrases() const;
        const bool allPhrasesCompleted() const;
        const bool isPhraseCompleted(const int& ) const;
        const int index() const;
        Phrase* phrase(const int& ) const;

    private:
        explicit Sentence(Corpus*, QDomElement* );
        QDomElement* getPhraseElement(const int& ) const;
        QDomElement* m_elem;
        Corpus* m_corpus;
        PhraseList m_phrsLst;
    };

}

#endif // SENTENCE_HPP
