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

#include <QMap>
#include <QList>
#include <QUuid>
#include <QObject>
#include <QGlib/Value>
#include <QGst/ChildProxy>
#include <QGst/Element>
#include <QGst/ElementFactory>
#include <QGst/PropertyProbe>

namespace SpeechControl {
    class Microphone;

    typedef QList<Microphone*> MicrophoneList;
    typedef QMap<QUuid, Microphone*> MicrophoneMap;

    class Microphone : public QObject {
        friend class Core;
        Q_OBJECT
        Q_PROPERTY(const bool Active READ active)
        Q_PROPERTY(const QString Name READ friendlyName)
        Q_PROPERTY(const QUuid Uuid READ uuid)
        Q_ENUMS(TestResults)

    public:
        enum TestResults {
            Failure = 0,
            CouldntOpen,
            NothingHeard,
            UnknownError
        };

        explicit Microphone(QGlib::Value = 0);
        static Microphone* getMicrophone(const QUuid& );
        static Microphone* defaultMicrophone();
        static MicrophoneList allMicrophones();
        const bool active() const;
        const QUuid uuid() const;
        const QString friendlyName() const;
        const TestResults test() const;

    private:
        static void init();
        static void findMicrophones();
        static MicrophoneMap s_lst;
        static QGst::ElementPtr s_src;
        static QGst::PropertyProbePtr s_propProbe;
        static QGst::ChildProxyPtr s_chldPrxy;
        QGlib::Value m_device;
        QUuid m_uuid;
    };
}

#endif // MICROPHONE_HPP
