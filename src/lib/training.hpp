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
#include <QMap>
#include <QObject>

class QDomDocument;
class QDomElement;
class QUuid;
class QUrl;
class QFile;

namespace SpeechControl {
    class Phrase;
    class Session;

    typedef QMap<QUuid, Phrase*> PhraseList;
    typedef QList<Session*> SessionList;

    class Phrase : public QObject {
        Q_OBJECT
        Q_DISABLE_COPY(Phrase)
        friend class Session;

    public:
        virtual ~Phrase();
        Session* parentSession() const;
        QUuid uuid() const;
        QFile* audio() const;
        static Phrase* create(Session*);

    private:
        explicit Phrase(Session*, QDomElement* );
        QDomElement* m_elem;
        Session* m_sess;
    };

    class Session : public QObject {
        Q_OBJECT
        Q_DISABLE_COPY(Session)
        friend class Phrase;

    public:
        virtual ~Session();
        PhraseList phrases() const;
        Phrase* phrase(const QUuid&) const;
        void load(const QUuid&);
        void save();
        void addPhrase(Phrase*);
        const QUuid uuid() const;
        Session& operator<<(Phrase*);
        Session& operator<<(PhraseList&);
        static Session* create();
        static Session* obtain(const QUuid&);
        static SessionList allSessions();
        static const bool exists(const QUuid&);

    private:
        Session(const QUuid& );
        static QUrl getPath(const QUuid&);
        QUrl audioPath();
        QDomDocument* m_dom;
        PhraseList m_phrsLst;
    };
}

#endif // TRAINING_HPP
