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

#ifndef ACOUSTICMODEL_HPP
#define ACOUSTICMODEL_HPP

#include <QObject>
#include <QVariant>

namespace SpeechControl {
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
}


#endif // ACOUSTICMODEL_HPP
