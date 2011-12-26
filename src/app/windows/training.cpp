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

#include <QDebug>
#include <QMessageBox>

#include <training.hpp>

#include "session.hpp"
#include "ui_training.h"
#include "windows/training.hpp"

#define ICON_RECORD "media-record"
#define ICON_PAUSE "media-playback-pause"
#define ICON_STOP "media-playback-stop"
#define ICON_SAVE "document-save"
#define ICON_UNDO "edit-undo"
#define ICON_CLEAR "view-clear"

using namespace SpeechControl;
using SpeechControl::Windows::Training;

Training::Training(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Training)
{
    ui->setupUi(this);
    ui->pushButtonProgress->setIcon(QIcon::fromTheme(ICON_RECORD));
    ui->pushButtonSave->setIcon(QIcon::fromTheme(ICON_SAVE));
    ui->pushButtonReset->setIcon(QIcon::fromTheme(ICON_CLEAR));
    ui->pushButtonUndo->setIcon(QIcon::fromTheme(ICON_UNDO));
}

Training::~Training()
{
    delete ui;
}

void Training::startTraining(Session *p_session)
{
    if (!p_session->isCompleted()){
        Training* l_dialog = new Training;
        l_dialog->setSession(p_session);
        l_dialog->open();
    } else
        QMessageBox::information(0,"Session Completed", "Your session is completed.");
}

/// @todo Start training the session.
void Training::startCollecting()
{
    // Configure the button.
    ui->pushButtonProgress->setIcon(QIcon::fromTheme(ICON_PAUSE));
    ui->pushButtonProgress->setText(tr("Pause"));

    // Determine the last saved sentence in the session.

    // Begin an iteration of reading sentences until interrupted or completed.
    m_curSntct = m_session->firstIncompleteSentence();
    if (m_curSntct)
        navigateToPart(0);
}

void Training::stopCollecting()
{
    ui->pushButtonProgress->setIcon(QIcon::fromTheme(ICON_RECORD));
    ui->pushButtonProgress->setText(tr("Start"));
}

/// @todo Connect the changing of progress of the session to this window.
void Training::setSession(Session *p_session)
{
    m_session = p_session;
    connect(m_session,SIGNAL(progressChanged(double)),this,SLOT(updateProgress(double)));
}

Session* Training::session() const
{
    return m_session;
}

void SpeechControl::Windows::Training::on_pushButtonClose_clicked()
{
    reject();
}

void SpeechControl::Windows::Training::on_pushButtonProgress_toggled(const bool& checked)
{
    ui->labelText->setEnabled(!checked);
    if (!checked)
        stopCollecting();
    else
        startCollecting();
}

void SpeechControl::Windows::Training::updateProgress(const double &p_progress)
{
    ui->progressBar->setValue((int)(p_progress * 100));
}

void SpeechControl::Windows::Training::open()
{
    ui->pushButtonProgress->setChecked(true);
    QDialog::open();
}

void SpeechControl::Windows::Training::navigateToPart(const int &l_index)
{
    int l_range = 0;
    QStringList l_words = m_curSntct->words();
    QString l_text;

    if (l_words.count() > 6)
        l_range = l_words.count() / 4;
    else
        l_range = 2;

    const int l_rangeMin = (l_index == 0 || l_index == 1) ? 0 : l_range * (l_index - 1);
    const int l_rangeMax = l_range * (l_index + 1);
    qDebug() << l_rangeMin << l_rangeMax;

    for (int i = 0; i < l_words.count(); i++){
        // determine if the index of the word falls between
        // the minimum or maximum value of the range.
        const QString l_curWord = l_words.at(i);

        if (l_rangeMin == i)
            l_text += "<b>";

        l_text += l_curWord;

        if (i != l_words.count() - 1)
            l_text += " ";

        if (l_rangeMax == i)
            l_text += "</b>";
    }

    ui->labelText->setText(l_text);
}

void SpeechControl::Windows::Training::navigateNextPart()
{
}

void SpeechControl::Windows::Training::navigatePreviousPart()
{
}

void SpeechControl::Windows::Training::startNavigating()
{
}

void SpeechControl::Windows::Training::stopNavigating()
{
}
