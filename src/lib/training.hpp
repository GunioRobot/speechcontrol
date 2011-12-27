/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 Jacky Alcine <jacky.alcine@thesii.org>
 *
 * SpeechControl is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * SpeechControl is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with SpeechControl; if not, write to the
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef TRAINING_HPP
#define TRAINING_HPP

#include <QMap>
#include <QList>
#include <QUuid>
#include <QObject>
#include <QDateTime>

class QUrl;
class QFile;
class QDomDocument;
class QDomElement;

namespace SpeechControl {
class Corpus;
class Phrase;
class Sentence;
class Dictionary;
class DictionaryEntry;

typedef QList<Corpus*> CorpusList;
typedef QList<Sentence*> SentenceList;
typedef QList<Dictionary*> DictionaryList;
typedef QList<DictionaryEntry*> DictionaryEntryList;

typedef QMap<QUuid, Corpus*> CorpusMap;
typedef QMap<QUuid, Sentence*> SentenceMap;
typedef QMap<QUuid, Dictionary*> DictionaryMap;
typedef QMap<QString, DictionaryEntry*> DictionaryEntryMap;

class DictionaryEntry : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(DictionaryEntry)
    friend class Dictionary;

public:
    DictionaryEntry(Dictionary*, const QString&, const QString&);
    virtual ~DictionaryEntry();
    QString word() const;
    QString phoneme() const;

private:
    Dictionary* m_dict;
    QString m_word;
    QString m_phnm;
};

class Dictionary : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Dictionary)
    friend class Corpus;

public:
    Dictionary(const QUuid&);
    virtual ~Dictionary();
    static Dictionary* obtain(const QUuid&);
    DictionaryEntryList* entries() const;
    void addEntry(DictionaryEntry*);
    DictionaryEntry* removeEntry(const QString&);
    Dictionary& operator<<(DictionaryEntry*);
    Dictionary& operator<<(DictionaryEntryList&);

public slots:
    void load(const QUuid&);
    void save();

private:
    static const QString getPath(const QUuid&);
    static DictionaryMap s_lst;
    DictionaryEntryMap m_words;
};

class Phrase : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Phrase)

public:
    Phrase(const Sentence*, const int&);
    virtual ~Phrase();
};

class Sentence : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Sentence)
    friend class Corpus;

public:
    virtual ~Sentence();
    static Sentence* create(Corpus* , QString l_txt = QString());
    Corpus* parentSession() const;
    QUuid uuid() const;
    QFile* audio() const;
    QString text() const;
    QStringList words() const;

private:
    explicit Sentence(Corpus*, QDomElement* );
    QDomElement* m_elem;
    Corpus* m_corpus;
};

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
    Sentence* sentence(const QUuid&) const;
    const QUuid uuid() const;

public slots:
    void load(const QUuid&);
    void save();

private:
    static QUrl getPath(const QUuid&);
    static CorpusMap s_lst;

    QUrl audioPath() const;
    QDomDocument* m_dom;
    SentenceMap m_sntncLst;
    Dictionary* m_dict;
};

}

#endif // TRAINING_HPP
