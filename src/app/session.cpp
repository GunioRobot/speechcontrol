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

Content::Content(const QUuid& p_uuid) {

}

Content* Content::obtain(const QUuid &p_uuid)
{
}

Content::~Content()
{
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

uint Content::pages() const
{
}

void Content::load(const QUuid &)
{
}

uint Content::words() const
{
}

uint Content::length() const
{
}

uint Content::characters() const
{
}

QString Content::getPath(const QUuid &p_uuid)
{
    return QDir::homePath() + "/.speechcontrol/contents/" + p_uuid.toString();
}
