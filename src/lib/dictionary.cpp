/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
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

#include "dictionary.hpp"
#include "corpus.hpp"

#include <QDir>
#include <QFile>
#include <QDebug>
#include <QDomDocument>

using namespace SpeechControl;


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
        QObject(p_dict), m_dict(p_dict), m_word(p_word), m_phnm(p_phoneme)
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
    return m_words.take(p_word);
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

/// @todo Implement the saving ability.
void Dictionary::save()
{
}

Dictionary::Dictionary(const QUuid &p_uuid)
{
    load(p_uuid);
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

Phrase * Sentence::phrase(const int &p_indx) const
{
    return m_phrsLst.at(p_indx);
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

const int Sentence::index() const
{
    return m_elem->attribute("index").toInt();
}

SpeechControl::Sentence* SpeechControl::Corpus::sentenceAt(const int &p_indx) const
{
    return m_sntncLst.at(p_indx);
}
