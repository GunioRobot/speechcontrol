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

#include <QFile>
#include <QDebug>
#include <QSettings>
#include <QApplication>
#include <QMessageBox>
#include <QtGStreamer/QGst/Init>

#include "core.hpp"
#include "microphone.hpp"
#include "windows/main.hpp"

using namespace SpeechControl;
using SpeechControl::Core;

Core* Core::s_inst = 0;

/// @todo Generate default settings.
/// @todo Invoke first-run wizard.
/// @todo Pass command-line arguments to QGst *after* QApplication gets them.
Core::Core(int argc,char** argv) : QObject(new QApplication(argc,argv)){
    s_inst = this;

    // start application.    
    QApplication* l_app = qobject_cast<QApplication*>(QApplication::instance());
    l_app->setApplicationName("SpeechControl");
    l_app->setOrganizationDomain("thesii.org");
    l_app->setOrganizationName("Synthetic Intellect Institute");
    l_app->setApplicationVersion("0.1b");


    // build settings
    m_settings = new QSettings(QSettings::UserScope,"Synthetic Intellect Institute","SpeechControl",this);
    QFile* l_settings = new QFile(m_settings->fileName());
    if (!l_settings->exists())
        l_settings->write("");
    qDebug() << l_settings->fileName() << l_settings->exists();
    l_settings->close();

    QGst::init();
    Microphone::init();

    // check for microphones
    if (Microphone::allMicrophones().empty()){
        QMessageBox::information(0,tr("No Microphones Found"),tr("No microphones were found on your system. Please ensure that you have one installed and detectable by ") +
                                 tr("the audio system and make sure that <b>gstreamer-plugins-good</b> is installed on your system."));
    }
}

Core::~Core () {
    m_settings->sync();
}

void Core::start() {
    Windows::Main* m_mw = new Windows::Main;
    m_mw->show();
}

void Core::stop() { }

QVariant Core::getConfig(const QString &p_pth, QVariant p_vrt) const {
    return m_settings->value(p_pth,p_vrt);
}

void Core::setConfig(const QString &p_pth, const QVariant &p_vrt) {
    m_settings->setValue(p_pth,p_vrt);
}

Core * SpeechControl::Core::instance(){
    return s_inst;
}
