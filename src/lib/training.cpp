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

#include "training.hpp"
#include <QDomDocument>
#include <QDomElement>
#include <QDebug>
#include <QUuid>
#include <QDir>
#include <QFile>
#include <QUrl>

using namespace SpeechControl;

Corpus::Corpus(const QUuid& p_uuid) : QObject(), m_dom(new QDomDocument) {
    load(p_uuid);
}

SentenceList Corpus::sentences() const
{
    return m_sntncLst.values();
}

Sentence * Corpus::sentence(const QUuid &l_key) const
{
    if (m_sntncLst.contains(l_key))
        return m_sntncLst.value(l_key);
    else
        return 0;
}

/// @todo Add the Sentence to the XML document and then to the list.
Sentence* Corpus::addSentence(Sentence *l_phrs)
{
    m_sntncLst.insert(l_phrs->uuid(),l_phrs);
    return l_phrs;
}

Sentence* Corpus::addSentence(const QString &l_txt, const QFile *l_audio)
{
    qDebug() << "Adding sentence" << l_txt << "...";
    Sentence* l_sentence = Sentence::create(this,l_txt);

    if (l_audio)
        l_sentence->m_elem->attribute(QUrl::fromLocalFile(l_audio->fileName()).toString());

    return l_sentence;
}

Corpus & Corpus::operator <<(Sentence *l_phrs)
{
    this->addSentence(l_phrs);
    return *this;
}

/// @todo Just invoke the above method.
Corpus & Corpus::operator <<(SentenceList &l_lst)
{
    foreach (Sentence* l_phrs, l_lst)
    this->addSentence(l_phrs);

    return *this;
}

/// @todo Build a dictionary from the said text.
Corpus * Corpus::create(const QStringList& p_text)
{
    QUuid l_uuid = QUuid::createUuid();
    QDir l_dir;
    if (!l_dir.mkpath(getPath(l_uuid).toLocalFile())) {
        qWarning() << "Can't make corpus" << l_uuid;
        return 0;
    }

    QDomDocument l_dom("Corpus");
    QDomElement l_rootElem = l_dom.createElement("Corpuses");
    QDomElement l_dateElem = l_dom.createElement("Timing");
    QDomElement l_sentencesElem = l_dom.createElement("Sentences");

    l_rootElem.setAttribute("Uuid",l_uuid.toString());
    l_rootElem.setAttribute("Version",0.01);
    l_dateElem.setAttribute("Created",QDateTime::currentDateTimeUtc().toString());

    l_rootElem.appendChild(l_dateElem);
    l_rootElem.appendChild(l_sentencesElem);
    l_dom.appendChild(l_rootElem);

    const QUrl l_path = getPath(l_uuid);
    QFile* l_file = new QFile(l_path.toLocalFile() + "/corpus.xml");
    l_file->open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream l_strm(l_file);
    l_dom.save(l_strm,4);

    Corpus* l_corpus = Corpus::obtain(l_uuid);
    Q_FOREACH(const QString& l_str, p_text) {
        Sentence* l_sent = l_corpus->addSentence(l_str.simplified(),0);
        l_corpus->m_dom->documentElement().namedItem("Sentences").appendChild(*l_sent->m_elem);
    }

    l_corpus->save();
    return l_corpus;
}

const bool Corpus::exists(const QUuid& l_uuid) {
    return QFile::exists(getPath(l_uuid).toLocalFile());
}

QUrl Corpus::getPath(const QUuid &l_uuid)
{
    return QDir::homePath() + "/.speechcontrol/corpus/" + l_uuid.toString();
}

QUrl Corpus::audioPath() const
{
    return getPath(this->uuid()).toLocalFile() + "/audio";
}

const QUuid Corpus::uuid() const
{
    return m_dom->documentElement().attribute("Uuid");
}

Corpus* Corpus::obtain(const QUuid &l_uuid)
{
    const QString l_path = getPath(l_uuid).toLocalFile() + "/corpus.xml";

    if (!QFile::exists(l_path)) {
        qDebug() << "Corpus not found at" << l_path;
        return 0;
    }

    return new Corpus(l_uuid);
}

void Corpus::load(const QUuid &p_uuid)
{
    const QUrl l_path = getPath(p_uuid);
    QFile* l_file = new QFile(l_path.toLocalFile() + "/corpus.xml");

    if (l_file->exists() && l_file->open(QIODevice::ReadOnly)) {
        if (!m_dom)
            m_dom = new QDomDocument("Corpus");

        if (!m_dom->setContent(l_file)) {
            qDebug() << "Failed to load corpus.";
            return;
        }

        QDomNodeList l_elems = m_dom->documentElement().firstChildElement("Sentences").childNodes();

        for (int i = 0; i < l_elems.count(); ++i) {
            QDomElement l_elem = l_elems.at(i).toElement();
            qDebug() << "Loading sentence:" << l_elem.attribute("uuid");

            if (l_elem.isNull()) continue;

            Sentence* l_sntc = new Sentence(this,(new QDomElement(l_elems.at(i).toElement())));
            qDebug() << "Loaded sentence:" << l_sntc->text();
            addSentence(l_sntc);
        }
    } else
        qDebug() << "Failed to open corpus XML file.";
}

void Corpus::save()
{
    const QUrl l_path = getPath(this->uuid());
    QFile* l_file = new QFile(l_path.toLocalFile() + "/corpus.xml");
    if (l_file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream l_strm(l_file);
        m_dom->save(l_strm,4);
    } else
        qDebug() << "Can't open:" << l_file->errorString();
}

CorpusList Corpus::allCorpuses()
{
    CorpusList l_lst;
    QDir l_dir(QDir::homePath() + "/.speechcontrol/corpus/");
    l_dir.setFilter(QDir::Dirs);
    QStringList l_results = l_dir.entryList(QStringList() << "*");
    Q_FOREACH(const QString& l_uuid, l_results) {
        l_lst << Corpus::obtain(QUuid(l_uuid));
    }

    return l_lst;
}

/// @todo What to clean-up?
Corpus::~Corpus()
{

}

/// @todo Drop the addition of the element and have it request it from the base Corpus.
Sentence::Sentence(Corpus* p_corpus, QDomElement *p_elem) : m_elem(p_elem), m_corpus(p_corpus)
{
    // Build phrases.
    QDomNodeList l_nodes = m_elem->childNodes();
    for (int i = 0; i < l_nodes.length(); i++)
        m_phrsLst << new Phrase(this,i);
}

Corpus * Sentence::parentSession() const
{
    return m_corpus;
}

const QUuid Sentence::uuid() const
{
    return QUuid(m_elem->attribute("uuid"));
}

const QDir Sentence::audioPath() const
{
    return QDir(m_corpus->audioPath().toLocalFile() + "/" + m_elem->attribute("file"));
}

/// @todo Merge all of the phrases together.
const QString Sentence::text() const {
    QString l_text;

    Q_FOREACH(const Phrase* l_phrs, m_phrsLst){
        l_text += l_phrs->text() + " ";
    }

    return l_text.trimmed();
}

Sentence* Sentence::create(Corpus *l_sess, const QString& l_txt)
{
    const QUuid l_uuid = QUuid::createUuid();
    QDomElement* l_elem = new QDomElement(l_sess->m_dom->createElement("Sentence"));
    l_elem->setAttribute("file",QUuid::createUuid());
    l_elem->setAttribute("uuid",l_uuid);

    // form phrases
    QStringList l_words = l_txt.split(" ",QString::SkipEmptyParts);
    QString l_phrase;

    for (int i = 0, j = 0; i < l_words.count(); i++, j++){
        if (j < 4)
            l_phrase += l_words.at(i) + " ";

        if (j == 3 || i + 2 == l_words.count() || i + 1 == l_words.count()){ // build phrase
            l_phrase = l_phrase.trimmed();

            QDomElement* l_phrsElem = new QDomElement(l_sess->m_dom->createElement("Phrase"));
            l_phrsElem->setAttribute("uuid",QUuid::createUuid());
            l_phrsElem->appendChild(l_sess->m_dom->createTextNode(l_phrase.toLocal8Bit().toBase64()));
            l_elem->appendChild(*l_phrsElem);

            j = -1;
            l_phrase.clear();
        }
    }

    return l_sess->addSentence(new Sentence(l_sess,l_elem));
}

Sentence::~Sentence()
{
    // What to clean up? :P
}

void Dictionary::load(const QUuid &l_uuid)
{
    QFile* l_file = new QFile(getPath(l_uuid));
    l_file->open(QIODevice::ReadOnly);
    QTextStream l_strm(l_file);

    while (!l_strm.atEnd()) {
        const QString l_line = l_strm.readLine();
        const QStringList l_tokens = l_line.split("\t",QString::SkipEmptyParts);
        m_words.insert(l_tokens[0],new DictionaryEntry(this,l_tokens[0],l_tokens[1]));
    }
}

const QString Dictionary::getPath(const QUuid &l_uuid)
{
    return QDir::homePath() + "/.speechcontrol/dictionaries/" + l_uuid.toString() + ".dic";
}

DictionaryEntry::DictionaryEntry(Dictionary *p_dict, const QString &p_word, const QString &p_phoneme) :
        m_dict(p_dict), m_word(p_word), m_phnm(p_phoneme), QObject(p_dict)
{
}

DictionaryEntry::~DictionaryEntry() {

}

QString DictionaryEntry::word() const
{
    return m_word;
}

QString DictionaryEntry::phoneme() const
{
    return m_phnm;
}

Dictionary* Dictionary::obtain(const QUuid &l_uuid)
{
    return 0;
}

DictionaryEntryList * Dictionary::entries() const
{
    return 0;
}

void Dictionary::addEntry(DictionaryEntry *l_entry)
{
}

DictionaryEntry * Dictionary::removeEntry(const QString& p_word)
{
    m_words.remove(p_word);
}

Dictionary& Dictionary::operator <<(DictionaryEntry *p_entry)
{
    m_words.insert(p_entry->word(),p_entry);
    return *this;
}

Dictionary& Dictionary::operator <<(DictionaryEntryList& p_lst)
{
    return *this;
}

void Dictionary::save()
{
}

Dictionary::Dictionary(const QUuid &p_uuid)
{
}

Dictionary::~Dictionary() {
}

Dictionary * Corpus::dictionary() const
{
    return m_dict;
}

const QDateTime Corpus::timeStarted() const
{
    return QDateTime::fromString(m_dom->elementsByTagName("Date").at(0).toElement().attribute("Started"));
}

const QDateTime Corpus::timeLastModified() const
{
    return QDateTime::fromString(m_dom->elementsByTagName("Date").at(0).toElement().attribute("LastModified"));
}

const QDateTime Corpus::timeCompleted() const
{
    return QDateTime::fromString(m_dom->elementsByTagName("Date").at(0).toElement().attribute("Completed"));
}

const QString Phrase::text() const
{
    QDomElement* l_elem = m_sntnc->getPhraseElement(m_indx);
    const QString l_base64Data = l_elem->text();
    qDebug() << l_base64Data << QByteArray::fromBase64(l_base64Data.toLocal8Bit());
    return QByteArray::fromBase64(l_base64Data.toLocal8Bit());
}

QFile* Phrase::audio() const
{
    const QString l_fileName = m_sntnc->getPhraseElement(m_indx)->attribute("uuid") + ".wav";
    const QString l_pth = m_sntnc->audioPath().path();
    return new QFile(l_pth + "/" + l_fileName);
}

const int Phrase::index() const
{
    return m_indx;
}

QDomElement* Sentence::getPhraseElement(const int &p_indx) const
{
    return new QDomElement(m_elem->elementsByTagName("Phrase").at(p_indx).toElement());
}

Phrase * Sentence::phrase(const int &) const
{
}

const PhraseList Sentence::phrases() const
{
    return m_phrsLst;
}

Phrase::Phrase(const Sentence *p_sntnct, const int &p_index) :
    m_sntnc(p_sntnct), m_indx(p_index)
{
}

Phrase::~Phrase() {

}

const bool Sentence::allPhrasesCompleted() const
{
    Q_FOREACH(const Phrase* l_phrs, m_phrsLst){
        if (!l_phrs->isCompleted())
            return false;
    }

    return true;
}

const bool Sentence::isPhraseCompleted(const int &p_indx) const
{
    return m_phrsLst.at(p_indx)->isCompleted();
}

const bool Phrase::isCompleted() const
{
    return audio()->exists();
}
