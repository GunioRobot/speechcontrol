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

#include "wizards/quickstart/micsetup.hpp"
#include "wizards/micsetup/wizard.hpp"
#include "ui_micasksetup.h"
#include <QDebug>

using namespace SpeechControl::Wizards;
using namespace SpeechControl::Wizards::Pages;

MicrophoneConfiguration::MicrophoneConfiguration(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::MicrophoneConfiguration), m_config(false)
{
    ui->setupUi(this);
}

MicrophoneConfiguration::~MicrophoneConfiguration()
{
    delete ui;
}

void SpeechControl::Wizards::Pages::MicrophoneConfiguration::on_pushButton_clicked()
{
    MicrophoneSetup* l_wiz = new MicrophoneSetup(this);
    m_config = l_wiz->exec();
    emit completeChanged();
    qDebug() << l_wiz->result() << m_config;
}

bool MicrophoneConfiguration::isComplete()
{
    return ui->pushButton->isEnabled();
}

void MicrophoneConfiguration::cleanupPage() {
    ui->pushButton->setEnabled(true);
}

void MicrophoneConfiguration::initializePage()
{
    m_config = false;
}

bool SpeechControl::Wizards::Pages::MicrophoneConfiguration::validatePage()
{
    return isComplete();
}
