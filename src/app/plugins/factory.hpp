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

#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <QObject>
#include "plugins.hpp"

namespace SpeechControl {
    namespace Plugins {
        class Factory;

        class Factory : public QObject
        {
            Q_OBJECT

        signals:
            void started();
            void stopped();
            void pluginLoaded();
            void pluginLoaded(const QString& );
            void pluginUnloaded();
            void pluginUnloaded(const QString& );

        public:
            explicit Factory(QObject* parent = 0);
            Factory();
            virtual ~Factory();

            static const bool loadPlugin(const QUuid& );
            static const bool unloadPlugin(const QUuid& );
            static PluginList loadedPlugins();
            static PluginList availablePlugins();

        private:
            static Factory* s_inst;
        };
    }
}

#endif // FACTORY_HPP
