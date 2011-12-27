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
    m_ui(new Ui::Training)
{
    m_ui->setupUi(this);
    m_ui->pushButtonProgress->setIcon(QIcon::fromTheme(ICON_RECORD));
    m_ui->pushButtonSave->setIcon(QIcon::fromTheme(ICON_SAVE));
    m_ui->pushButtonReset->setIcon(QIcon::fromTheme(ICON_CLEAR));
    m_ui->pushButtonUndo->setIcon(QIcon::fromTheme(ICON_UNDO));
}

Training::~Training()
{
    delete m_ui;
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
    m_ui->pushButtonProgress->setIcon(QIcon::fromTheme(ICON_PAUSE));
    m_ui->pushButtonProgress->setText(tr("Pause"));

    // Determine the last saved sentence in the session.

    // Begin an iteration of reading sentences until interrupted or completed.
    m_curSntct = m_session->firstIncompleteSentence();
    if (m_curSntct)
        navigateToPart(0);
}

void Training::stopCollecting()
{
    m_ui->pushButtonProgress->setIcon(QIcon::fromTheme(ICON_RECORD));
    m_ui->pushButtonProgress->setText(tr("Start"));
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
    m_ui->labelText->setEnabled(!checked);
    if (!checked)
        stopCollecting();
    else
        startCollecting();
}

void SpeechControl::Windows::Training::updateProgress(const double &p_progress)
{
    m_ui->progressBar->setValue((int)(p_progress * 100));
}

void SpeechControl::Windows::Training::open()
{
    m_ui->pushButtonProgress->setChecked(true);
    QDialog::open();
}

void SpeechControl::Windows::Training::navigateToPart(const int &l_index)
{
    m_posMin = (l_index == 0 || l_index == 1) ? 0 : m_range * (l_index - 1);
    m_posMax = m_range * (l_index + 1);
    qDebug() << m_posMin << m_posMax;

    QStringList l_words = m_curSntct->words();
    QString l_text;

    for (int i = 0; i < l_words.count(); i++){
        // determine if the index of the word falls between
        // the minimum or maximum value of the range.
        const QString l_curWord = l_words.at(i);

        if (m_posMin == i)
            l_text += "<b>";

        l_text += l_curWord;

        if (i != l_words.count() - 1)
            l_text += " ";

        if (m_posMax == i)
            l_text += "</b>";
    }

    m_pos = l_index;
    m_ui->labelText->setText(l_text);
}

/// @todo When this goes over, advance to the next sentence.
void SpeechControl::Windows::Training::navigateNextPart()
{
    if (m_pos + 1 == m_posMax)
        navigateToPart(m_pos + 1);
}

/// @todo When this hits -1, it should head back to the previous sentence.
void SpeechControl::Windows::Training::navigatePreviousPart()
{
    if (m_pos - 1 < 0)
        navigateToPart(m_pos - 1);
}

void SpeechControl::Windows::Training::startNavigating()
{
    m_pos = 0;
    QStringList l_words = m_curSntct->words();

    if (l_words.count() > 6)
        m_range = l_words.count() / 4;
    else
        m_range = 2;

    navigateToPart(0);
}

void SpeechControl::Windows::Training::stopNavigating()
{
    m_pos = 0;
}
