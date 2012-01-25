/**
 * This file is part of SpeechControl
 *
 * Copyright 2011 SpeechControl Developers <spchcntrl-devel@thesii.org>
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

    /**
     * @brief Represents a shorthand for denoating a list of @see Microphone.
     **/
    typedef QList<Microphone*> MicrophoneList;
    /**
     * @brief Represents a named mapping of Microphones.
     **/
    typedef QMap<QUuid, Microphone*> MicrophoneMap;

    /**
     * @brief Represents a handle of an audio input device on this computer.
     **/
    class Microphone : public QObject {
        Q_OBJECT
        Q_PROPERTY(const bool Active READ active)
        Q_PROPERTY(const QString Name READ friendlyName)
        Q_PROPERTY(const QUuid Uuid READ uuid)
        Q_PROPERTY(const QByteArray* Data READ data)
        Q_PROPERTY(const double Volume READ volume WRITE setVolume)
        Q_PROPERTY(const bool Muted READ isMuted WRITE mute)

    signals:
        void listening();
        void stoppedListening();

    public:
        Q_DISABLE_COPY(Microphone)
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
        QDataStream* stream() const;

        void setVolume(const double& );
        void mute(const bool& );

    public slots:
        void startRecording();
        void stopRecording();

    private slots:
      void release();
      void onPipelineBusmessage(const QGst::MessagePtr &);
      void onSinkAudioEos(const QGlib::Value& );
      void onSinkAudioNewbuffer(const QGlib::Value& );

    private:
        static void findMicrophones();
        static MicrophoneMap s_lst;
        static QGst::ElementPtr s_src;
        static QGst::PropertyProbePtr s_propProbe;
        static QGst::ChildProxyPtr s_chldPrxy;

        void obtain();
        QGst::BinPtr m_binAudio;
        QGst::PadPtr m_padAudio;
        QGst::ElementPtr m_srcAudio;
        QGst::ElementPtr m_srcVolume;
        QGst::ElementPtr m_sinkAudio;
        QGst::PipelinePtr m_pipeline;
        QGlib::Value m_device;
        QUuid m_uuid;
        QByteArray m_data;

    };
}

#endif // MICROPHONE_HPP


