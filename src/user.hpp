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

#ifndef USER_HPP
#define USER_HPP

#include <QObject>
#include "training.hpp"

class QUrl;
class QFile;
class QUuid;
class QDomDocument;

namespace SpeechControl {
    class User;

    /// @todo Create a global listing
    class User : public QObject {
        Q_OBJECT
        Q_DISABLE_COPY(User)

    public:
        explicit User(QObject *parent = 0);
        virtual ~User();
        SessionList sessions() const;
        Session* createSession();
        void load(const QUuid&);
        const QUuid uuid() const;
        static const bool hasAny();
        static User* obtain(const QUuid&);
        static User* create();

    signals:
        void loaded();
        void saved();

    public slots:
        void save();       

    private:
        User(const QUuid&, QObject* parent = 0);
        static QUrl path(const QUuid&);
        QDomDocument* m_dom;
        SessionList m_sessLst;
    };
}

#endif // USER_HPP
