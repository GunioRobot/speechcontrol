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

#ifndef MICROPHONE_HPP
#define MICROPHONE_HPP

#include <QUuid>
#include <QObject>
#include <QObject>

namespace SpeechControl {
    class Microphone;

    typedef QList<Microphone*> MicrophoneList;

    class Microphone : public QObject {
        Q_OBJECT
        Q_ENUMS(TestResults)

    public:
        enum TestResults {
            Failure = 0,
            CouldntOpen,
            NothingHeard,
            UnknownError
        };

        explicit Microphone(QObject *parent = 0);
        static Microphone* getMicrophone(const QUuid& );
        static Microphone* primaryMicrophone();
        static MicrophoneList allMicrophones();
        const bool active() const;
        const QString friendlyName() const;
        const TestResults test();

    public slots:

    };
}

#endif // MICROPHONE_HPP
