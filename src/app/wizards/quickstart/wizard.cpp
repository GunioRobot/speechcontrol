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

#include "wizard.hpp"
#include "userinit.hpp"
#include "micpicker.hpp"
#include "wizards/intro.hpp"
#include "wizards/outro.hpp"

using namespace SpeechControl::Wizards;

QuickStart::QuickStart(QWidget *parent) :
    WizardBase(parent)
{
    this->setWindowTitle(tr("Quick Start :: SpeechControl"));
    setPage(QuickStart::IntroductionPage,
            new Pages::Introduction(tr("This wizard is designed to ease the process of configuring SpeechControl.")));
    setPage(QuickStart::UserCreationPage,
            new Pages::UserInitialization);
    setPage(QuickStart::MicrophoneCreationPage,
            new Pages::MicrophonePicker);
    /*setPage(QuickStart::BookDownloadPage,
            new Pages::BookDownload);*/
    setPage(QuickStart::ConclusionPage,
            new Pages::Conclusion(tr("You've successfully configured SpeechControl to your liking. "
                                     "If you need to re-configure SpeechControl, feel free to run this wizard again.")));
}

QuickStart::~QuickStart()
{
}
