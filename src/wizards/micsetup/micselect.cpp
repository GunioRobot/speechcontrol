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

#include "microphone.hpp"
#include "wizards/micsetup/micselect.hpp"
#include "ui_micselect.h"

using namespace SpeechControl::Wizards::Pages;
using SpeechControl::Wizards::Pages::MicrophoneSelection;

MicrophoneSelection::MicrophoneSelection(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MicrophoneSelection)
{
    ui->setupUi(this);
}

MicrophoneSelection::~MicrophoneSelection()
{
    delete ui;
}

/// @todo Fill the combo box with all of the mics.
void SpeechControl::Wizards::Pages::MicrophoneSelection::initializePage()
{
    cleanupPage();
}

bool SpeechControl::Wizards::Pages::MicrophoneSelection::validatePage()
{
    return ui->progressBarFeedback->isEnabled();
}

void SpeechControl::Wizards::Pages::MicrophoneSelection::cleanupPage()
{
    ui->comboBoxMicrophones->clear();
    ui->progressBarFeedback->setValue(0);
    ui->progressBarFeedback->setFormat("inactive");
}

bool SpeechControl::Wizards::Pages::MicrophoneSelection::isComplete()
{
    return m_complete;
}
