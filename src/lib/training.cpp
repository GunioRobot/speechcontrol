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

Corpus::Corpus(const QUuid& p_uuid) : QObject(), m_dom(new QDomDocument){
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
void Corpus::addSentence(Sentence *l_phrs)
{
    m_sntncLst.insert(l_phrs->uuid(),l_phrs);
}

void Corpus::addSentence(const QString &l_txt, const QFile *l_audio){
    Sentence* l_sentence = Sentence::create(this);
    l_sentence->m_elem->attribute(QUrl::fromLocalFile(l_audio->fileName()).toString());
    l_sentence->m_elem->attribute(l_txt);
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

Corpus * Corpus::create()
{
    QUuid l_uuid = QUuid::createUuid();
    QDir l_dir;
    if (!l_dir.mkpath(getPath(l_uuid).toLocalFile())){
        qWarning() << "Can't make session" << l_uuid;
        return 0;
    }

    QDomDocument l_dom("Session");
    QDomElement l_rootElem = l_dom.createElement("Session");
    QDomElement l_dateElem = l_dom.createElement("Date");

    l_rootElem.setAttribute("uuid",l_uuid.toString());
    l_dateElem.setAttribute("Created",QDateTime::currentDateTimeUtc().toString());
    l_dom.appendChild(l_rootElem).appendChild(l_dateElem);

    const QUrl l_path = getPath(l_uuid);
    QFile* l_file = new QFile(l_path.toLocalFile());
    if (l_file->open(QIODevice::WriteOnly | QIODevice::Truncate)){
        QTextStream l_strm(l_file);
        l_dom.save(l_strm,4);
    }

    l_file->close();
    l_file->deleteLater();
}

const bool Corpus::exists(const QUuid& l_uuid){
    return QFile::exists(getPath(l_uuid).toLocalFile());
}

QUrl Corpus::getPath(const QUuid &l_uuid)
{
    return QDir::homePath() + "./speechcontrol/corpus/" + l_uuid.toString();
}

QUrl Corpus::audioPath()
{
    return getPath(this->uuid()).toLocalFile() + "/audio";
}

const QUuid Corpus::uuid() const
{
    return m_dom->documentElement().attribute("uuid");
}

Corpus* Corpus::obtain(const QUuid &l_uuid)
{
    if (!QFile::exists(getPath(l_uuid).toLocalFile()))
        return 0;

    return new Corpus(l_uuid);
}

void Corpus::load(const QUuid &p_uuid)
{
    const QUrl l_path = getPath(p_uuid);
    QFile* l_file = new QFile(l_path.toLocalFile());

    if (l_file->open(QIODevice::ReadOnly)){
        if (!m_dom)
            m_dom = new QDomDocument;

        m_dom->setContent(l_file);

        // get phrases
        QDomNodeList l_elems = m_dom->documentElement().firstChildElement("Sentences").childNodes();

        for (int i = 0; i < l_elems.count(); ++i){
            QDomElement l_elem = l_elems.at(i).toElement();
            Sentence* l_phrs = new Sentence(this,&l_elem);
            m_sntncLst.insert(l_phrs->uuid(),l_phrs);
        }
    }
}

void Corpus::save()
{
    const QUrl l_path = getPath(this->uuid());
    QFile* l_file = new QFile(l_path.toLocalFile());
    if (l_file->open(QIODevice::WriteOnly | QIODevice::Truncate)){
        QTextStream l_strm(l_file);
        m_dom->save(l_strm,4);
    }
}

CorpusList Corpus::allCorpuses()
{
    CorpusList l_lst;
    QDir l_dir(QDir::homePath() + "./speechcontrol/corpus/");
    l_dir.setFilter(QDir::Dirs);
    QStringList l_results = l_dir.entryList(QStringList() << "*");
    Q_FOREACH(const QString& l_uuid, l_results){
        l_lst << Corpus::obtain(QUuid(l_uuid));
    }

    return l_lst;
}

/// @todo What to clean-up?
Corpus::~Corpus()
{

}

Sentence::Sentence(Corpus* p_sess, QDomElement *p_elem) : m_elem(p_elem), m_sess(p_sess)
{
}

Corpus * Sentence::parentSession() const
{
    return m_sess;
}

QUuid Sentence::uuid() const
{
    return QUuid(m_elem->attribute("uuid"));
}

QFile * Sentence::audio() const
{
    const QString l_pth = m_sess->audioPath().toLocalFile() + "/" + m_elem->attribute("file");
    return new QFile(l_pth);
}

QString Sentence::text() const {
    return m_elem->attribute("text");
}

/// @todo This method should add itself to the parent session.
Sentence * Sentence::create(Corpus *l_sess)
{
    QDomElement l_elem = l_sess->m_dom->createElement("Sentence"),
            l_rootElem = l_sess->m_dom->documentElement().namedItem("Sentences").toElement();

    l_rootElem.appendChild(l_elem);
    l_elem.setAttribute("file",QString::null);
    l_elem.setAttribute("uuid",QUuid::createUuid());
    Sentence* l_phrs = new Sentence(l_sess,&l_elem);
    l_sess->addSentence(l_phrs);

    return l_phrs;
}

Sentence::~Sentence()
{

}

void Dictionary::load(const QUuid &l_uuid)
{
    QFile* l_file = new QFile(getPath(l_uuid));
    l_file->open(QIODevice::ReadOnly);
    QTextStream l_strm(l_file);

    while (!l_strm.atEnd()){
        const QString l_line = l_strm.readLine();
        const QStringList l_tokens = l_line.split("\t",QString::SkipEmptyParts);
        m_words.insert(l_tokens[0],new DictionaryEntry(this,l_tokens[0],l_tokens[1]));
    }
}

const QString Dictionary::getPath(const QUuid &l_uuid)
{
    return QDir::homePath() + "./speechcontrol/dictionaries/" + l_uuid.toString() + ".dic";
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

Dictionary * Dictionary::obtain(const QUuid &)
{
}

DictionaryEntryList * Dictionary::entries() const
{
}

void Dictionary::addEntry(DictionaryEntry *)
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

const QString SpeechControl::Corpus::title() const
{
    return m_dom->documentElement().elementsByTagName("Biblography").at(0).toElement().attribute("Title");
}

const QString SpeechControl::Corpus::author() const
{
    return m_dom->documentElement().elementsByTagName("Biblography").at(0).toElement().attribute("Author");
}
