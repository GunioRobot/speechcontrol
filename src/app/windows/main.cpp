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
 
#include <QFile>
#include <QUuid>
#include <QDebug>
#include <QVariant>
#include <QSettings>
#include <QMessageBox>

#include <training.hpp>

#include "settings.hpp"
#include "ui_main.h"
#include "core.hpp"
#include "main.hpp"
#include "managers/books.hpp"
#include "managers/session.hpp"
#include "wizards/quickstart/wizard.hpp"

using namespace SpeechControl;
using namespace SpeechControl::Windows;
using namespace SpeechControl::Wizards;
using namespace SpeechControl::Windows::Managers;

using SpeechControl::Session;

Main::Main() : m_ui(new Ui::MainWindow) {
   m_ui->setupUi(this);
   m_ui->retranslateUi(this);

   this->restoreGeometry(Core::instance()->getConfig("MainWindow/Geometry").toByteArray());
   this->restoreState(Core::instance()->getConfig("MainWindow/State").toByteArray());

   m_ui->labelSessionCount->setText(tr("You have <b>%1</b> sessions.").arg(Session::allSessions().count()));
   m_ui->statusBar->showMessage("Ready.");
}

Main::~Main() {
    delete m_ui;
}

void SpeechControl::Windows::Main::on_actionOptions_triggered()
{
    Settings* l_settings = new Settings(this);
    l_settings->exec();
}

void SpeechControl::Windows::Main::on_actionAboutQt_triggered()
{
    QApplication::aboutQt();
}

/// @todo Implement About dialog using QMessageBox.
void SpeechControl::Windows::Main::on_actionAbout_SpeechControl_triggered()
{    
}

void SpeechControl::Windows::Main::on_pushButtonStartTrain_clicked()
{
    Session* l_session = SessionManager::doSelectSession();
}
