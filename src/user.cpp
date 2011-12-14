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

#include "user.hpp"
#include <QtXml>
#include <QFile>
#include <QDir>
#include <QUuid>
#include <QUrl>

using namespace SpeechControl;

User::User(QObject *parent) : QObject(parent), m_dom(new QDomDocument){
}

User::User(const QUuid& p_uuid, QObject* parent) : QObject(parent), m_dom(new QDomDocument) {
    load(p_uuid);
}

QUrl User::path(const QUuid &p_uuid)
{
    return QDir::homePath() + "./speechcontrol/users/" + p_uuid.toString() + ".usr";
}

const QUuid User::uuid() const {
    return QUuid(m_dom->documentElement().attribute("uuid"));
}

void User::save(){
    QFile* l_file = new QFile(path(uuid()).toLocalFile());
    QTextStream l_strm(l_file);
    m_dom->save(l_strm,4);
}

void User::load(const QUuid &p_uuid) {
    const QUrl l_url = path(p_uuid);
    QFile* l_file = new QFile(path(p_uuid).toLocalFile());
    m_dom->setContent(l_file);

    QDomElement l_domElem = m_dom->documentElement().toElement();
    QDomElement l_sessElem = l_domElem.namedItem("Sessions").toElement();
    QDomNodeList l_sessElemLst = l_sessElem.childNodes();

    for (int i = 0; i < l_sessElemLst.count(); ++i){
        const QDomElement l_elem = l_sessElemLst.at(i).toElement();
        if (l_elem.isNull()) continue;

        const QUuid l_sessUuid(l_elem.attribute("uuid"));
        m_sessLst << Session::obtain(l_sessUuid);
    }
}

/// @todo This does a bit of I/O; might need to cache this value into the app's QSettings and update it on start-up.
const bool User::hasAny()
{
    QDir l_dir = QDir::home();
    l_dir.cd(".speechcontrol/users");
    return l_dir.entryList(QStringList() << "*.usr",QDir::Files).empty();
}

SessionList User::sessions() const
{
    return m_sessLst;
}

Session * User::createSession()
{
    return Session::create(this);
}

/// @todo Should the encoding be UTF-16? To hold other languages?
User* User::create()
{
    const QUuid l_uuid = QUuid::createUuid();
    const QUrl l_url = path(l_uuid);
    QFile::copy(path(l_uuid).toString().replace(l_uuid.toString(),"template"),l_url.toString());
    QFile* l_file = new QFile(l_url.toString());
    QString l_data = l_file->readAll();
    l_data = l_data.replace("%{template}",l_uuid.toString());
    l_file->write(l_data.toUtf8());
    return new User(l_uuid);
}

User* User::obtain(const QUuid &p_uuid)
{
    if (!QFile::exists(path(p_uuid).toLocalFile()))
        return 0;

    return new User(p_uuid);
}

/// @todo What to do?
User::~User() {
}
