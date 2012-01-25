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

#ifndef SPHINX_HPP
#define SPHINX_HPP

#include <QObject>
#include <QVariant>
#include <QVariantMap>
#include <pocketsphinx.h>
#include "microphone.hpp"

class QFile;

namespace SpeechControl {
    class Sphinx;
    class Trainer;
    class Microphone;
    class AcousticModel;

    class Sphinx : public QObject {
        Q_OBJECT

    signals:
        void textRecognized(const QString&);

    public:
        Q_DISABLE_COPY(Sphinx)
        explicit Sphinx(const AcousticModel* = 0);
        virtual ~Sphinx();
        void setAcousticModel(const AcousticModel* );
        AcousticModel* acousticModel() const;
        static void startRecognizing(Microphone* = Microphone::defaultMicrophone());
        static void stopRecognizing(Microphone* = Microphone::defaultMicrophone());
        const bool isListening() const;
        const QString text() const;

    public slots:
        void recognizeFromFile(const QFile* );
        void recognizeFromMicrophone(const Microphone* = Microphone::defaultMicrophone());

    private:
        void initialize();
        QString m_hypothesis;
        Microphone* m_mic;
        AcousticModel* m_mdl;
        ps_decoder_t *m_decoder;
        cmd_ln_t *m_config;
    };
}

#endif // SPHINX_HPP
