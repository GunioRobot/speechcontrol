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
#include <QTextStream>

using SpeechControl::Corpus;
using SpeechControl::Content;
using SpeechControl::Session;
using SpeechControl::SessionMap;
using SpeechControl::SessionList;
using SpeechControl::ContentList;

QMap<QUuid, QDomElement*> Session::s_elems;
QDomDocument* Session::s_dom = 0;

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
    QFile l_file(QDir::homePath() + "/.speechcontrol/sessions.xml");
    l_file.open(QIODevice::ReadOnly);
    s_dom = new QDomDocument;
    s_dom->setContent(&l_file);

    const QDomElement l_domElem = s_dom->documentElement();
    const QDomNodeList l_domList = l_domElem.elementsByTagName("Session");

    for (int i = 0; i < l_domList.count(); i++){
        QDomElement l_node = l_domList.at(i).toElement();
        s_elems.insert(QUuid(l_node.attribute("uuid")),&l_node);
    }
}

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
    return new Session(p_uuid);
}

/// @todo Create a new @see Corpus with this @see Session.
Session* Session::create(const Content* p_content)
{
}

Content::Content(const QUuid& p_uuid) {
    load(p_uuid);
}

Content* Content::obtain(const QUuid &p_uuid)
{
}

void Content::load(const QUuid &p_uuid)
{
    QFile* l_file = new QFile(getPath(p_uuid));
    m_dom = new QDomDocument("Content");
    m_dom->setContent(l_file);

    m_lines = m_dom->documentElement().elementsByTagName("Content").at(0).nodeValue().split("\n");
    m_uuid = p_uuid;
}

const uint Content::pageCount() const
{
    return m_dom->documentElement().elementsByTagName("Count").at(0).toElement().attribute("pages").toUInt();
}

const uint Content::words() const
{
    return m_dom->documentElement().elementsByTagName("Count").at(0).toElement().attribute("words").toUInt();
}

const uint Content::length() const
{
    return m_lines.join("\n").length();
}

const uint Content::characters() const
{
    return m_dom->documentElement().elementsByTagName("Count").at(0).toElement().attribute("chars").toUInt();
}

QString Content::getPath(const QUuid &p_uuid)
{
    return QDir::homePath() + "/.speechcontrol/contents/" + p_uuid.toString();
}

const QString Content::title() const
{
    return m_dom->documentElement().elementsByTagName("Biblography").at(0).toElement().attribute("Title");
}

const QString Content::author() const
{
    return m_dom->documentElement().elementsByTagName("Biblography").at(0).toElement().attribute("Author");
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
    return ContentList();
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
