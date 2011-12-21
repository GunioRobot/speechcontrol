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

#include "sphinx.hpp"
#include "microphone.hpp"
#include <QFile>
#include <pocketsphinx.h>

using SpeechControl::Sphinx;

Sphinx::Sphinx() {
}

Sphinx::~Sphinx() {

}

void Sphinx::recognizeFromFile(const QFile *p_file)
{
}

void Sphinx::recognizeFromMicrophone(const Microphone *p_mic)
{
}

void SpeechControl::Sphinx::startRecognizing(const SpeechControl::Microphone *)
{
}

void SpeechControl::Sphinx::stopRecording()
{
}

SpeechControl::AcousticModel::~AcousticModel()
{
}

void SpeechControl::AcousticModel::setParamter(const QString &, const QVariant &)
{
}

void SpeechControl::AcousticModel::setParamters(const QVariantMap &)
{
}

void SpeechControl::AcousticModel::mergeParameters(const QVariantMap &)
{
}

QVariant SpeechControl::AcousticModel::parameter(const QString &) const
{
}

QVariantMap SpeechControl::AcousticModel::paramters() const
{
}

quint16 SpeechControl::AcousticModel::sampleRate() const
{
}

void SpeechControl::AcousticModel::setSampleRate(const quint16 &)
{
}
