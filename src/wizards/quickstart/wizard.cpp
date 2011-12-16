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

#include "wizards/quickstart/wizard.hpp"
#include "wizards/quickstart/micsetup.hpp"
#include "wizards/intro.hpp"
#include "ui_quickstart.h"

using namespace SpeechControl::Wizards;

QuickStart::QuickStart(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::QuickStartWizard)
{
    ui->setupUi(this);
    setPage(QuickStart::IntroductionPage,
            new Pages::Introduction(tr("This wizard will guide you through the process ")+
                                     tr("of configuring SpeechControl for your needs.")));
    setPage(QuickStart::ConfigureMicrophonePage,
            new Wizards::Pages::MicrophoneConfiguration);
}

QuickStart::~QuickStart()
{
    delete ui;
}
