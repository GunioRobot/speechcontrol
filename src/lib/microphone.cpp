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

#include "microphone.hpp"

#include <QUuid>
#include <QDebug>
#include <QVariant>
#include <QGlib/Error>
#include <QGlib/Connect>

using namespace SpeechControl;
using SpeechControl::Microphone;

MicrophoneMap Microphone::s_lst;
QGst::ElementPtr Microphone::s_src;
QGst::PropertyProbePtr Microphone::s_propProbe;
QGst::ChildProxyPtr Microphone::s_chldPrxy;

Microphone::Microphone(QGlib::Value device ) :
    m_device(device), m_uuid(QUuid::createUuid())
{
   // Get the microphone.
   obtain();
   connect(this,SIGNAL(destroyed()),this,SLOT(release()));
}

/// @todo Have the system detect when new microphones are added + removed to the system.
void Microphone::init() {
    //QGlib::connect(propertyProbe, "probe-needed", this, &Recorder::probeForDevices, QGlib::PassSender);
    findMicrophones();
}

void Microphone::findMicrophones() {
    const QString l_audioSrc("autoaudiosrc");
    s_src = QGst::ElementFactory::make(l_audioSrc);
    if (s_src) {
        s_src->setState(QGst::StateReady);
        s_chldPrxy = s_src.dynamicCast<QGst::ChildProxy>();

        if (s_chldPrxy)
            s_propProbe = s_chldPrxy->childByIndex(0).dynamicCast<QGst::PropertyProbe>();

        if (s_propProbe){
            QList<QGlib::Value> devices = s_propProbe->probeAndGetValues("device");
            s_src->setState(QGst::StateNull);

            if (s_propProbe && s_propProbe->propertySupportsProbe("device")) {
                Q_FOREACH(QGlib::Value l_device, devices) {
                    qDebug() << tr("Found microphone") << l_device;
                    s_propProbe->setProperty("device", l_device);
                    new Microphone(l_device);
                }
            }
        }
    } else {
        qDebug() << tr("Failed to create element \"%1\". Make sure you have "
                       "gstreamer-plugins-good installed").arg(l_audioSrc);
    }
}

Microphone * Microphone::getMicrophone(const QUuid &l_uuid)
{
    if (s_lst.contains(l_uuid))
        return s_lst.value(l_uuid);

    return 0;
}

/// @todo How do you determine which microphone is the default one?
Microphone* Microphone::defaultMicrophone()
{
    if (!s_lst.empty())
        return s_lst.values().first();

    return 0;
}

const bool Microphone::active() const
{
    return !(!this->m_pipeline);
}

const QString Microphone::friendlyName() const
{
    return m_device.toString();
}


const QUuid Microphone::uuid() const
{
    return m_uuid;
}

MicrophoneList Microphone::allMicrophones()
{    
    return s_lst.values();
}

void SpeechControl::Microphone::startRecording()
{
    // Wipe any data already used for recording; their loss.
    m_data.clear();

    // Obtain tools for recording like the encoder and the source.
    m_sinkFile = QGst::ElementFactory::make("filesink");

    if (!m_sinkFile) {
       qCritical() << tr("One or more elements could not be created. "
                             "Verify that you have all the necessary element plugins installed.");
       return;
    }

    m_sinkFile->setProperty("location", "file.wav");

    // Build the pipeline.
    m_pipeline = QGst::Pipeline::create();
    m_pipeline->add(m_binAudioSrc, m_muxWav, m_sinkFile);

    //link elements
    QGst::PadPtr audioPad = m_muxWav->getRequestPad("sink_%d");
    m_binAudioSrc->getStaticPad("src")->link(audioPad);

    m_muxWav->link(m_muxWav);
    m_muxWav->link(m_sinkFile);

    // Connect the bus to this Microphone to detect changes in the pipeline.
    m_pipeline->bus()->addSignalWatch();
    QGlib::connect(m_pipeline->bus(), "message", this, &Microphone::onBusMessage);

    // Get the party started :)
    m_pipeline->setState(QGst::StatePlaying);
}

void SpeechControl::Microphone::stopRecording()
{
    // Stop recording.
}

const QByteArray* Microphone::data() const {
    if (m_data.size() == 0)
        return 0;
    else
        return &m_data;
}

const double SpeechControl::Microphone::volume() const
{
    return 0.0;
}

const bool SpeechControl::Microphone::isMuted() const
{
    return false;
}

void SpeechControl::Microphone::setVolume(const double &p_volume)
{
}

void SpeechControl::Microphone::mute(const bool &p_mute)
{
}

void SpeechControl::Microphone::obtain()
{
   try {
       m_binAudioSrc = QGst::Bin::fromDescription("autoaudiosrc name=\"audiosrc\" ! audioconvert ! "
                                             "audioresample ! audiorate ! wavenc name=\"wavenc\" ! queue");
   } catch (const QGlib::Error & error) {
       qCritical() << "Failed to create audio source bin:" << error;
       m_binAudioSrc.clear();
       return;
   }

   s_lst.insert(m_uuid,const_cast<Microphone*>(this));

   m_srcAudio = m_binAudioSrc->getElementByName("audiosrc");
   m_muxWav = m_binAudioSrc->getElementByName("wavenc");

   //autoaudiosrc creates the actual source in the READY state
   m_srcAudio->setState(QGst::StateReady);

   QGst::ChildProxyPtr childProxy = m_srcAudio.dynamicCast<QGst::ChildProxy>();
   if (childProxy && childProxy->childrenCount() > 0) {
       //the actual source is the first child
       QGst::ObjectPtr realSrc = childProxy->childByIndex(0);
       realSrc->setProperty("device", m_device.toString());
   }

   m_srcAudio->setState(QGst::StateNull);
}

void SpeechControl::Microphone::release()
{
    // Turn everything off.
    m_binAudioSrc->setState(QGst::StateNull);
    m_srcAudio->setState(QGst::StateNull);
    m_sinkFile->setState(QGst::StateNull);
    m_pipeline->setState(QGst::StateNull);

    // Free memory.
    m_binAudioSrc.clear();
    m_srcAudio.clear();
    m_sinkFile.clear();
    m_pipeline.clear();
}

const bool Microphone::isValid() const {
    return !m_binAudioSrc.isNull();
}

void Microphone::onBusMessage(const QGst::MessagePtr & message)
{
    switch (message->type()) {
        case QGst::MessageEos:
            //got end-of-stream - stop the pipeline
            //stop();
            break;
        case QGst::MessageError:
            //check if the pipeline exists before destroying it,
            //as we might get multiple error messages
            if (m_pipeline) {
                //stop();
            }
            qCritical() << tr("Pipeline Error")
                        << message.staticCast<QGst::ErrorMessage>()->error().message();
            break;

        default:
            break;
    }
}

Microphone::~Microphone() {
    // Release the microphone.
    release();
}