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
    Q_DISABLE_COPY(AcousticModel)

public:
    virtual ~AcousticModel();
    void setParamter(const QString&, const QVariant& );
    void setParamters(const QVariantMap&);
    void mergeParameters(const QVariantMap&);
    QVariant parameter(const QString&) const;
    QVariantMap paramters() const;
    quint16 sampleRate() const;
    void setSampleRate(const quint16&);
};

class Sphinx : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Sphinx)

public:
    explicit Sphinx();
    virtual ~Sphinx();
    void setAcousticModel(const AcousticModel* );
    AcousticModel* acousticModel() const;

public slots:
    void recognizeFromFile(const QFile* );
    void recognizeFromMicrophone(const Microphone* );
    void startRecognizing(const Microphone* );
    void stopRecording();

};
}

#endif // SPHINX_HPP
