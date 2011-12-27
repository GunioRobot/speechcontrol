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


#ifndef CORE_HPP
#define CORE_HPP

#include <QObject>
#include <QVariant>

class QSettings;

namespace SpeechControl {
namespace Windows {
struct Main;
}
struct Core;

/// @todo Allow versioning of the configuration.
/// @todo Remove reference to Windows::Main.
class Core : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Core)
    friend class Windows::Main;

signals:
    void started();
    void stopped();

public:
    Core(int,char**);
    virtual ~Core();
    QVariant getConfig(const QString&, QVariant = QVariant()) const;
    void setConfig(const QString&, const QVariant&);
    static Core* instance();

public slots:
    void start();
    void stop();

private:
    QSettings* m_settings;
    static Core* s_inst;

};
}

#endif // CORE_HPP
