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

#include <stdio.h>

#include <QDir>
#include <QDebug>
#include <QTextStream>

#include "session.hpp"
#include <corpus.hpp>
#include <sentence.hpp>

using namespace SpeechControl;

QMap<QUuid, QDomElement*> Session::s_elems;
QDomDocument* Session::s_dom = 0;
ContentMap Content::s_lst;

Session::Session(const QUuid& p_uuid) : m_corpus(0), m_content(0), m_elem(0){
    load(p_uuid);
}

Session::~Session()
{
}

Corpus * Session::corpus() const
{
    return m_corpus;
}

/// @todo Invoke the progress of this session when loaded.
void Session::setCorpus(Corpus* l_corpus)
{
    if (l_corpus){
        m_corpus = l_corpus;
        assessProgress();
    } else {
        qDebug() << "Null corpus not added.";
    }
}

Content* Session::content() const
{
    return m_content;
}

void Session::setContent(Content* l_content)
{
    m_content = l_content;
    assessProgress();
}

void Session::assessProgress()
{
    double l_progress;
    if (m_corpus){
        Sentence* l_snt = this->firstIncompleteSentence();
        if (l_snt){
            const int l_index = m_corpus->sentences().indexOf(l_snt);
            l_progress = (double) l_index / (double) m_corpus->sentences().count();

        } else
            l_progress = 1.0;
    } else
        l_progress = 0.0;

    emit progressChanged(l_progress);
}

void Session::init(){
    qDebug() << "Loading sessions...";
    QFile* l_file = new QFile(QDir::homePath() + "/.speechcontrol/sessions.xml");
    s_elems.clear();

    if (s_dom){
        s_dom->clear();
        delete s_dom;
    }

    s_dom = new QDomDocument("Sessions");

    if (l_file->exists()){
        l_file->open(QIODevice::ReadOnly);
        s_dom->setContent(l_file);

        const QDomElement l_domElem = s_dom->documentElement();
        const QDomNodeList l_domList = l_domElem.elementsByTagName("Session");

        for (int i = 0; i < l_domList.count(); i++){
            QDomElement l_node = l_domList.at(i).toElement();
            const QUuid l_uuid(l_node.attribute("uuid"));
            s_elems.insert(l_uuid,new QDomElement(l_domList.at(i).toElement()));
        }
        qDebug() << l_domList.count() << "sessions loaded.";
    } else {
        l_file->open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDomElement l_elem = s_dom->createElement("Sessions");
        s_dom->appendChild(l_elem);
        l_file->write(s_dom->toString(4).toLocal8Bit());
        qDebug() << "Created session listing.";
    }

    l_file->close();
}

void Session::load(const QUuid &p_uuid)
{
    m_elem = s_elems.value(p_uuid);
    setCorpus(Corpus::obtain(m_elem->attribute("corpus")));
    setContent(Content::obtain(m_elem->attribute("content")));
}


SessionList Session::allSessions()
{
    SessionList l_lst;
    Q_FOREACH(const QUuid l_uuid, s_elems.keys()){
        Session* l_session = Session::obtain(l_uuid);
        l_lst << l_session;
    }

    return l_lst;
}

const QUuid Session::uuid() const
{
    return m_elem->attribute("uuid");
}

Session* Session::obtain(const QUuid &p_uuid)
{
    qDebug() << "Obtaining session" << p_uuid;
    return new Session(p_uuid);
}

/// @todo Create a new @see Corpus with this @see Session.
Session* Session::create(const Content* p_content)
{
    const QStringList l_lst = p_content->pages().join("\n").simplified().trimmed().replace(".",".\n").split("\n",QString::SkipEmptyParts);
    qDebug () << "Session text:" << l_lst;
    const QUuid l_uuid = QUuid::createUuid();
    QDomElement l_sessElem = s_dom->createElement("Session");
    s_dom->documentElement().appendChild(l_sessElem);
    l_sessElem.setAttribute("uuid",l_uuid.toString());
    l_sessElem.setAttribute("content",p_content->uuid().toString());
    l_sessElem.setAttribute("corpus",Corpus::create(l_lst)->uuid());

    QFile* l_file = new QFile(QDir::homePath() + "/.speechcontrol/sessions.xml");
    l_file->open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream l_str(l_file);
    s_dom->save(l_str,4);
    l_file->close();

    init();

    return Session::obtain(l_uuid);
}

Content::Content(const QUuid& p_uuid) {
    load(p_uuid);
}

Content* Content::obtain(const QUuid &p_uuid)
{
    if (!s_lst.contains(p_uuid))
        s_lst.insert(p_uuid,(new Content(p_uuid)));

    return s_lst.value(p_uuid);
}

void Content::load(const QUuid &p_uuid)
{
    QFile* l_file = new QFile(getPath(p_uuid));
    QByteArray l_data;
    if (l_file->open(QIODevice::ReadOnly)){
        m_dom = new QDomDocument("Content");
        m_dom->setContent(l_file);

        // Chunk up the text every 200 characters.
        QString l_text;
        QString l_textBase = m_dom->documentElement().elementsByTagName("Content").at(0).toElement().text();
        uint l_count = 0;

        Q_FOREACH(const QChar l_chr, l_textBase){
            if (l_count == 200){
                m_pages << l_text;
                l_count = 0;
            }

            if (l_chr.isLetterOrNumber())
                ++l_count;

            l_text += l_chr;
        }

        m_uuid = p_uuid;
    } else
        qDebug() << "Can't open Content XML document.";
}

/// @todo Determine a means of finding the page count.
/// @todo Define the size of one page.
const uint Content::pageCount() const
{
    return m_pages.count();
}

/// @todo Determine a means of finding the words in the text.
const uint Content::words() const
{
    return m_pages.join("\n").split(" ").count();
}

const uint Content::length() const
{
    return m_pages.join("\n").length();
}

/// @todo Determine a means of finding the count of alpha-numeric characters.
const uint Content::characters() const
{
    return m_pages.join("\n").length();
}

QString Content::getPath(const QUuid &p_uuid)
{
    return QDir::homePath() + "/.speechcontrol/contents/" + p_uuid.toString();
}

/// @todo Find a cleaner, safer way of finding this info.
const QString Content::title() const
{
    QDomElement l_domElem = m_dom->documentElement();
    QDomElement l_bilboElem = l_domElem.namedItem("Bibliography").toElement();
    return l_bilboElem.attribute("Title");
}

/// @todo Find a cleaner, safer way of finding this info.
const QString Content::author() const
{
    QDomElement l_domElem = m_dom->documentElement();
    QDomElement l_bilboElem = l_domElem.namedItem("Bibliography").toElement();
    return l_bilboElem.attribute("Author");
}

Content::~Content()
{
    // What to clean up?
}

const QUuid Content::uuid() const
{
    return m_uuid;
}

ContentList Content::allContents()
{
    ContentList l_lst;
    QDir l_dir(QDir::homePath() + "/.speechcontrol/contents/");
    l_dir.setFilter(QDir::Files);
    QStringList l_results = l_dir.entryList(QStringList() << "*");

    Q_FOREACH(const QString l_uuid, l_results){
        l_lst << Content::obtain(QUuid(l_uuid));
    }

    return l_lst;
}

const QStringList SpeechControl::Content::pages() const
{
    return m_pages;
}

const QString SpeechControl::Content::pageAt(const int &l_index) const
{
    if (l_index < m_pages.count())
        return m_pages.at(l_index);

    return QString::null;
}

Content * Content::create(const QString &p_author, const QString &p_title, const QString &p_content)
{
    QUuid l_uuid = QUuid::createUuid();
    QDomDocument* l_dom = new QDomDocument("Content");
    QDomElement l_domElem = l_dom->createElement("Content");
    l_domElem.setAttribute("Uuid",l_uuid);
    l_dom->appendChild(l_domElem);

    QDomElement l_bilboElem = l_dom->createElement("Bibliography");
    l_bilboElem.setAttribute("Author",p_author);
    l_bilboElem.setAttribute("Title",p_title);
    l_domElem.appendChild(l_bilboElem);

    QDomElement l_textElem = l_dom->createElement("Text");
    QDomText l_textNode = l_dom->createTextNode(p_content);
    l_textElem.appendChild(l_textNode);
    l_domElem.appendChild(l_textElem);

    const QString l_path = Content::getPath(l_uuid);
    QFile* l_file = new QFile(l_path);
    l_file->open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream l_strm(l_file);
    l_dom->save(l_strm,4);

    return Content::obtain(l_uuid);
}

Sentence* Session::firstIncompleteSentence() const
{
    const SentenceList l_lst = m_corpus->sentences();
    for (int i = 0; i < l_lst.count(); i++){
        Sentence* l_sent = l_lst.at(i);

        if (!l_sent->allPhrasesCompleted())
            return l_sent;
        else
            continue;
    }

    return 0;
}

Sentence* Session::lastIncompleteSentence() const
{
    const SentenceList l_lst = m_corpus->sentences();
    SentenceList::ConstIterator l_endItr = l_lst.begin();
    for (SentenceList::ConstIterator l_itr = l_lst.end(); l_itr != l_endItr; l_itr--){
        const Sentence* l_sent = (*l_itr);

        if (!l_sent->allPhrasesCompleted())
            return *l_itr;
        else
            continue;
    }

    return 0;
}

SentenceList Session::incompletedSentences() const
{
    return SentenceList();
}

const bool Session::isCompleted() const
{
    return !(incompletedSentences().empty());
}
