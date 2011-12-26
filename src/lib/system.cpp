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

#include "system.hpp"
#include "microphone.hpp"

#include <QDir>
#include <QGst/Init>
#include <QApplication>

using SpeechControl::System;
using SpeechControl::Microphone;

System* SpeechControl::System::s_inst = 0;

System::System(int* argc, char** argv[]) :
    QObject(QApplication::instance())
{
    if (argc && argv)
        QGst::init(argc,argv);
    else
        QGst::init();

    Microphone::init();
    QDir l_dir;
    l_dir.mkpath(QDir::homePath() + "/.speechcontrol/corpus");
    l_dir.mkpath(QDir::homePath() + "/.speechcontrol/dictionaries");
}

void System::start() {
    if (!s_inst)
        s_inst = new System(0,0);
}

void System::start(int* argc, char** argv[])
{
    if (!s_inst)
        s_inst = new System(argc,argv);
}

void System::stop(){
    s_inst->deleteLater();
}
