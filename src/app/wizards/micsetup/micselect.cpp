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

using namespace SpeechControl;
using namespace SpeechControl::Wizards::Pages;
using SpeechControl::Wizards::Pages::MicrophoneSelection;

MicrophoneSelection::MicrophoneSelection(QWidget *parent) :
    QWizardPage(parent), ui(new Ui::MicrophoneSelection),
    m_mic(Microphone::defaultMicrophone())
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
    MicrophoneList l_allMics = Microphone::allMicrophones();
    if (l_allMics.empty()){
        /// @todo Add error saying no mics found.
    } else {
        Q_FOREACH(const Microphone* l_mic, l_allMics){
            ui->comboBoxMicrophones->addItem(l_mic->friendlyName(),l_mic->uuid().toString());
        }
    }
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

/// @todo Obtain the device via GStreamer.
/// @todo Set the device to be detected for volume detection here.
/// @todo Set this page's value to this field.
void SpeechControl::Wizards::Pages::MicrophoneSelection::on_comboBoxMicrophones_activated(int index)
{
    const QUuid l_uuid(ui->comboBoxMicrophones->itemData(index).toString());
    m_mic = Microphone::getMicrophone(l_uuid);
}
