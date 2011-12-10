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

#ifndef TRAINING_HPP
#define TRAINING_HPP

#include <QList>
#include <QObject>

class QDomDocument;
class QDomElement;
class QUuid;
class QFile;

namespace SpeechControl {
    class User;
    class Phrase;
    class Session;

    typedef QList<Phrase*> PhraseList;
    typedef QList<Session*> SessionList;

    class Phrase : public QObject {
        Q_OBJECT

    public:
        virtual ~Phrase();
        Session* parentSession();
        QUuid uuid();
        QFile* audio();
        static Phrase* create(const Session*, const QDomElement* );

    private:
        explicit Phrase(QObject* parent = 0);
        const QDomElement* m_elem;
        const Session* m_sess;
    };

    class Session : public QObject {
        Q_OBJECT
        Q_DISABLE_COPY(Session)
    public:
        virtual ~Session();
        PhraseList phrases() const;
        Phrase* phrase(const QUuid&) const;
        void addPhrase(const Phrase&);
        Session& operator<<(const Phrase&);
        Session& operator<<(const PhraseList&);
        static Session* create(const User*);

    private:
        explicit Session(QObject *parent = 0);
        User* m_usr;
        QDomDocument* m_dom;
        PhraseList m_phrsLst;
    };
}

#endif // TRAINING_HPP
