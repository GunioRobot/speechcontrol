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
#include <QGst/Bin>
#include <QGst/Pad>
#include <QGst/Bus>
#include <QGst/Message>
#include <QGst/Element>
#include <QGst/Pipeline>
#include <QGst/ChildProxy>
#include <QGst/ElementFactory>
#include <QGst/PropertyProbe>
#include <QGst/StreamVolume>


namespace SpeechControl {
    class Microphone;

    typedef QList<Microphone*> MicrophoneList;
    typedef QMap<QUuid, Microphone*> MicrophoneMap;

    class Microphone : public QObject {
        Q_OBJECT
        Q_PROPERTY(const bool Active READ active)
        Q_PROPERTY(const QString Name READ friendlyName)
        Q_PROPERTY(const QUuid Uuid READ uuid)
        Q_PROPERTY(const QByteArray* Data READ data)
        Q_PROPERTY(const double Volume READ volume WRITE setVolume)
        Q_PROPERTY(const bool Muted READ isMuted WRITE mute)

    public:
        enum TestResult {
            Failure = 0x1,
            CouldntOpen = 0x2,
            NothingHeard = 0x4,
            UnknownError = 0x8
        };

        Q_DECLARE_FLAGS(TestResults, TestResult)

        explicit Microphone(QGlib::Value = 0);
        virtual ~Microphone();
        static Microphone* getMicrophone(const QUuid& );
        static Microphone* defaultMicrophone();
        static MicrophoneList allMicrophones();
        static void init();
        const bool active() const;
        const QByteArray* data() const;
        const QUuid uuid() const;
        const QString friendlyName() const;
        const double volume() const;
        const bool isMuted() const;
        const bool isValid() const;

        void setVolume(const double& );
        void mute(const bool& );

    public slots:
        void startRecording();
        void stopRecording();

    private:        
        static void findMicrophones();
        static MicrophoneMap s_lst;
        static QGst::ElementPtr s_src;
        static QGst::PropertyProbePtr s_propProbe;
        static QGst::ChildProxyPtr s_chldPrxy;

        void obtain();
        QGst::BinPtr m_binAudioSrc;
        QGst::PadPtr m_padAudio;
        QGst::ElementPtr m_srcAudio;
        QGst::ElementPtr m_sinkFile;
        QGst::ElementPtr m_muxWav;
        QGst::PipelinePtr m_pipeline;
        QGlib::Value m_device;
        QUuid m_uuid;
        QByteArray m_data;

    private slots:
        void release();
        void onBusMessage(const QGst::MessagePtr &);
    };
}

#endif // MICROPHONE_HPP
