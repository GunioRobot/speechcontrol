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
    QDialog(parent), m_curPos(0), m_initPos(0), m_posMin(0), m_posMax(0),
    m_session(0), m_curSntct(0), m_initSntct(0),
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
        QMessageBox::information(0,tr("Session Completed"), tr("Your session is completed."));
}

/// @todo Start training the session.
void Training::startCollecting()
{
    // Configure the button.
    m_ui->pushButtonProgress->setIcon(QIcon::fromTheme(ICON_PAUSE));
    m_ui->pushButtonProgress->setText(tr("Pause"));
    m_ui->labelText->setText(tr("<i>Rendering...</i>"));

    // Determine the last saved sentence in the session.
    m_initSntct= m_curSntct = m_session->firstIncompleteSentence();

    // Begin an iteration of reading sentences until interrupted or completed.
    if (m_curSntct){
        int l_start = 0;
        const int l_max = m_curSntct->phrases().count();

        for (; l_start < l_max; l_start++){
            if (!m_curSntct->isPhraseCompleted(l_start)){
                m_initPos = l_start;
                break;
            }
        }

        navigateToPart(l_start);
    }
    else
        m_ui->labelText->setText(tr("<i>No text is available for this session</i>."));
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
    this->setWindowTitle(tr("Training (%1) - SpeechControl").arg(m_session->content()->title()));
    connect(m_session,SIGNAL(progressChanged(double)),this,SLOT(updateProgress(double)));
}

Session* Training::session() const
{
    return m_session;
}

void Training::on_pushButtonClose_clicked()
{
    reject();
}

void Training::on_pushButtonProgress_toggled(const bool& checked)
{
    m_ui->labelText->setEnabled(!checked);
    if (!checked)
        stopCollecting();
    else
        startCollecting();
}

void Training::updateProgress(const double &p_progress)
{
    m_ui->progressBar->setValue((int)(p_progress * 100));
}

void Training::open()
{
    m_ui->pushButtonProgress->setChecked(true);
    QDialog::open();
}

void Training::navigateToPart(const int &l_index)
{
    QString l_text;
    PhraseList l_phrsLst = m_curSntct->phrases();

    for (int i = 0; i < l_phrsLst.count(); i++){
        const QString l_curWord = l_phrsLst.at(i)->text();

        if (l_index == i)
            l_text += "<b>";

        l_text += l_curWord;

        if (l_index == i)
            l_text += "</b>";

        if (i != l_phrsLst.count() - 1)
            l_text += " ";
    }

    m_curPos = l_index;
    m_ui->labelText->setText(l_text);
    qDebug() << l_phrsLst.count() << l_text;
}

/// @todo When this goes over, advance to the next sentence.
void Training::navigateNextPart()
{
    if (m_curPos + 1 != m_curSntct->phrases().count())
        navigateToPart(m_curPos + 1);
}

/// @todo When this hits -1, it should head back to the previous sentence.
void Training::navigatePreviousPart()
{
    if (m_curPos - 1 < 0)
        navigateToPart(m_curPos - 1);
}

void Training::startNavigating()
{
    m_curPos = 0;
    navigateToPart(0);
}

void Training::stopNavigating()
{
    m_curPos = 0;
}

/// @todo This should clear all of the progress made since the start of training WHEN this dialog opened.
void SpeechControl::Windows::Training::on_pushButtonReset_clicked()
{
    // Undo the work up to the initial point.

    // Now, revert and jump to the beginning.
    m_curSntct = m_initSntct;
    m_curPos = m_initPos;
}

/// @todo This should undo progress at a decrementing interval until it hits the point of where the dialog opened.
void SpeechControl::Windows::Training::on_pushButtonUndo_clicked()
{
   navigatePreviousPart();
}

void SpeechControl::Windows::Training::on_pushButton_2_clicked()
{
    navigateNextPart();
}
