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

#include "session.hpp"
#include <training.hpp>

#include <QDir>
#include <QDebug>
#include <QTextStream>

using SpeechControl::Corpus;
using SpeechControl::Content;
using SpeechControl::Session;
using SpeechControl::SessionMap;
using SpeechControl::SessionList;
using SpeechControl::ContentList;
using SpeechControl::ContentMap;

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
    m_corpus = l_corpus;
    assessProgress();
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
}

void Session::init(){
    qDebug() << "Loading sessions...";
    QFile l_file(QDir::homePath() + "/.speechcontrol/sessions.xml");
    s_elems.clear();

    if (s_dom){
        s_dom->clear();
        delete s_dom;
    }

    s_dom = new QDomDocument("Sessions");

    if (l_file.exists()){
        l_file.open(QIODevice::ReadOnly);
        s_dom->setContent(&l_file);

        const QDomElement l_domElem = s_dom->documentElement();
        const QDomNodeList l_domList = l_domElem.elementsByTagName("Session");

        for (int i = 0; i < l_domList.count(); i++){
            QDomElement *l_node = new QDomElement(l_domList.at(i).toElement());
            s_elems.insert(QUuid(l_node->attribute("uuid")),l_node);
        }
        qDebug() << l_domList.count() << "sessions loaded.";
    } else {
        l_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QTextStream l_strm(&l_file);
        QDomElement l_elem = s_dom->createElement("Sessions");
        s_dom->appendChild(l_elem);
        s_dom->save(l_strm,4);
        qDebug() << "Created session listing.";
    }
}

/// @bug A weird SIGSEGV occurs here.
void Session::load(const QUuid &p_uuid)
{
    m_elem = s_elems.value(p_uuid);
    const QUuid l_cntntUuid(m_elem->attribute("content"));
    const QUuid l_corpusUuid(m_elem->attribute("corpus"));
    m_content = Content::obtain(l_cntntUuid);
    m_corpus = Corpus::obtain(l_corpusUuid);
}


SessionList Session::allSessions()
{
    SessionList l_lst;
    foreach (QUuid l_uuid, s_elems.keys())
        l_lst << new Session(l_uuid);

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
    const QUuid l_uuid = QUuid::createUuid();
    QDomElement l_sessElem = s_dom->createElement("Session");
    s_dom->documentElement().appendChild(l_sessElem);
    l_sessElem.setAttribute("uuid",l_uuid.toString());
    l_sessElem.setAttribute("content",p_content->uuid().toString());
    l_sessElem.setAttribute("corpus",Corpus::create(p_content->pages().join("\n").split("\n.",QString::SkipEmptyParts))->uuid());

    QFile l_file(QDir::homePath() + "/.speechcontrol/sessions.xml");
    l_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream l_str(&l_file);
    s_dom->save(l_str,4);

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
    l_file->open(QIODevice::ReadOnly);
    m_dom = new QDomDocument("Content");
    m_dom->setContent(l_file->readAll());
    qDebug() << l_file->readAll();

    m_lines = m_dom->documentElement().elementsByTagName("Content").at(0).toElement().text().split("\n");
    m_uuid = p_uuid;
}

/// @todo Determine a means of finding the page count.
/// @todo Define the size of one page.
const uint Content::pageCount() const
{
    return length();
}

/// @todo Determine a means of finding the words in the text.
const uint Content::words() const
{
    return length();
}

const uint Content::length() const
{
    return m_lines.join("\n").length();
}

/// @todo Determine a means of finding the count of alpha-numeric characters.
const uint Content::characters() const
{
    return length();
}

QString Content::getPath(const QUuid &p_uuid)
{
    return QDir::homePath() + "/.speechcontrol/contents/" + p_uuid.toString();
}

/// @todo Find a cleaner, safer way of finding this info.
const QString Content::title() const
{
    QDomElement l_domElem = m_dom->documentElement();
    QDomElement l_bilboElem = l_domElem.namedItem("Bilbography").toElement();
    return l_bilboElem.attributes().namedItem("Title").toAttr().value();
}

/// @todo Find a cleaner, safer way of finding this info.
const QString Content::author() const
{
    QDomElement l_bilboElem = m_dom->documentElement().elementsByTagName("Bilbography").at(0).toElement();
    return l_bilboElem.attributes().namedItem("Author").toAttr().value();
}

Content::~Content()
{
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

    Q_FOREACH(const QString& l_uuid, l_results){
        l_lst << Content::obtain(QUuid(l_uuid));
    }

    return l_lst;
}

const QStringList SpeechControl::Content::pages() const
{
    return m_lines;
}

const QString SpeechControl::Content::pageAt(const int &l_index) const
{
    if (l_index < m_lines.count() && l_index != 0)
        return m_lines.at(l_index);

    return QString::null;
}

Content * Content::create(const QString &p_author, const QString &p_title, const QString &p_content)
{
    QUuid l_uuid = QUuid::createUuid();
    QDomDocument* l_dom = new QDomDocument("Content");
    QDomElement l_domElem = l_dom->createElement("Content");
    l_domElem.setAttribute("Uuid",l_uuid);
    l_dom->appendChild(l_domElem);

    QDomElement l_bilboElem = l_dom->createElement("Bilbography");
    l_bilboElem.setAttribute("Author",p_author);
    l_bilboElem.setAttribute("Title",p_title);
    l_domElem.appendChild(l_bilboElem);

    QDomElement l_textElem = l_dom->createElement("Content");
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
