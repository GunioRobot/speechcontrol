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
#include <QErrorMessage>
#include <QMessageBox>

#include <system.hpp>
#include <training.hpp>
#include <microphone.hpp>

#include "core.hpp"
#include "session.hpp"
#include "windows/main.hpp"
#include "wizards/quickstart/wizard.hpp"

using namespace SpeechControl;
using namespace SpeechControl::Wizards;

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
    l_app->setApplicationVersion("0.0b");

    System::start(&argc,&argv);
    Session::init();

    // build settings
    m_settings = new QSettings(QSettings::UserScope,"Synthetic Intellect Institute","SpeechControl",this);

    // check for microphones
    if (Microphone::allMicrophones().empty()){
        QErrorMessage* l_msg = new QErrorMessage;
        l_msg->setWindowTitle("No Microphones Found");
        l_msg->showMessage(tr("No microphones were found on your system. Please ensure that you have one installed and detectable by ") +
                                 tr("the audio system and make sure that <b>gstreamer-plugins-good</b> is installed on your system."),
                                   "NoMicrophonesFoundOnStart");
    }
}

Core::~Core () {
    m_settings->sync();
}

void Core::start() {
    Windows::Main* l_mw = new Windows::Main;

    if (!s_inst->m_settings->contains("User/Name")){
        if (QMessageBox::question(l_mw,tr("First Run"),
                                 tr("This seems to be the first time you've run SpeechControl on this system. "
                                    "A wizard allowing you to start SpeechControl will appear."),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes){
            QuickStart* l_win = new QuickStart(l_mw);
            l_win->exec();
        }
    }

    l_mw->show();
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
