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

#ifndef SPHINX_HPP
#define SPHINX_HPP

#include <QObject>
#include <QVariant>
#include <QVariantMap>

class QFile;

namespace SpeechControl {
    class Sphinx;
    class Trainer;
    class Microphone;
    class AcousticModel;

    typedef QList<AcousticModel*> AcousticModelList;

    class AcousticModel : public QObject {
        Q_OBJECT
        Q_PROPERTY(const QVariantMap Parameters READ parameters WRITE setParameters)
        Q_PROPERTY(const quint16 SampleRate READ sampleRate WRITE setSampleRate)

    public:
        Q_DISABLE_COPY(AcousticModel)
        virtual ~AcousticModel();
        void setParameter(const QString&, const QVariant& );
        void setParameters(const QVariantMap&);
        void mergeParameters(const QVariantMap&);
        QVariant parameter(const QString&) const;
        const QVariantMap parameters() const;
        const quint16 sampleRate() const;
        void setSampleRate(const quint16&);
    };

    class Sphinx : public QObject {
        Q_OBJECT
        Q_DISABLE_COPY(Sphinx)

    public:
        explicit Sphinx(const AcousticModel* = 0);
        virtual ~Sphinx();
        void setAcousticModel(const AcousticModel* );
        AcousticModel* acousticModel() const;
        static void startRecognizing(Microphone* = 0);
        static void stopRecording(Microphone* = 0);
        const bool isListening() const;

    public slots:
        const QString recognizeFromFile(const QFile* );
        const QString recognizeFromMicrophone(const Microphone* = 0);


    private:
        Microphone* m_mic;
        AcousticModel* m_mdl;
    };
}

#endif // SPHINX_HPP
