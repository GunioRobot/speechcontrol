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
#include <QFile>
#include <QUuid>
#include <QUrl>

using namespace SpeechControl;

User::User(QObject *parent) : QObject(parent){
}

User* User::loadFromString(QFile &p_file) {
}

User* User::loadFromUrl(QUrl &p_url) {
}

User* User::loadFromUuid(QUuid &p_uuid) {
}

void User::save(){
}

void User::load(QUrl &p_url) {
}

void User::load(QUuid &p_uuid) {
}

const bool User::hasAny()
{
    return true;
}

User::~User() {

}
