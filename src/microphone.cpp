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

#include <QUuid>
#include <QMessageBox>
#include "microphone.hpp"

using namespace SpeechControl;
using SpeechControl::Microphone;

MicrophoneMap Microphone::s_lst;
QGst::ElementPtr Microphone::s_src;
QGst::PropertyProbePtr Microphone::s_propProbe;
QGst::ChildProxyPtr Microphone::s_chldPrxy;

Microphone::Microphone(QGlib::Value device ) :
    m_device(device), m_uuid(QUuid::createUuid())
{
    s_lst.insert(m_uuid,const_cast<Microphone*>(this));
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
                Q_FOREACH(QGlib::Value device, devices) {
                    s_propProbe->setProperty("device", device);
                    new Microphone(device);
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

}

const bool Microphone::active() const
{
}

const QString Microphone::friendlyName() const
{
    return m_device.toString();
}

const Microphone::TestResults Microphone::test() const
{
}

const QUuid Microphone::uuid() const
{
    return m_uuid;
}

MicrophoneList Microphone::allMicrophones()
{    
    return s_lst.values();
}
