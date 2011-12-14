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

Session::Session(const QUuid& p_uuid) : QObject(), m_dom(new QDomDocument){
    load(p_uuid);
}

PhraseList Session::phrases() const
{
    return m_phrsLst;
}

Phrase * Session::phrase(const QUuid &l_key) const
{
    if (m_phrsLst.contains(l_key))
        return m_phrsLst.value(l_key);
    else
        return 0;
}

/// @todo Add the Phrase to the XML document and then to the list.
void Session::addPhrase(Phrase *l_phrs)
{
    m_phrsLst.insert(l_phrs->uuid(),l_phrs);
}

Session & Session::operator <<(Phrase *l_phrs)
{
    this->addPhrase(l_phrs);
}

/// @todo Just invoke the above method.
Session & Session::operator <<(PhraseList &l_lst)
{
    foreach (Phrase* l_phrs, l_lst)
        this->addPhrase(l_phrs);
}

Session * Session::create(const User *)
{
    QUuid l_uuid = QUuid::createUuid();
    QDir l_dir;
    if (!l_dir.mkpath(getPath(l_uuid).toLocalFile())){
        qWarning() << "Can't make session" << l_uuid;
        return 0;
    }
}

const bool Session::exists(const QUuid& l_uuid){
    return QFile::exists(getPath(l_uuid).toLocalFile());
}

QUrl Session::getPath(const QUuid &l_uuid)
{
    return QDir::homePath() + "./speechcontrol/sessions/" + l_uuid.toString();
}

QUrl Session::audioPath()
{
    return getPath(this->uuid()).toLocalFile() + "/audio";
}

const QUuid Session::uuid() const
{
    return m_dom->documentElement().attribute("uuid");
}

Session* Session::obtain(const QUuid &l_uuid)
{
    if (!QFile::exists(getPath(l_uuid).toLocalFile()))
        return 0;

    return new Session(l_uuid);
}

void Session::load(const QUuid &p_uuid)
{
    const QUrl l_path = getPath(p_uuid);
    QFile* l_file = new QFile(l_path.toLocalFile());

    if (!m_dom)
        m_dom = new QDomDocument;

    m_dom->setContent(l_file);
}

void Session::save()
{
}

/// @todo What to clean-up?
Session::~Session()
{

}

Phrase::Phrase(Session* p_sess, QDomElement *p_elem) : m_elem(p_elem), m_sess(p_sess)
{
}

Session * Phrase::parentSession() const
{
    return m_sess;
}

QUuid Phrase::uuid() const
{
    return QUuid(m_elem->attribute("uuid"));
}

QFile * Phrase::audio() const
{
    const QString l_pth = m_sess->audioPath().toLocalFile() + "/" + m_elem->attribute("filename");
    return new QFile(l_pth);
}

/// @todo This method should add itself to the parent session.
Phrase * Phrase::create(Session *l_sess)
{
    QDomElement l_elem = l_sess->m_dom->createElement("Phrase"),
            l_rootElem = l_sess->m_dom->documentElement().namedItem("Phrases").toElement();

    l_rootElem.appendChild(l_elem);
    l_elem.setAttribute("filename",QString::null);
    l_elem.setAttribute("uuid",QUuid::createUuid());
    Phrase* l_phrs = new Phrase(l_sess,&l_elem);
    l_sess->addPhrase(l_phrs);

    return l_phrs;
}

Phrase::~Phrase()
{

}
