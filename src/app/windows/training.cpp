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

#include <training.hpp>

#include "session.hpp"
#include "ui_training.h"
#include "windows/training.hpp"

#define ICON_RECORD "media-record"
#define ICON_PAUSE "media-playback-pause"
#define ICON_PLAY "media-playback-play"
#define ICON_STOP "media-playback-stop"
#define ICON_SAVE "document-save"
#define ICON_UNDO "edit-undo"
#define ICON_CLEAR "view-clear"

using SpeechControl::Session;
using SpeechControl::Corpus;
using SpeechControl::Windows::Training;

Training::Training(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Training)
{
    ui->setupUi(this);
    ui->pushButtonProgress->setIcon(QIcon::fromTheme(ICON_PLAY));
    ui->pushButtonSave->setIcon(QIcon::fromTheme(ICON_SAVE));
    ui->pushButtonReset->setIcon(QIcon::fromTheme(ICON_CLEAR));
    ui->pushButtonUndo->setIcon(QIcon::fromTheme(ICON_UNDO));
}

Training::~Training()
{
    delete ui;
}

void Training::on_pushButtonProgress_toggled(bool checked)
{
    if (checked){

    }
    else {

    }
}

void Training::startTraining(Session *p_session)
{
    Training* l_dialog = new Training(QApplication::topLevelWidgets().front());
    l_dialog->setSession(p_session);
    l_dialog->exec();
}

void Training::startCollecting()
{
}

void Training::stopCollecting()
{
}

void Training::setSession(Session *p_session)
{
    m_session = p_session;
}

Session* Training::session() const
{
    return m_session;
}
