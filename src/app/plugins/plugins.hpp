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

#ifndef PLUGINS_HPP
#define PLUGINS_HPP

#include <QUrl>
#include <QUuid>
#include <QList>
#include <QObject>

class QPluginLoader;
class QSettings;

namespace SpeechControl {
    namespace Plugins {
        class Factory;
        class AbstractPlugin;

        typedef QList<AbstractPlugin*> PluginList;

        class AbstractPlugin : public QObject {
            Q_OBJECT
            Q_DISABLE_COPY(AbstractPlugin)

        signals:
            void started();
            void stopped();

        public:
            explicit AbstractPlugin(QObject* = 0);
            const double version() const;
            const QString name() const;
            const QUuid uuid() const;

        protected:
            virtual void initialize() = 0;
            virtual void deinitialize() = 0;

        private:
            const bool isSupported() const;
            QPluginLoader* m_ldr;
            QSettings* m_cfg;
            QSettings* m_sttgs;
        };
    }
}

#endif // PLUGINS_HPP
