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

#include "core.hpp"
#include "windows_main.hpp"
#include <QSettings>
#include <QApplication>

using namespace SpeechControl;
using SpeechControl::Core;

Core* Core::s_inst = 0;

// @todo Generate default settings.
Core::Core(int argc,char** argv) : QObject(new QApplication(argc,argv)){
    s_inst = this;
    m_settings = new QSettings(QSettings::UserScope,"Synthetic Intellect Institute","SpeechControl",this);
}

Core::~Core () {
    m_settings->sync();
}

void Core::start() {
    Windows::Main* m_mw = new Windows::Main;
    m_mw->show();
}

void Core::stop() { }

QVariant Core::getConfiguration(const QString &p_pth, QVariant p_vrt) const {
    return m_settings->value(p_pth,p_vrt);
}

void Core::setConfiguration(const QString &p_pth, const QVariant &p_vrt) {
    m_settings->setValue(p_pth,p_vrt);
}

Core * SpeechControl::Core::instance(){
    return s_inst;
}
